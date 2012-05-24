#ifndef _NNET_ACTIVATOR_HPP_INCLUDED_
#define _NNET_ACTIVATOR_HPP_INCLUDED_

#include <cmath>

namespace vision {

template<typename Number>
struct Sigmoid {
	static Number activate(Number input) {
		Number tmp = std::exp(-input);
		return 1 / (tmp + 1);
	}

	static Number derivative(Number input) {
		Number tmp = activate(input);
		return tmp * (1 - tmp);
	}
};

} // namespace vision

#endif // _NNET_ACTIVATOR_HPP_INCLUDED_
