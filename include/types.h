/*~-------------------------------------------------------------------------~*\
 * miso - Modern C++ Utilities                                               *
 *~-------------------------------------------------------------------------~*
 * > File: types.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include <utility> // std::tuple
#include <type_traits>

namespace miso {

/*~-------------------------------------------------------------------------~*\
 * Type Declarations                                                         *
\*~-------------------------------------------------------------------------~*/

	// Represents a nonexistent/null type
	// For example: The result of geting first argument of pack<> (empty)
	struct no_type {
		using type = no_type;
	};

	// Forward declare pack
	template<typename... Ts>
	struct pack;

/*~-------------------------------------------------------------------------~*\
 * Variadic Type Access                                                      *
\*~-------------------------------------------------------------------------~*/

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

/*~-------------------------------------------------------------------------~*\
 * Variadic Type Removal                                                     *
\*~-------------------------------------------------------------------------~*/

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
			using rest_og = typename first_arg<OPAs...>::remove;
			// If the type in the original pack is the same as any remove type,
			// don't add it to the pack, otherwise add it. Then recurse
			using type = std::conditional_t<
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

/*~-------------------------------------------------------------------------~*\
 * Variadic Type Storage                                                     *
\*~-------------------------------------------------------------------------~*/

	// Type that can be used like a container of type objects.
	// pack is used all over this file as a "return type" of multiple types
	//   or as an additional variadic template parameter pack
	template<typename... Ts>
	struct pack {
		using as_tuple = std::tuple<Ts...>;

		template<typename... NewTs>
		using add = pack<Ts..., NewTs...>;

		using first = first_arg<Ts...>;

		template<typename... RemoveTs>
		using remove = remove_types_t<pack<Ts...>, RemoveTs...>;
	};

/*~-------------------------------------------------------------------------~*\
 * Variadic Type Normalization                                               *
\*~-------------------------------------------------------------------------~*/

	namespace detail {
		// Primary template
		template<class Pack, typename First, typename... Rest>
		struct flatten;

		// Primary template
		template<class Pack, typename... Rest>
		struct reflatten;

		// Last arg is non-tuple (guaranteed endpoint)
		template<class Pack, typename First>
		struct flatten<Pack, First> {
			using type = typename Pack::template add<First>;
		};
		// Last arg is tuple (requires reflatten)
		template<class Pack, typename... FirstArgs>
		struct flatten<Pack, std::tuple<FirstArgs...>> {
			using type = typename reflatten<typename Pack::template add<FirstArgs...>>::type;
		};
		// Arg is non-tuple
		template<class Pack, typename First, typename... Rest>
		struct flatten {
			using type = typename flatten<typename Pack::template add<First>, Rest...>::type;
		};
		// Arg is tuple (requires reflatten)
		template<class Pack, typename... FirstArgs, typename... Rest>
		struct flatten<Pack, std::tuple<FirstArgs...>, Rest...> {
			using type = typename reflatten<typename Pack::template add<FirstArgs...>, Rest...>::type;
		};
		// Reflatten all args to get all nested tuples at any depth
		template<typename... PackArgs, typename... Rest>
		struct reflatten<pack<PackArgs...>, Rest...> {
			using type = typename flatten<pack<>, PackArgs..., Rest...>::type;
		};
	} // namespace detail

	// Helper template that combines any number of
	// types or tuples (nested to any depth) into a pack
	template<typename... Args>
	using flatten_t = typename detail::flatten<pack<>, Args...>::type;
	// Helper template that combines any number of
	// types or tuples (nested to any depth) into another tuple
	template<typename... Args>
	using flatten_into_tuple_t = typename flatten_t<Args...>::as_tuple;

/*~-------------------------------------------------------------------------~*\
 *                                                                           *
\*~-------------------------------------------------------------------------~*/

} // namespace miso
