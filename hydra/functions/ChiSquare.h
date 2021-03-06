/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2016 - 2018 Antonio Augusto Alves Junior
 *
 *   This file is part of Hydra Data Analysis Framework.
 *
 *   Hydra is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Hydra is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Hydra.  If not, see <http://www.gnu.org/licenses/>.
 *
 *---------------------------------------------------------------------------*/

/*
 * ChiSquare.h
 *
 *  Created on: 31/03/2018
 *      Author: Antonio Augusto Alves Junior
 */

/**
 * \ingroup common_functions
 *
 */
#ifndef CHISQUARE_H_
#define CHISQUARE_H_

#include <hydra/Types.h>
#include <hydra/Function.h>
#include <hydra/Pdf.h>
#include <hydra/detail/Integrator.h>
#include <hydra/detail/utility/CheckValue.h>
#include <hydra/Parameter.h>
#include <hydra/Tuple.h>
#include <tuple>
#include <limits>
#include <stdexcept>
#include <cassert>
#include <utility>

namespace hydra {


template< unsigned int ArgIndex=0 >
class ChiSquare: public BaseFunctor< ChiSquare<ArgIndex>, double, 1>
{
	using BaseFunctor<ChiSquare<ArgIndex>, double, 1>::_par;

	public:

		ChiSquare()=delete;

		ChiSquare(Parameter const& ndof ):
			BaseFunctor<ChiSquare<ArgIndex>, double, 1>({ndof})
			{}

		__hydra_host__ __hydra_device__
		ChiSquare(ChiSquare<ArgIndex> const& other ):
			BaseFunctor<ChiSquare<ArgIndex>, double,1>(other)
			{}

		__hydra_host__ __hydra_device__
		inline ChiSquare<ArgIndex>&
		operator=(ChiSquare<ArgIndex> const& other ){
			if(this==&other) return  *this;
			BaseFunctor<ChiSquare<ArgIndex>,double, 1>::operator=(other);
			return  *this;
		}

		template<typename T>
		__hydra_host__ __hydra_device__
		inline double Evaluate(unsigned int, T*m)  const
		{
			double ndof  = _par[0];
			double x     = m[ArgIndex];

			double r = (x > 0)?::pow(x,(ndof/2.0)-1.0) * ::exp(-x/2.0) / (::tgamma(ndof/2.0)*::pow(2.0,ndof/2.0)):0.0;


			return CHECK_VALUE(r, "par[0]=%f", _par[0]) ;
		}

		template<typename T>
		__hydra_host__ __hydra_device__
		inline double Evaluate(T m)  const
		{
			double ndof  = _par[0];
			double x     = 	get<ArgIndex>(m);
			double r = (x > 0)?::pow(x,(ndof/2.0)-1.0) * ::exp(-x/2.0) / (::tgamma(ndof/2.0)*::pow(2.0,ndof/2.0)):0.0;


			return  CHECK_VALUE(r, "par[0]=%f", _par[0]) ;
		}


};



class ChiSquareAnalyticalIntegral: public Integrator<ChiSquareAnalyticalIntegral>
{

public:

	ChiSquareAnalyticalIntegral(double min, double max):
		fLowerLimit(min),
		fUpperLimit(max)
	{
		assert(fLowerLimit < fUpperLimit
				&& "hydra::ChiSquareAnalyticalIntegral: MESSAGE << LowerLimit >= fUpperLimit >>");
	}

	inline ChiSquareAnalyticalIntegral(ChiSquareAnalyticalIntegral const& other):
		fLowerLimit(other.GetLowerLimit()),
		fUpperLimit(other.GetUpperLimit())
	{}

	inline ChiSquareAnalyticalIntegral&
	operator=( ChiSquareAnalyticalIntegral const& other)
	{
		if(this == &other) return *this;

		this->fLowerLimit = other.GetLowerLimit();
		this->fUpperLimit = other.GetUpperLimit();

		return *this;
	}

	double GetLowerLimit() const {
		return fLowerLimit;
	}

	void SetLowerLimit(double lowerLimit ) {
		fLowerLimit = lowerLimit;
	}

	double GetUpperLimit() const {
		return fUpperLimit;
	}

	void SetUpperLimit(double upperLimit) {
		fUpperLimit = upperLimit;
	}

