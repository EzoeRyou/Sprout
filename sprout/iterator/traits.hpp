#ifndef SPROUT_ITERATOR_TRAITS_HPP
#define SPROUT_ITERATOR_TRAITS_HPP

#include <iterator>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/type_traits/has_xxx.hpp>

namespace sprout {
	namespace detail {
		// has_iterator_category
		SPROUT_HAS_XXX_TYPE_DEF_LAZY(iterator_category);
	}	// namespace detail
	//
	// is_iterator
	//
	template<typename Iterator>
	struct is_iterator
		: sprout::detail::has_iterator_category<std::iterator_traits<Iterator> >
	{};

	//
	// is_input_iterator_category
	//
	template<typename Category>
	struct is_input_iterator_category
		: public std::is_convertible<Category, std::input_iterator_tag>
	{};
	//
	// is_output_iterator_category
	//
	template<typename Category>
	struct is_output_iterator_category
		: public std::is_convertible<Category, std::output_iterator_tag>
	{};
	//
	// is_forward_iterator_category
	//
	template<typename Category>
	struct is_forward_iterator_category
		: public std::is_convertible<Category, std::forward_iterator_tag>
	{};
	//
	// is_bidirectional_iterator_category
	//
	template<typename Category>
	struct is_bidirectional_iterator_category
		: public std::is_convertible<Category, std::bidirectional_iterator_tag>
	{};
	//
	// is_random_access_iterator_category
	//
	template<typename Category>
	struct is_random_access_iterator_category
		: public std::is_convertible<Category, std::random_access_iterator_tag>
	{};

	//
	// is_input_iterator
	//
	template<typename Iterator>
	struct is_input_iterator
		: public sprout::is_input_iterator_category<typename std::iterator_traits<Iterator>::iterator_category>
	{};
	//
	// is_output_iterator
	//
	template<typename Iterator>
	struct is_output_iterator
		: public sprout::is_output_iterator_category<typename std::iterator_traits<Iterator>::iterator_category>
	{};
	//
	// is_forward_iterator
	//
	template<typename Iterator>
	struct is_forward_iterator
		: public sprout::is_forward_iterator_category<typename std::iterator_traits<Iterator>::iterator_category>
	{};
	//
	// is_bidirectional_iterator
	//
	template<typename Iterator>
	struct is_bidirectional_iterator
		: public sprout::is_bidirectional_iterator_category<typename std::iterator_traits<Iterator>::iterator_category>
	{};
	//
	// is_random_access_iterator
	//
	template<typename Iterator>
	struct is_random_access_iterator
		: public sprout::is_random_access_iterator_category<typename std::iterator_traits<Iterator>::iterator_category>
	{};

	namespace detail {
		template<typename Category, bool IsRandomAccess, bool IsBidirectional, bool IsForward, bool IsSinglePassOrIncrementable>
		struct iterator_category_hierarchy_impl;
		template<typename Category, bool IsBidirectional, bool IsForward, bool IsSinglePassOrIncrementable>
		struct iterator_category_hierarchy_impl<Category, true, IsBidirectional, IsForward, IsSinglePassOrIncrementable>
			: public std::integral_constant<int, 4>
		{};
		template<typename Category, bool IsForward, bool IsSinglePassOrIncrementable>
		struct iterator_category_hierarchy_impl<Category, false, true, IsForward, IsSinglePassOrIncrementable>
			: public std::integral_constant<int, 3>
		{};
		template<typename Category, bool IsSinglePassOrIncrementable>
		struct iterator_category_hierarchy_impl<Category, false, false, true, IsSinglePassOrIncrementable>
			: public std::integral_constant<int, 2>
		{};
		template<typename Category>
		struct iterator_category_hierarchy_impl<Category, false, false, false, true>
			: public std::integral_constant<int, 1>
		{};

