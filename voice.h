/*
 * Voice.h
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#ifndef VOICE_H_
#define VOICE_H_

#include "effect.h"
#include "effects_instances.h"
#include "source.h"
#include <algorithm>
#include <vector>

namespace aal
{

	class voice
	{
	public:

		voice(source& s) : src(&s) {}
		~voice() {}

		bool is_playing() const noexcept { return src->is_playing(); }
		void stop() const noexcept { src->stop(); }

		template <class T, class... Args>
		void add_effect(Args... args)
		{
			src->add_effect(new T(args...));
		}

	private:

		source* src;

	};


}



#endif /* VOICE_H_ */
