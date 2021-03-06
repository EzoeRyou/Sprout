/*=============================================================================
  Copyright (c) 2011-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_TYPE_TRAITS_IS_INT_HPP
#define SPROUT_TYPE_TRAITS_IS_INT_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/type_traits/integral_constant.hpp>

namespace sprout {
	//
	// is_int
	//
	template<typename T>
	struct is_int
		: public sprout::integral_constant<
			bool,
			std::is_integral<T>::value && std::is_signed<T>::value
		>
	{};

#if SPROUT_USE_TEMPLATE_ALIASES
	template<typename T>
	using is_int_t = typename sprout::is_int<T>::type;
#endif	// #if SPROUT_USE_TEMPLATE_ALIASES
}	// namespace sprout

#endif	// #ifndef SPROUT_TYPE_TRAITS_IS_INT_HPP
