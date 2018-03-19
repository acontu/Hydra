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


#pragma once

#include <hydra/detail/external/thrust/detail/config.h>
#include <hydra/detail/external/thrust/system/detail/generic/tag.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace detail
{
namespace generic
{

template<typename ExecutionPolicy,
         typename ForwardIterator,
         typename Generator>
__hydra_host__ __hydra_device__
  void generate(thrust::execution_policy<ExecutionPolicy> &exec,
                ForwardIterator first,
                ForwardIterator last,
                Generator gen);

template<typename ExecutionPolicy,
         typename OutputIterator,
         typename Size,
         typename Generator>
__hydra_host__ __hydra_device__
  OutputIterator generate_n(thrust::execution_policy<ExecutionPolicy> &exec,
                            OutputIterator first,
                            Size n,
                            Generator gen);

} // end namespace generic
} // end namespace detail
} // end namespace system
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust

HYDRA_EXTERNAL_NAMESPACE_END

#include <hydra/detail/external/thrust/system/detail/generic/generate.inl>

