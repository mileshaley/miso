/*~-------------------------------------------------------------------------~*\
 * miso - Modern C++ Utilities                                               *
 *~-------------------------------------------------------------------------~*
 * > File: types.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include <utility> // std::tuple
#include <type_traits>

namespace miso {
	// Represents a NULL type
	// For example: The result of geting first argument of pack<> (empty)
	struct no_type {
		using type = no_type;
	};

	// Forward declare pack
	template<typename... Ts>
	struct pack;

	// Primary template
	// Also handles 0 Ts case
	template<typename... Ts>
	struct first_arg {
		using get = void; // Doesn't exist
		using remove = pack<>;
	};

	// Isolates first type argument for getting or removing
	template<typename First, typename... Rest>
	struct first_arg<First, Rest...> {       
		using get = First;
		using remove = pack<Rest...>;
	};

	namespace detail {
		// Primary template
		template<class NewPack, class OriginalPack, typename... Rest>
		struct remove_ts_aux;

		// Handles 0 arguments left (base case)
		template<typename... NPAs, typename... Rest>
		struct remove_ts_aux<pack<NPAs...>, pack<>, Rest...> {
			using type = pack<NPAs...>;
		};

		template<typename... NPAs, typename... OPAs, typename... Rest>
		struct remove_ts_aux<pack<NPAs...>, pack<OPAs...>, Rest...> {
			using first_og = typename first_arg<OPAs...>::get;
			using rest_og  = typename first_arg<OPAs...>::remove;
			using type     = std::conditional_t<
				(std::_Is_any_of_v<first_og, Rest...>),
				typename remove_ts_aux<pack<NPAs...			 >, 
									   rest_og, Rest...>::type,
				typename remove_ts_aux<pack<NPAs..., first_og>, 
									   rest_og, Rest...>::type
			>;
		};

	} // namespace detail
	
	// Helper to remove all types RemoveTs from a pack
	template<class Pack, typename... RemoveTs>
	using remove_types_t = detail::remove_ts_aux<
		pack<>, Pack, RemoveTs...>::type;

	// miso::pack is a type that can be used like a container of type objects.
	// pack is used all over this file as a "return type" of multiple types
	//   or as an additional template parameter pack
	template<typename... Ts>
	struct pack {
		using as_tuple = std::tuple<Ts...>;

		template<typename... NewTs>
		using add = pack<Ts..., NewTs...>;

		using first = first_arg<Ts...>;

		template<typename... RemoveTs>
		using remove = remove_types_t<pack<Ts...>, RemoveTs...>;
	};

} // namespace miso
