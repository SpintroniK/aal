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

		voice(const source& s) : src(s)
		{

		}
		~voice(){}

		bool is_playing() { return !src.is_over(); }

	private:

		const source& src;

	};


}



#endif /* VOICE_H_ */
