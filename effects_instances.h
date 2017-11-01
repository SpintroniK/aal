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

	class Volume : public effect
	{

	public:

		Volume(double val) : k(val){}
		virtual ~Volume(){}

		virtual void process(const short* buffer_in, short* buffer_out, size_t length_samples) const noexcept final
		{
			for(size_t i = 0; i < length_samples; ++i)
			{
				buffer_out[i] = static_cast<short>(static_cast<double>(buffer_in[i]) * k);
			}
		}

	private:

		double k;

	};

}



#endif /* EFFECTS_INSTANCES_H_ */
