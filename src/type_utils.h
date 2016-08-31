#pragma once

// Two types meaning true and false
struct true_type {};
struct false_type {};

template <typename...TArgs>
struct TypeList {};

// Assume TElement is not in the list unless proven otherwise
template < typename TElement, typename TList >
struct ContainsType {
	typedef false_type type;
	static constexpr bool value = false;
};

// If it matches the first type, it is definitely in the list
template < typename TElement, typename... TTail >
struct ContainsType < TElement, TypeList < TElement, TTail... > >
{
	typedef true_type type;
	static constexpr bool value = true;
};

// If it is not the first element, check the remaining list
template < typename TElement, typename THead, typename... TTail >
struct ContainsType < TElement, TypeList < THead, TTail... > >
{
	typedef typename ContainsType < TElement, TypeList < TTail... > >::type type;
	static constexpr bool value = (ContainsType < TElement, TypeList < TTail... > >::value);
};

template < typename TEmptyList, typename TList >
struct ContainsAllTypes
{
	typedef true_type type;
	static constexpr bool value = true;
};

template < typename TElement, typename ...TElements, typename ...TTail >
struct ContainsAllTypes<TypeList<TElement, TElements...>, TypeList<TTail...>>
{
	typedef typename std::conditional<
		std::is_same<true_type, typename ContainsType<TElement, TypeList<TTail...> >::type>::value,
		typename ContainsAllTypes<TypeList<TElements...>, TypeList<TTail...> >::type,
		false_type
	>::type type;
	static constexpr bool value =
			(ContainsType<TElement, TypeList<TTail...> >::value)
		&& (ContainsAllTypes<TypeList<TElements...>, TypeList<TTail...> >::value);
};

template<typename T, typename TList> struct AddType;

template<typename R, typename ...TArgs> struct RemoveType;

template<typename T, typename ... TArgs>
struct AddType< T, TypeList<TArgs...> >
{
	typedef typename std::conditional<
		ContainsType<T, TypeList<TArgs...> >::value,
		TypeList<TArgs... >,
		TypeList<T, TArgs... >
	>::type type;
};

template<typename R>
struct RemoveType<R>
{
	typedef TypeList<> type;
};

template<typename R, typename T, typename ...TArgs>
struct RemoveType<R, T, TArgs...>
{
	typedef typename std::conditional
		< std::is_same<R, T>::value
		, typename RemoveType<R, TArgs...>::type
		, typename AddType<T, typename RemoveType<R, TArgs...>::type>::type
		>::type type;
};