
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
 * LogNormal.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Marcos Romero Lamas
 */

#ifndef LOGNORMAL_H_
#define LOGNORMAL_H_

#include <hydra/detail/Config.h>
#include <hydra/detail/BackendPolicy.h>
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
#include <assert.h>
#include <utility>

namespace hydra {

template<unsigned int ArgIndex=0>
class LogNormal: public BaseFunctor<LogNormal<ArgIndex>, double, 2>
{
	using BaseFunctor<LogNormal<ArgIndex>, double, 2>::_par;

public:

	LogNormal() = delete;

	LogNormal(Parameter const& mean, Parameter const& sigma ):
		BaseFunctor<LogNormal<ArgIndex>, double, 2>({mean, sigma})
		{}

	__hydra_host__ __hydra_device__
	LogNormal(LogNormal<ArgIndex> const& other ):
		BaseFunctor<LogNormal<ArgIndex>, double,2>(other)
		{}

	__hydra_host__ __hydra_device__
	LogNormal<ArgIndex>&
	operator=(LogNormal<ArgIndex> const& other ){
		if(this==&other) return  *this;
		BaseFunctor<LogNormal<ArgIndex>,double, 2>::operator=(other);
		return  *this;
	}

	template<typename T>
	__hydra_host__ __hydra_device__
	inline double Evaluate(unsigned int, T*x)  const
	{
		double m2  = (::log(x[ArgIndex]) - _par[0])*(::log(x[ArgIndex]) - _par[0] );
		double s2  = _par[1]*_par[1];
		double val = (::exp(-m2/(2.0 * s2 ))) / x[ArgIndex];
		return  CHECK_VALUE( (x[ArgIndex]>0 ? val : 0) , "par[0]=%f, par[1]=%f", _par[0], _par[1]);
	}

	template<typename T>
	__hydra_host__ __hydra_device__
	inline double Evaluate(T x)  const
	{
		double m2  = ( ::log(get<ArgIndex>(x)) - _par[0])*( ::log(get<ArgIndex>(x)) - _par[0] );
		double s2  = _par[1]*_par[1];
		double val = (::exp(-m2/(2.0 * s2 ))) / get<ArgIndex>(x);
		return CHECK_VALUE( (get<ArgIndex>(x)>0 ? val : 0) , "par[0]=%f, par[1]=%f", _par[0], _par[1]);
	}

};


class LogNormalAnalyticalIntegral: public Integrator<LogNormalAnalyticalIntegral>
{

public:

	LogNormalAnalyticalIntegral(double min, double max):
		fLowerLimit(min),
		fUpperLimit(max)
	{
		assert( fLowerLimit < fUpperLimit && "hydra::LogNormalAnalyticalIntegral: MESSAGE << LowerLimit >= fUpperLimit >>");
	 }

	inline LogNormalAnalyticalIntegral(LogNormalAnalyticalIntegral const& other):
		fLowerLimit(other.GetLowerLimit()),
		fUpperLimit(other.GetUpperLimit())
	{}

	inline LogNormalAnalyticalIntegral&
	operator=( LogNormalAnalyticalIntegral const& other)
	{
		if(this == &other) return *this;

		this->fLowerLimit = other.GetLowerLimit();
		this->fUpperLimit = other.GetUpperLimit();

		return *this;
	}

	double GetLowerLimit() const
	{
		return fLowerLimit;
	}

	void SetLowerLimit(double lowerLimit )
	{
		fLowerLimit = lowerLimit;
	}

	double GetUpperLimit() const
	{
		return fUpperLimit;
	}

	void SetUpperLimit(double upperLimit)
	{
		fUpperLimit = upperLimit;
	}

	template<typename FUNCTOR>	inline
	std::pair<double, double> Integrate(FUNCTOR const& functor) const
	{

		double fraction = cumulative(functor[0], functor[1], fUpperLimit)
						- cumulative(functor[0], functor[1], fLowerLimit);

		return std::make_pair(
				CHECK_VALUE(fraction," par[0] = %f par[1] = %f fLowerLimit = %f fUpperLimit = %f", functor[0], functor[1], fLowerLimit,fUpperLimit ) ,0.0);
	}


private:

	inline double cumulative(const double mean, const double sigma, const double x) const
	{
		static const double sqrt_pi_over_two = 1.2533141373155002512079;
		static const double sqrt_two         = 1.4142135623730950488017;

		return sigma*sqrt_pi_over_two*( erf( (::log(x)-mean)/( sigma*sqrt_two ) ) );
	}

	double fLowerLimit;
	double fUpperLimit;

};



}  // namespace hydra


#endif /* LOGNORMAL_H_ */