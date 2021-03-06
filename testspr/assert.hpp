/*=============================================================================
  Copyright (c) 2011-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef TESTSPR_ASSERT_HPP
#define TESTSPR_ASSERT_HPP

#include <string>
#include <stdexcept>
#ifdef TESTSPR_CONFIG_ENABLE_STATIC_WARNING
#	include <boost/serialization/static_warning.hpp>
#endif
#include <sprout/assert.hpp>

namespace testspr {
	//
	// assertion_failed
	//
	class assertion_failed
		: public std::runtime_error
	{
	public:
		explicit assertion_failed(std::string const& msg)
			: std::runtime_error(msg)
		{}
		explicit assertion_failed(char const* msg)
			: std::runtime_error(msg)
		{}
	};

	namespace detail {
		inline SPROUT_CONSTEXPR bool
		assertion_check(bool cond, std::string const& msg) {
			return cond ? true
				: throw testspr::assertion_failed(msg)
				;
		}
		inline SPROUT_CONSTEXPR bool
		assertion_check(bool cond, char const* msg) {
			return cond ? true
				: throw testspr::assertion_failed(msg)
				;
		}
	}	// namespace detail
}	// namespace testspr

//
// TESTSPR_STATIC_ASSERT
// TESTSPR_ASSERT
//
#define TESTSPR_STATIC_ASSERT(expr) \
	static_assert(expr, #expr)
#define TESTSPR_ASSERT(expr) \
	((void)sprout::detail::assertion_check((expr), SPROUT_ASSERTION_FAILED_FORMAT(expr, __FILE__, __LINE__)))
//
// TESTSPR_BOTH_ASSERT
//
#ifndef SPROUT_CONFIG_DISABLE_CONSTEXPR
#	define TESTSPR_BOTH_ASSERT(expr) TESTSPR_STATIC_ASSERT(expr); TESTSPR_ASSERT(expr)
#else	// #ifndef SPROUT_CONFIG_DISABLE_CONSTEXPR
#	define TESTSPR_BOTH_ASSERT(expr) TESTSPR_ASSERT(expr)
#endif	// #ifndef SPROUT_CONFIG_DISABLE_CONSTEXPR

//
// TESTSPR_STATIC_WARNING
//
#ifdef TESTSPR_CONFIG_ENABLE_STATIC_WARNING
#	define TESTSPR_STATIC_WARNING(expr) BOOST_STATIC_WARNING(expr)
#else
#	define TESTSPR_STATIC_WARNING(expr)
#endif

//
// TESTSPR_STATIC_UNCHECKED
// TESTSPR_UNCHECKED
// TESTSPR_BOTH_UNCHECKED
//
#define TESTSPR_STATIC_UNCHECKED(expr) TESTSPR_STATIC_WARNING(expr)
#define TESTSPR_UNCHECKED(expr) (expr)
#ifndef SPROUT_CONFIG_DISABLE_CONSTEXPR
#	define TESTSPR_BOTH_UNCHECKED(expr) TESTSPR_STATIC_UNCHECKED(expr); TESTSPR_UNCHECKED(expr)
#else	// #ifndef SPROUT_CONFIG_DISABLE_CONSTEXPR
#	define TESTSPR_BOTH_UNCHECKED(expr) TESTSPR_UNCHECKED(expr)
#endif	// #ifndef SPROUT_CONFIG_DISABLE_CONSTEXPR

#endif	// #ifndef TESTSPR_ASSERT_HPP
