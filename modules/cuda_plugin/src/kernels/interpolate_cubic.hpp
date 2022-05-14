// Copyright (C) 2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <vector>

#include "cuda_type_traits.hpp"
#include "error.hpp"
#include "interpolate_base.hpp"
#include "tensor_helpers.hpp"

namespace CUDAPlugin {
namespace kernel {

class InterpolateCubic : public InterpolateBase {
public:
    InterpolateCubic(std::vector<size_t> in_shape,
                     std::vector<size_t> axes,
                     std::vector<float> scales,
                     std::vector<size_t> out_shape,
                     CoordinateTransformMode transform_mode,
                     float cube_coeff,
                     Type_t element_type,
                     size_t max_threads_per_block);

    void operator()(const cudaStream_t stream, const void* input, void* output) const;

    std::vector<size_t> immutableWorkbufferSizes() const;
    void initImmutableWorkbuffers(const std::vector<void*>& buffers);

    struct Props {
        UIntShape input_shape{};
        UIntShape output_shape{};

        unsigned num_of_axes{};
        UIntShape axes{};
        FloatShape scales{};

        float cube_coeff{};
        CoordinateTransformMode transform_mode{};
    };

private:
    template <typename T, typename CT>
    void callKernel(const cudaStream_t stream, const void* input, void* output) const;

private:
    Props props_;
    const void* props_device_ptr_;
    std::vector<Index> indices_;
    const void* indices_device_ptr_;

    size_t num_blocks_;
    size_t threads_per_block_;
    Type_t element_type_;
};

}  // namespace kernel
}  // namespace CUDAPlugin