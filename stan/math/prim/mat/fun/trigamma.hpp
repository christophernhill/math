#ifndef STAN_MATH_PRIM_MAT_FUN_TRIGAMMA_HPP
#define STAN_MATH_PRIM_MAT_FUN_TRIGAMMA_HPP

#include <stan/math/prim/vectorize/apply_scalar_unary.hpp>
#include <stan/math/prim/scal/fun/trigamma.hpp>

namespace stan {
namespace math {

/**
 * Structure to wrap trigamma() so it can be vectorized.
 */
struct trigamma_fun {
  /**
   * Return the trigamma() function applied to
   * the argument.
   *
   * @tparam T type of argument
   * @param x argument
   * @return trigamma applied to argument.
   */
  template <typename T>
  static inline T fun(const T& x) {
    return trigamma(x);
  }
};

/**
 * Return the elementwise application of <code>trigamma()</code> to
 * specified argument container.  The return type promotes the
 * underlying scalar argument type to double if it is an integer,
 * and otherwise is the argument type.
 *
 * @tparam T type of container
 * @param x container
 * @return elementwise trigamma of container elements
 */
template <typename T>
inline auto trigamma(const T& x) {
  return apply_scalar_unary<trigamma_fun, T>::apply(x);
}

}  // namespace math
}  // namespace stan

#endif
