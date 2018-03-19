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
#include <hydra/detail/external/thrust/detail/execution_policy.h>
#include <hydra/detail/external/thrust/detail/execute_with_allocator.h>
#include <hydra/detail/external/thrust/system/detail/sequential/execution_policy.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace detail
{


struct seq_t : thrust::system::detail::sequential::execution_policy<seq_t>
{
  __hydra_host__ __hydra_device__
  seq_t() : thrust::system::detail::sequential::execution_policy<seq_t>() {}

  // allow any execution_policy to convert to seq_t
  template<typename DerivedPolicy>
  __hydra_host__ __hydra_device__
  seq_t(const thrust::execution_policy<DerivedPolicy> &)
    : thrust::system::detail::sequential::execution_policy<seq_t>()
  {}

  template<typename Allocator>
    thrust::detail::execute_with_allocator<Allocator, thrust::system::detail::sequential::execution_policy>
      operator()(Allocator &alloc) const
  {
    return thrust::detail::execute_with_allocator<Allocator, thrust::system::detail::sequential::execution_policy>(alloc);
  }
};


} // end detail


#ifdef __CUDA_ARCH__
static const __hydra_device__ detail::seq_t seq;
#else
static const detail::seq_t seq;
#endif


} // end thrust


HYDRA_EXTERNAL_NAMESPACE_END
