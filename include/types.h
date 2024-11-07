/*~-------------------------------------------------------------------------~*\
 * miso - Modern C++ Utilities                                               *
 *~-------------------------------------------------------------------------~*
 * > File: types.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

#include <utility> // std::tuple

namespace miso {
	template<typename... Ts>
	struct pack {
		using as_tuple_t = std::tuple<Ts...>;

		template<typename... NewTs>
		using add = pack<Ts..., NewTs...>;
	};

} // namespace miso
