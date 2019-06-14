#ifndef STAN_MATH_OPENCL_KERNELS_SUBTRACT_HPP
#define STAN_MATH_OPENCL_KERNELS_SUBTRACT_HPP
#ifdef STAN_OPENCL

#include <stan/math/opencl/kernel_cl.hpp>
#include <stan/math/opencl/buffer_types.hpp>

namespace stan {
namespace math {
namespace opencl_kernels {
// \cond
static const char *subtract_kernel_code = STRINGIFY(
    // \endcond
    /**
     * Matrix subtraction on the OpenCL device
     * Subtracts the second matrix from the
     * first matrix and stores the result
     * in the third matrix (C=A-B).
     *
     * @param[out] C The output matrix.
     * @param[in] B RHS input matrix.
     * @param[in] A LHS input matrix.
     * @param rows The number of rows for matrix A.
     * @param cols The number of columns for matrix A.
     * @note Code is a <code>const char*</code> held in
     * <code>subtract_kernel_code.</code>
     * Used in math/opencl/subtract_opencl.hpp
     *  This kernel uses the helper macros available in helpers.cl.
     */
    __kernel void subtract(__global double *C, __global double *A,
                           __global double *B, unsigned int rows,
                           unsigned int cols, int part_A, int part_B) {
      int i = get_global_id(0);
      int j = get_global_id(1);

      if (i < rows && j < cols) {
        double a;
        if ((!(part_A & LOWER) && j < i) || (!(part_A & UPPER) && j > i)) {
          a = 0;
        } else {
          a = A(i, j);
        }
        double b;
        if ((!(part_B & LOWER) && j < i) || (!(part_B & UPPER) && j > i)) {
          b = 0;
        } else {
          b = B(i, j);
        }
        C(i, j) = a - b;
      }
    }
    // \cond
);
// \endcond

/**
 * See the docs for \link kernels/subtract.hpp subtract() \endlink
 */
const kernel_cl<out_buffer, in_buffer, in_buffer, int, int, TriangularViewCL,
                TriangularViewCL>
    subtract("subtract", {indexing_helpers, subtract_kernel_code});

}  // namespace opencl_kernels
}  // namespace math
}  // namespace stan
#endif
#endif
