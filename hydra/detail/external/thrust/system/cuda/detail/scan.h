/*
 *  Copyright 2008-2013 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


/*! \file scan.h
 *  \brief Scan operations (parallel prefix-sum) [cuda]
 */

#pragma once

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/system/cuda/detail/execution_policy.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace cuda
{
namespace detail
{


template<typename DerivedPolicy,
         typename InputIterator,
         typename OutputIterator,
         typename AssociativeOperator>
__hydra_host__ __hydra_device__
OutputIterator inclusive_scan(execution_policy<DerivedPolicy> &exec,
                              InputIterator first,
                              InputIterator last,
                              OutputIterator result,
                              AssociativeOperator binary_op);


template<typename DerivedPolicy,
         typename InputIterator,
         typename OutputIterator,
         typename T,
         typename AssociativeOperator>
__hydra_host__ __hydra_device__
OutputIterator exclusive_scan(execution_policy<DerivedPolicy> &exec,
                              InputIterator first,
                              InputIterator last,
                              OutputIterator result,
                              T init,
                              AssociativeOperator binary_op);


} // end namespace detail
} // end namespace cuda
} // end namespace system
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust

HYDRA_EXTERNAL_NAMESPACE_END

#include <hydra/detail/external/thrust/system/cuda/detail/scan.inl>

