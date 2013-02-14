#ifndef SPROUT_MATH_FLOAT_SIGNIFICAND_HPP
#define SPROUT_MATH_FLOAT_SIGNIFICAND_HPP

#include <limits>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/math/detail/config.hpp>
#include <sprout/detail/pow.hpp>
#include <sprout/math/float_exponent.hpp>
#include <sprout/type_traits/enabler_if.hpp>

namespace sprout {
	namespace math {
		namespace detail {
			template<
				typename FloatType,
				typename sprout::enabler_if<std::is_floating_point<FloatType>::value>::type = sprout::enabler
			>
			inline SPROUT_CONSTEXPR FloatType
			float_significand(FloatType x) {
				return x == 0 ? FloatType(0)
					: x == std::numeric_limits<FloatType>::infinity() ? std::numeric_limits<FloatType>::infinity()
					: x == -std::numeric_limits<FloatType>::infinity() ? -std::numeric_limits<FloatType>::infinity()
					: x == std::numeric_limits<FloatType>::quiet_NaN() ? std::numeric_limits<FloatType>::quiet_NaN()
					: x / sprout::detail::pow_n(FloatType(std::numeric_limits<FloatType>::radix), sprout::math::float_exponent(x))
					;
			}

			template<
				typename IntType,
				typename sprout::enabler_if<std::is_integral<IntType>::value>::type = sprout::enabler
			>
			inline SPROUT_CONSTEXPR double
			float_significand(IntType x) {
				return sprout::math::detail::float_significand(static_cast<double>(x));
			}
		}	// namespace detail

		using sprout::math::detail::float_significand;
	}	// namespace math

	using sprout::math::float_significand;
}	// namespace sprout

#endif	// #ifndef SPROUT_MATH_FLOAT_SIGNIFICAND_HPP
