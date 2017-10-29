/*
 * Source.h
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#ifndef SOURCE_H_
#define SOURCE_H_


#include "effect.h"
#include <string>
#include <vector>
#include <atomic>
#include <algorithm>

namespace aal
{

	class device;
	class voice;

	class source
	{

		friend class device;
		friend class voice;

	public:

		source() : index{0}, playing{true}, is_loop{false} {}
		virtual ~source()
		{
			// Delete all the effects
			std::for_each(effects.begin(), effects.end(), [](auto e) { delete e; });
		}


		virtual const short* get_chunk(size_t& length) const noexcept
		{

			const auto data_length = data.size();
			auto old_index = index.fetch_add(length, std::memory_order_release);

			// Truncate if length is greater than the buffer's size
			if(old_index + length >= data_length)
			{
				length = data_length - old_index;

				if(is_loop.load(std::memory_order_acquire))
				{
					index.store(0, std::memory_order_release);
				}
				else
				{
					index.store(length, std::memory_order_release);
					playing.store(false, std::memory_order_release);
				}
			}

			return &data[old_index];
		}

		virtual bool is_playing() const noexcept { return playing.load(std::memory_order_acquire); }
		virtual bool is_circular() const noexcept { return is_loop.load(std::memory_order_acquire); }
		virtual void add_effect(effect* e) { effects.push_back(e); }

	protected:

		virtual void play() noexcept
		{
			index.store(0, std::memory_order_release);
			playing.store(true, std::memory_order_release);
		}

		virtual void stop() noexcept { playing.store(false, std::memory_order_release);}

		mutable std::atomic<size_t> index;
		mutable std::atomic<bool> playing;
		std::atomic<bool> is_loop;
		std::vector<short> data;
		std::vector<effect*> effects;

	};



}


#endif /* SOURCE_H_ */
