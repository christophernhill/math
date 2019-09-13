#ifndef STAN_MATH_PRIM_SCAL_PROB_UNIFORM_CDF_LOG_HPP
#define STAN_MATH_PRIM_SCAL_PROB_UNIFORM_CDF_LOG_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/scal/prob/uniform_lcdf.hpp>

namespace stan {
namespace math {

/**
 * @deprecated use <code>uniform_lcdf</code>
 */
template <typename T_y, typename T_low, typename T_high>
inline auto uniform_cdf_log(T_y&& y, T_low&& alpha, T_high&& beta) {
  return uniform_lcdf(std::forward<T_y>(y), std::forward<T_low>(alpha),
                      std::forward<T_high>(beta));
}

}  // namespace math
}  // namespace stan
#endif