		template<typename Category>
		struct iterator_category_hierarchy
			: public sprout::detail::iterator_category_hierarchy_impl<
				Category,
				sprout::is_random_access_iterator_category<Category>::value,
				sprout::is_bidirectional_iterator_category<Category>::value,
				sprout::is_forward_iterator_category<Category>::value,
				sprout::is_input_iterator_category<Category>::value || sprout::is_output_iterator_category<Category>::value
			>
		{};

		template<typename Category1, typename Category2>
		struct iterator_category_less
			: public std::integral_constant<
				bool,
				(sprout::detail::iterator_category_hierarchy<Category1>::value < sprout::detail::iterator_category_hierarchy<Category2>::value)
			>
		{};
	}	// namespace detail
	//
	// min_iterator_category
	//
	template<typename Head, typename... Tail>
	struct min_iterator_category;
	template<typename Category>
	struct min_iterator_category<Category>
		: public std::common_type<Category>
	{};
	template<typename Category1, typename Category2>
	struct min_iterator_category<Category1, Category2>
		: public std::conditional<
			sprout::detail::iterator_category_less<Category1, Category2>::value,
			Category1, Category2
		>
	{};
	template<typename Head, typename... Tail>
	struct min_iterator_category
		: public sprout::min_iterator_category<
			Head,
			typename sprout::min_iterator_category<Tail...>::type
		>
	{};

	//
	// common_iterator_category
	//
	template<typename... Iterators>
	struct common_iterator_category
		: public sprout::min_iterator_category<
			typename std::iterator_traits<Iterators>::iterator_category...
		>
	{};

	namespace detail {
		template<typename... Types>
		struct common_iterator_reference_impl;
		template<typename T>
		struct common_iterator_reference_impl<T> {
		public:
			typedef T type;
		};
		template<typename T, typename U>
		struct common_iterator_reference_impl<T, U>
			: public std::conditional<
				std::is_reference<T>::value && std::is_reference<U>::value,
				typename std::conditional<
					std::is_convertible<T, U>::value
						&& (std::is_same<typename std::decay<T>::type, typename std::decay<U>::type>::value
							|| std::is_base_of<typename std::decay<U>::type, typename std::decay<T>::type>::value
							)
						,
					U,
					typename std::conditional<
						std::is_convertible<U, T>::value
							&& (std::is_same<typename std::decay<U>::type, typename std::decay<T>::type>::value
								|| std::is_base_of<typename std::decay<T>::type, typename std::decay<U>::type>::value
								)
							,
						T,
						typename std::common_type<typename std::decay<T>::type, typename std::decay<U>::type>::type
					>::type
				>::type,
				typename std::common_type<typename std::decay<T>::type, typename std::decay<U>::type>::type
			>
		{};
		template<typename T, typename U, typename... Tail>
		struct common_iterator_reference_impl<T, U, Tail...>
			: public sprout::detail::common_iterator_reference_impl<
				typename sprout::detail::common_iterator_reference_impl<T, U>::type,
				Tail...
			>
		{};
	}	// namespace detail
	//
	// common_iterator_reference
	//
	template<typename... Iterators>
	struct common_iterator_reference
		: public sprout::detail::common_iterator_reference_impl<
			typename std::iterator_traits<Iterators>::reference...
		>
	{};

	//
	// common_iterator_value_type
	//
	template<typename... Iterators>
	struct common_iterator_value_type
		: public std::decay<
			typename sprout::common_iterator_reference<Iterators...>::type
		>
	{};

	//
	// common_iterator_pointer
	//
	template<typename... Iterators>
	struct common_iterator_pointer
		: public std::add_pointer<
			typename std::remove_reference<typename sprout::common_iterator_reference<Iterators...>::type>::type
		>
	{};

	//
	// common_iterator_difference_type
	//
	template<typename... Iterators>
	struct common_iterator_difference_type
		: public std::common_type<
			typename std::iterator_traits<Iterators>::difference_type...
		>
	{};
}	// namespace sprout

#endif	// #ifndef SPROUT_ITERATOR_TRAITS_HPP
