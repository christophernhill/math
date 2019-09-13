#ifndef STAN_MATH_PRIM_SCAL_PROB_BERNOULLI_LOGIT_LPMF_HPP
#define STAN_MATH_PRIM_SCAL_PROB_BERNOULLI_LOGIT_LPMF_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/scal/err/check_consistent_sizes.hpp>
#include <stan/math/prim/scal/err/check_bounded.hpp>
#include <stan/math/prim/scal/err/check_not_nan.hpp>
#include <stan/math/prim/scal/fun/size_zero.hpp>
#include <stan/math/prim/scal/fun/log1p.hpp>
#include <stan/math/prim/scal/fun/value_of.hpp>
#include <cmath>
#include <utility>

namespace stan {
namespace math {

/**
 * Returns the log PMF of the logit-parametrized Bernoulli distribution. If
 * containers are supplied, returns the log sum of the probabilities.
 *
 * @tparam T_n type of integer parameter
 * @tparam T_prob type of chance of success parameter
 * @param n integer parameter
 * @param theta logit-transformed chance of success parameter
 * @return log probability or log sum of probabilities
 * @throw std::domain_error if theta is infinite.
 * @throw std::invalid_argument if container sizes mismatch.
 */
template <bool propto, typename T_n, typename T_prob>
inline auto bernoulli_logit_lpmf(T_n&& n, T_prob&& theta) {
  static const char* function = "bernoulli_logit_lpmf";
  using T_partials = partials_return_t<T_n, T_prob>;
  T_partials logp(0.0);

  using std::exp;

  check_bounded(function, "n", n, 0, 1);
  check_not_nan(function, "Logit transformed probability parameter", theta);
  check_consistent_sizes(function, "Random variable", n,
                         "Probability parameter", theta);

  const scalar_seq_view<T_n> n_vec(n);
  const scalar_seq_view<T_prob> theta_vec(theta);
  const size_t N = max_size(n, theta);
  operands_and_partials<T_prob> ops_partials(theta);
  if (!include_summand<propto, T_prob>::value) {
    return ops_partials.build(logp);
  } else if (size_zero(n, theta)) {
    return ops_partials.build(logp);
  }

  for (size_t n = 0; n < N; n++) {
    const T_partials theta_dbl = value_of(theta_vec[n]);

    const int sign = 2 * n_vec[n] - 1;
    const T_partials ntheta = sign * theta_dbl;
    const T_partials exp_m_ntheta = exp(-ntheta);

    // Handle extreme values gracefully using Taylor approximations.
    static const double cutoff = 20.0;
    if (ntheta > cutoff) {
      logp -= exp_m_ntheta;
    } else if (ntheta < -cutoff) {
      logp += ntheta;
    } else {
      logp -= log1p(exp_m_ntheta);
    }

    if (!is_constant_all<T_prob>::value) {
      if (ntheta > cutoff) {
        ops_partials.edge1_.partials_[n] -= exp_m_ntheta;
      } else if (ntheta < -cutoff) {
        ops_partials.edge1_.partials_[n] += sign;
      } else {
        ops_partials.edge1_.partials_[n]
            += sign * exp_m_ntheta / (exp_m_ntheta + 1);
      }
    }
  }
  return ops_partials.build(logp);
}

template <typename T_n, typename T_prob>
inline auto bernoulli_logit_lpmf(T_n&& n, T_prob&& theta) {
  return bernoulli_logit_lpmf<false>(std::forward<T_n>(n),
                                     std::forward<T_prob>(theta));
}

}  // namespace math
}  // namespace stan
#endif
