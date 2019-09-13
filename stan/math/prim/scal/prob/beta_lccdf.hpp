#ifndef STAN_MATH_PRIM_SCAL_PROB_BETA_LCCDF_HPP
#define STAN_MATH_PRIM_SCAL_PROB_BETA_LCCDF_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/scal/err/check_consistent_sizes.hpp>
#include <stan/math/prim/scal/err/check_less_or_equal.hpp>
#include <stan/math/prim/scal/err/check_nonnegative.hpp>
#include <stan/math/prim/scal/err/check_not_nan.hpp>
#include <stan/math/prim/scal/err/check_positive_finite.hpp>
#include <stan/math/prim/scal/fun/size_zero.hpp>
#include <stan/math/prim/scal/fun/value_of.hpp>
#include <stan/math/prim/scal/fun/digamma.hpp>
#include <stan/math/prim/scal/fun/beta.hpp>
#include <stan/math/prim/scal/fun/grad_reg_inc_beta.hpp>
#include <stan/math/prim/scal/fun/inc_beta.hpp>
#include <cmath>

namespace stan {
namespace math {

/**
 * Returns the beta log complementary cumulative distribution function
 * for the given probability, success, and failure parameters.  Any
 * arguments other than scalars must be containers of the same size.
 * With non-scalar arguments, the return is the sum of the log ccdfs
 * with scalars broadcast as necessary.
 *
 * @tparam T_y type of y
 * @tparam T_scale_succ type of success parameter
 * @tparam T_scale_fail type of failure parameter
 * @param y (Sequence of) scalar(s) between zero and one
 * @param alpha (Sequence of) success parameter(s)
 * @param beta (Sequence of) failure parameter(s)
 * @return log probability or sum of log of proabilities
 * @throw std::domain_error if alpha or beta is negative
 * @throw std::domain_error if y is not a valid probability
 * @throw std::invalid_argument if container sizes mismatch
 */
template <typename T_y, typename T_scale_succ, typename T_scale_fail>
inline auto beta_lccdf(T_y&& y, T_scale_succ&& alpha, T_scale_fail&& beta) {
  using T_partials = partials_return_t<T_y, T_scale_succ, T_scale_fail>;
  T_partials ccdf_log(0.0);
  static const char* function = "beta_lccdf";
  check_positive_finite(function, "First shape parameter", alpha);
  check_positive_finite(function, "Second shape parameter", beta);
  check_not_nan(function, "Random variable", y);
  check_nonnegative(function, "Random variable", y);
  check_less_or_equal(function, "Random variable", y, 1);
  check_consistent_sizes(function, "Random variable", y,
                         "First shape parameter", alpha,
                         "Second shape parameter", beta);

  const scalar_seq_view<T_y> y_vec(y);
  const scalar_seq_view<T_scale_succ> alpha_vec(alpha);
  const scalar_seq_view<T_scale_fail> beta_vec(beta);
  const size_t N = max_size(y, alpha, beta);

  operands_and_partials<T_y, T_scale_succ, T_scale_fail> ops_partials(y, alpha,
                                                                      beta);
  if (size_zero(y, alpha, beta)) {
    return ops_partials.build(ccdf_log);
  }

  using std::exp;
  using std::log;
  using std::pow;

  VectorBuilder<!is_constant_all<T_scale_succ, T_scale_fail>::value, T_partials,
                T_scale_succ, T_scale_fail>
      digamma_alpha_vec(max_size(alpha, beta));
  VectorBuilder<!is_constant_all<T_scale_succ, T_scale_fail>::value, T_partials,
                T_scale_succ, T_scale_fail>
      digamma_beta_vec(max_size(alpha, beta));
  VectorBuilder<!is_constant_all<T_scale_succ, T_scale_fail>::value, T_partials,
                T_scale_succ, T_scale_fail>
      digamma_sum_vec(max_size(alpha, beta));

  if (!is_constant_all<T_scale_succ, T_scale_fail>::value) {
    for (size_t i = 0; i < max_size(alpha, beta); i++) {
      const T_partials alpha_dbl = value_of(alpha_vec[i]);
      const T_partials beta_dbl = value_of(beta_vec[i]);

      digamma_alpha_vec[i] = digamma(alpha_dbl);
      digamma_beta_vec[i] = digamma(beta_dbl);
      digamma_sum_vec[i] = digamma(alpha_dbl + beta_dbl);
    }
  }

  for (size_t n = 0; n < N; n++) {
    const T_partials y_dbl = value_of(y_vec[n]);
    const T_partials alpha_dbl = value_of(alpha_vec[n]);
    const T_partials beta_dbl = value_of(beta_vec[n]);
    const T_partials betafunc_dbl = stan::math::beta(alpha_dbl, beta_dbl);

    const T_partials Pn = 1.0 - inc_beta(alpha_dbl, beta_dbl, y_dbl);

    ccdf_log += log(Pn);

    if (!is_constant_all<T_y>::value) {
      ops_partials.edge1_.partials_[n] -= pow(1 - y_dbl, beta_dbl - 1)
                                          * pow(y_dbl, alpha_dbl - 1)
                                          / betafunc_dbl / Pn;
    }

    T_partials g1 = 0;
    T_partials g2 = 0;

    if (!is_constant_all<T_scale_succ, T_scale_fail>::value) {
      grad_reg_inc_beta(g1, g2, alpha_dbl, beta_dbl, y_dbl,
                        digamma_alpha_vec[n], digamma_beta_vec[n],
                        digamma_sum_vec[n], betafunc_dbl);
    }
    if (!is_constant_all<T_scale_succ>::value) {
      ops_partials.edge2_.partials_[n] -= g1 / Pn;
    }
    if (!is_constant_all<T_scale_fail>::value) {
      ops_partials.edge3_.partials_[n] -= g2 / Pn;
    }
  }
  return ops_partials.build(ccdf_log);
}

}  // namespace math
}  // namespace stan
#endif
