/*
 * Voice.h
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#ifndef VOICE_H_
#define VOICE_H_

#include "source.h"

namespace aal
{

	class voice
	{
	public:

		voice(source& s) : src(s) {}
		~voice() {}

		bool is_playing() const noexcept { return src.is_playing(); }
		void stop() const noexcept { src.stop(); }

	private:

		source& src;

	};


}



#endif /* VOICE_H_ */
