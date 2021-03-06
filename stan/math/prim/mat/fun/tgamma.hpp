#ifndef STAN_MATH_PRIM_MAT_FUN_TGAMMA_HPP
#define STAN_MATH_PRIM_MAT_FUN_TGAMMA_HPP

#include <stan/math/prim/vectorize/apply_scalar_unary.hpp>
#include <stan/math/prim/scal/fun/tgamma.hpp>

namespace stan {
namespace math {

/**
 * Structure to wrap tgamma() so that it can be vectorized.
 *
 * @tparam T type of variable
 * @param x variable
 * @return Gamma function applied to x.
 * @throw std::domain_error if x is 0 or a negative integer
 */
struct tgamma_fun {
  template <typename T>
  static inline T fun(const T& x) {
    return tgamma(x);
  }
};

/**
 * Vectorized version of tgamma().
 *
 * @tparam T type of container
 * @param x container
 * @return Gamma function applied to each value in x.
 * @throw std::domain_error if any value is 0 or a negative integer
 */
template <typename T>
inline auto tgamma(const T& x) {
  return apply_scalar_unary<tgamma_fun, T>::apply(x);
}

}  // namespace math
}  // namespace stan

#endif
