#ifndef STAN_MATH_PRIM_SCAL_FUN_COMMON_TYPE_HPP
#define STAN_MATH_PRIM_SCAL_FUN_COMMON_TYPE_HPP

#include <stan/math/prim/meta.hpp>

namespace stan {
namespace math {
/**
 * Struct which calculates type promotion given two types.
 *
 * <p>This is the base implementation for scalar types.
 * Allowed promotions are:
 * - int to double
 * - int to var
 * - double to var
 *
 * <p>Promotion between differing var types is not allowed, i.e.,
 * cannot promote fvar to var or vice versa.
 *
 * @tparam T1 scalar type
 * @tparam T2 scalar type
 */
template <typename T1, typename T2>
struct common_type {
  using type = return_type_t<T1, T2>;
};

}  // namespace math
}  // namespace stan

#endif
