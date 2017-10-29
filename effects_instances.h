/*
 * effects_instances.h
 *
 *  Created on: 28 Oct 2017
 *      Author: jeremy
 */

#ifndef EFFECTS_INSTANCES_H_
#define EFFECTS_INSTANCES_H_

#include "effect.h"

namespace aal
{

	class Amplify : public effect
	{

	public:

		Amplify(){}
		virtual ~Amplify(){}

		virtual void process(const short* buffer_in, short* buffer_out, size_t length_samples) final
		{
			for(size_t i = 0; i < length_samples; ++i)
			{
				buffer_out[i] = buffer_in[i] * 2;
			}
		}

	};

}



#endif /* EFFECTS_INSTANCES_H_ */
