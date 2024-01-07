#pragma once

namespace Behemoth
{
	template <typename ... T>
	struct variadic_typedef
	{

	};

	template<typename ...T>
	struct ConvertTuple
	{
		typedef std::tuple<T...>type;
	};

	template<typename ...T>
	struct ConvertTuple<variadic_typedef<T...>>
	{
		typedef typename ConvertTuple<T...>::type type;
	};


}