#ifndef STAN_MATH_PRIM_SCAL_PROB_PARETO_CDF_LOG_HPP
#define STAN_MATH_PRIM_SCAL_PROB_PARETO_CDF_LOG_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/scal/prob/pareto_lcdf.hpp>

namespace stan {
namespace math {

/**
 * @deprecated use <code>pareto_lcdf</code>
 */
template <typename T_y, typename T_scale, typename T_shape>
inline auto pareto_cdf_log(T_y&& y, T_scale&& y_min, T_shape&& alpha) {
  return pareto_lcdf(y, y_min, alpha);
}

}  // namespace math
}  // namespace stan
#endif
