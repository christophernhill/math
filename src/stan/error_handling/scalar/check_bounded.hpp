#ifndef STAN__ERROR_HANDLING__SCALAR__CHECK_BOUNDED_HPP
#define STAN__ERROR_HANDLING__SCALAR__CHECK_BOUNDED_HPP

#include <stan/error_handling/domain_error.hpp>
#include <stan/error_handling/domain_error_vec.hpp>
#include <stan/meta/traits.hpp>

namespace stan {
  namespace error_handling {

    namespace detail {
      
      // implemented using structs because there is no partial specialization
      // for templated functions
      //
      // default implementation works for scalar T_y. T_low and T_high can
      // be either scalar or vector
      //
      // throws if y, low, or high is nan
      template <typename T_y, typename T_low, typename T_high,
                bool y_is_vec>
      struct bounded {
        static bool check(const std::string& function,
                          const std::string& name,
                          const T_y& y,
                          const T_low& low,
                          const T_high& high) {
          using stan::max_size;

          VectorView<const T_low> low_vec(low);
          VectorView<const T_high> high_vec(high);
          for (size_t n = 0; n < max_size(low, high); n++) {
            if (!(low_vec[n] <= y && y <= high_vec[n])) {
              std::stringstream msg;
              msg << ", but must be between ";
              msg << "(" << low_vec[n] << ", " << high_vec[n] << ")";

              domain_error(function, name, y,
                      "is ", msg.str());
            }
          }
          return true;
        }
      };
    
      template <typename T_y, typename T_low, typename T_high>
      struct bounded<T_y, T_low, T_high, true> {
        static bool check(const std::string& function,
                          const std::string& name,
                          const T_y& y,
                          const T_low& low,
                          const T_high& high) {
          using stan::length;
          using stan::get;
          
          VectorView<const T_low> low_vec(low);
          VectorView<const T_high> high_vec(high);
          for (size_t n = 0; n < length(y); n++) {
            if (!(low_vec[n] <= get(y,n) && get(y,n) <= high_vec[n])) {
              std::stringstream msg;
              msg << ", but must be between ";
              msg << "(" << low_vec[n] << ", " << high_vec[n] << ")";
              domain_error_vec(function, name, y, n,
                          "is ", msg.str());
            }
          }
          return true;
        }
      };
    }

    /**
     * Return <code>true</code> if the value is between the low and high
     * values, inclusively.
     *
     * @tparam T_y Type of value
     * @tparam T_low Type of low value
     * @tparam T_high Type of high value
     *
     * @param function Function name (for error messages)
     * @param name Variable name (for error messages)
     * @param y Value to check
     * @param low Low bound
     * @param high High bound
     *
     * @return <code>true</code> if the value is between low and high,
     *   inclusively.
     * @throw <code>std::domain_error</code> otherwise. This also throws
     *   if any of the arguments are NaN.
     */
    template <typename T_y, typename T_low, typename T_high>
    inline bool check_bounded(const std::string& function,
                              const std::string& name,  
                              const T_y& y,
                              const T_low& low,
                              const T_high& high) {
      return detail::bounded<T_y, T_low, T_high, 
                             is_vector_like<T_y>::value>
        ::check(function, name, y, low, high);
    }

  }
}
#endif
