/*~-------------------------------------------------------------------------~*\
 * miso - Modern C++ Utilities                                               *
 *~-------------------------------------------------------------------------~*
 * > File: types.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include <utility> // std::tuple

namespace miso {
	// Forward declare pack
	template<typename... Ts>
	struct pack;

	//namespace detail {
	//	// Separating this logic allows us to 
	//	template<typename First, typename...>
	//	using first_type_arg_get_aux = First;
	//	template<typename, typename... Rest>
	//	using first_type_arg_remove_aux = pack<Rest...>;
	//
	//} // namespace detail

	// Primary template
	template<typename... Ts>
	struct first_arg {
		using get = void; // Doesn't exist
		using remove = pack<>;
	};

	template<typename First, typename... Rest>
	struct first_arg<First, Rest...> {       
		using get = First;
		using remove = pack<Rest...>;
	};

	// miso::pack is a type that can be used like a container of type objects
	template<typename... Ts>
	struct pack {
		using as_tuple = std::tuple<Ts...>;

		template<typename... NewTs>
		using add = pack<Ts..., NewTs...>;
	};



} // namespace miso
