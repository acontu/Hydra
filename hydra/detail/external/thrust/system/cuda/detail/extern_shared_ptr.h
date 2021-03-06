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

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace cuda
{
namespace detail
{

template<typename T>
  class extern_shared_ptr
{
// don't attempt to compile with any compiler other than nvcc
// due to use of __shared__ below
#if HYDRA_THRUST_DEVICE_COMPILER == HYDRA_THRUST_DEVICE_COMPILER_NVCC
  public:
    __hydra_device__
    inline operator T * (void)
    {
      extern __shared__ int4 smem[];
      return reinterpret_cast<T*>(smem);
    }

    __hydra_device__
    inline operator const T * (void) const
    {
      extern __shared__ int4 smem[];
      return reinterpret_cast<const T*>(smem);
    }
#endif // HYDRA_THRUST_DEVICE_COMPILER_NVCC
}; // end extern_shared_ptr

} // end detail
} // end cuda
} // end system
} // end thrust

HYDRA_EXTERNAL_NAMESPACE_END
