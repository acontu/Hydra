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


/*! \file runtime_introspection.h
 *  \brief Defines the interface to functions
 *         providing introspection into the architecture
 *         of CUDA devices.
 */

#pragma once

#include <hydra/detail/external/thrust/detail/config.h>

// #include this for device_properties_t and function_attributes_t
#include <hydra/detail/external/thrust/system/cuda/detail/cuda_launch_config.h>

// #include this for size_t
#include <cstddef>

HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust
{
namespace system
{
namespace cuda
{
namespace detail
{


/*! Returns the current device ordinal.
 */
inline __hydra_host__ __hydra_device__
int current_device();


/*! Returns a copy of the device_properties_t structure
 *  that is associated with a given device.
 */
inline __hydra_host__ __hydra_device__
device_properties_t device_properties(int device_id);


/*! Returns a copy of the device_properties_t structure
 *  that is associated with the current device.
 */
inline __hydra_host__ __hydra_device__
device_properties_t device_properties();


/*! Returns a copy of the function_attributes_t structure
 *  that is associated with a given __global__ function
 */
template<typename KernelFunction>
inline __hydra_host__ __hydra_device__
function_attributes_t function_attributes(KernelFunction kernel);


/*! Returns the compute capability of a device in integer format.
 *  For example, returns 10 for sm_10 and 21 for sm_21
 *  \return The compute capability as an integer
 */
inline __hydra_host__ __hydra_device__
size_t compute_capability(const device_properties_t &properties);


/*! Returns the compute capability of the current device in integer format.
 *  For example, returns 10 for sm_10 and 21 for sm_21
 *  \return The compute capability as an integer
 */
inline __hydra_host__ __hydra_device__
size_t compute_capability();


} // end namespace detail
} // end namespace cuda
} // end namespace system
} // end HYDRA_EXTERNAL_NAMESPACE_BEGIN  namespace thrust

HYDRA_EXTERNAL_NAMESPACE_END

#include <hydra/detail/external/thrust/system/cuda/detail/runtime_introspection.inl>

