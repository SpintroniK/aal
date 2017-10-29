/*
 * effect.h
 *
 *  Created on: 28 Oct 2017
 *      Author: jeremy
 */

#ifndef EFFECT_H_
#define EFFECT_H_

#include <cstddef>

namespace aal
{

	class effect
	{

	public:

		effect() {}
		virtual ~effect() {}

		virtual void process(const short* buffer_in, short* buffer_out, size_t length_samples) = 0;

	private:



	};

}

#endif /* EFFECT_H_ */
