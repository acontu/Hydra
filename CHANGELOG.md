CHANGES


### Hydra 2.2.0

# New features

1. ChiSquare functor and analytical integral
2. Orthogonal polynomials (`hydra/functions/Math.h`)
  
  * Chebychev 1st and 2nd kinds 
  * Laguerre
  * Hermite
  * Legendre

3. 1D kernel density estimation functor: GaussianKDE 
4. Bifurcated Gaussian functor:  BifurcatedGaussian
5. `Parameter::Create('name')` method added 

# Bug fixes

1. Null pointer breaking build in CLING
2. `multiarray::insert(...)` bug fixes