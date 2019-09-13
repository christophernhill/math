#ifndef STAN_MATH_PRIM_SCAL_PROB_HYPERGEOMETRIC_LPMF_HPP
#define STAN_MATH_PRIM_SCAL_PROB_HYPERGEOMETRIC_LPMF_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/scal/err/check_consistent_sizes.hpp>
#include <stan/math/prim/scal/err/check_bounded.hpp>
#include <stan/math/prim/scal/err/check_greater.hpp>
#include <stan/math/prim/scal/fun/size_zero.hpp>
#include <stan/math/prim/scal/fun/binomial_coefficient_log.hpp>

namespace stan {
namespace math {

// Hypergeometric(n|N, a, b)  [0 <= n <= a;  0 <= N-n <= b;  0 <= N <= a+b]
// n: #white balls drawn;  N: #balls drawn;
// a: #white balls;  b: #black balls
template <bool propto, typename T_n, typename T_N, typename T_a, typename T_b>
auto hypergeometric_lpmf(T_n&& n, T_N&& N, T_a&& a, T_b&& b) {
  static const char* function = "hypergeometric_lpmf";
  using T_partials = partials_return_t<T_n, T_N, T_a, T_b>;
  using T_return = return_type_t<T_n, T_N, T_a, T_b>;
  T_partials logp(0.0);

  check_bounded(function, "Successes variable", n, 0, a);
  check_greater(function, "Draws parameter", N, n);
  check_consistent_sizes(function, "Successes variable", n, "Draws parameter",
                         N, "Successes in population parameter", a,
                         "Failures in population parameter", b);
  const scalar_seq_view<T_n> n_vec(n);
  const scalar_seq_view<T_N> N_vec(N);
  const scalar_seq_view<T_a> a_vec(a);
  const scalar_seq_view<T_b> b_vec(b);
  const size_t size = max_size(n, N, a, b);
  if (!include_summand<propto>::value || size_zero(n, N, a, b)) {
    return T_return(logp);
  }
  for (size_t i = 0; i < size; i++) {
    check_bounded(function, "Draws parameter minus successes variable",
                  N_vec[i] - n_vec[i], 0, b_vec[i]);
    check_bounded(function, "Draws parameter", N_vec[i], 0,
                  a_vec[i] + b_vec[i]);
  }

  for (size_t i = 0; i < size; i++) {
    logp += math::binomial_coefficient_log(a_vec[i], n_vec[i])
            + math::binomial_coefficient_log(b_vec[i], N_vec[i] - n_vec[i])
            - math::binomial_coefficient_log(a_vec[i] + b_vec[i], N_vec[i]);
  }
  return T_return(logp);
}

template <typename T_n, typename T_N, typename T_a, typename T_b>
inline double hypergeometric_lpmf(T_n&& n, T_N&& N, T_a&& a, T_b&& b) {
  return hypergeometric_lpmf<false>(n, N, a, b);
}

}  // namespace math
}  // namespace stan
#endif