	template<typename FUNCTOR>	inline
	std::pair<double, double> Integrate(FUNCTOR const& functor) const {

		double r = cumulative(functor[0], fUpperLimit)
						 - cumulative(functor[0], fLowerLimit);

		return std::make_pair(
				CHECK_VALUE(r," par[0] = %f fLowerLimit = %f fUpperLimit = %f", functor[0], fLowerLimit,fUpperLimit ), 0.0);
	}


private:

	inline double cumulative( const double ndof, const double x) const
	{

		return igam(ndof/2.0, x/2.0);
	}

	// borrowed from Cephes
	/* left tail of incomplete gamma function:
	  *
	  *          inf.      k
	  *   a  -x   -       x
	  *  x  e     >   ----------
	  *           -     -
	  *          k=0   | (a+k+1)
	  *
	  */
	double igam( double a, double x ) const
	{
		double ans, ax, c, r;

		// LM: for negative values returns 1.0 instead of zero
		// This is correct if a is a negative integer since Gamma(-n) = +/- inf
		if (a <= 0)  return 1.0;

		if (x <= 0)  return 0.0;

		if( (x > 1.0) && (x > a ) )
			return( 1.0 - igamc(a,x) );

		/* Compute  x**a * exp(-x) / gamma(a)  */
		ax = a * ::log(x) - x - ::lgamma(a);
		if( ax < -kMAXLOG )
			return( 0.0 );

		ax = std::exp(ax);

		/* power series */
		r = a;
		c = 1.0;
		ans = 1.0;

		do
		{
			r += 1.0;
			c *= x/r;
			ans += c;
		}
		while( c/ans > kMACHEP );

		return( ans * ax/a );
	}

	 // incomplete gamma function (complement integral)
	 //  igamc(a,x)   =   1 - igam(a,x)
	 //
	 //                            inf.
	 //                              -
	 //                     1       | |  -t  a-1
	 //               =   -----     |   e   t   dt.
	 //                    -      | |
	 //                   | (a)    -
	 //                             x
	 //
	 //
	 // In this implementation both arguments must be positive.
	 // The integral is evaluated by either a power series or
	 // continued fraction expansion, depending on the relative
	 // values of a and x.

	 double igamc( double a, double x ) const
	 {

	    double ans, ax, c, yc, r, t, y, z;
	    double pk, pkm1, pkm2, qk, qkm1, qkm2;

	    // LM: for negative values returns 0.0
	    // This is correct if a is a negative integer since Gamma(-n) = +/- inf
	    if (a <= 0)  return 0.0;

	    if (x <= 0) return 1.0;

	    if( (x < 1.0) || (x < a) )
	       return( 1.0 - igam(a,x) );

	    ax = a * ::log(x) - x - ::lgamma(a);
	    if( ax < -kMAXLOG )
	       return( 0.0 );

	    ax = ::exp(ax);

	 /* continued fraction */
	    y = 1.0 - a;
	    z = x + y + 1.0;
	    c = 0.0;
	    pkm2 = 1.0;
	    qkm2 = x;
	    pkm1 = x + 1.0;
	    qkm1 = z * x;
	    ans = pkm1/qkm1;

	    do
	    {
	       c += 1.0;
	       y += 1.0;
	       z += 2.0;
	       yc = y * c;
	       pk = pkm1 * z  -  pkm2 * yc;
	       qk = qkm1 * z  -  qkm2 * yc;
	       if(qk)
	       {
	          r = pk/qk;
	          t = ::fabs( (ans - r)/r );
	          ans = r;
	       }
	       else
	          t = 1.0;
	       pkm2 = pkm1;
	       pkm1 = pk;
	       qkm2 = qkm1;
	       qkm1 = qk;
	       if( ::fabs(pk) > kBig )
	       {
	          pkm2 *= kBiginv;
	          pkm1 *= kBiginv;
	          qkm2 *= kBiginv;
	          qkm1 *= kBiginv;
	       }
	    }
	    while( t > kMACHEP );

	    return( ans * ax );
	 }


	double fLowerLimit;
	double fUpperLimit;

	/* the machine roundoff error */
	static constexpr double kMACHEP = 1.11022302462515654042363166809e-16;

	 /* largest argument for TMath::Exp() */
	static constexpr double kMAXLOG = 709.782712893383973096206318587;

    static constexpr  double kBig = 4.503599627370496e15;
    static constexpr  double kBiginv = 2.22044604925031308085e-16;
};



}  // namespace hydra



#endif /* CHISQUARE_H_ */
