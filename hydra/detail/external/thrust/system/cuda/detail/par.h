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
#include <hydra/detail/external/thrust/system/cuda/detail/execution_policy.h>
#include <hydra/detail/external/thrust/detail/execute_with_allocator.h>
#include <hydra/detail/external/thrust/system/cuda/detail/execute_on_stream.h>
#include <hydra/detail/external/thrust/detail/allocator/allocator_traits.h>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace cuda
{
namespace detail
{


struct par_t : thrust::system::cuda::detail::execution_policy<par_t>
{
  par_t() : thrust::system::cuda::detail::execution_policy<par_t>() {}

  template<typename Allocator>
  __hydra_host__ __hydra_device__
  typename thrust::detail::enable_if<
    thrust::detail::is_allocator<Allocator>::value,
    thrust::detail::execute_with_allocator<Allocator, execute_on_stream_base>
  >::type
    operator()(Allocator &alloc) const
  {
    return thrust::detail::execute_with_allocator<Allocator, execute_on_stream_base>(alloc);
  }

  __hydra_host__ __hydra_device__
  inline execute_on_stream on(const cudaStream_t &stream) const
  {
    return execute_on_stream(stream);
  }
};


} // end detail


#ifdef __CUDA_ARCH__
static const __hydra_device__ detail::par_t par;
#else
static const detail::par_t par;
#endif


} // end cuda
} // end system


// alias par here
namespace cuda
{


using thrust::system::cuda::par;


} // end cuda
} // end thrust

HYDRA_EXTERNAL_NAMESPACE_END
