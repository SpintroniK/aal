/*
 * Source.h
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#ifndef SOURCE_H_
#define SOURCE_H_


#include <string>
#include <vector>
#include <atomic>

namespace aal
{

	class device;
	class voice;

	class source
	{

		friend class device;
		friend class voice;

	public:

		source() : index{0}, playing{true} {}
		virtual ~source(){}


		virtual const short* get_chunk(size_t& length) const noexcept
		{

			const auto data_length = data.size();
			auto old_index = index.fetch_add(length, std::memory_order_release);

			// Truncate if length is greater than the buffer's size
			if(old_index + length > data_length)
			{
				length = data_length - old_index;
				index.store(data_length, std::memory_order_release);
				playing.store(false, std::memory_order_release);
			}

			return &data[old_index];
		}

		virtual bool is_playing() const noexcept { return playing.load(std::memory_order_acquire); }

	protected:

		virtual void play() noexcept { playing.store(true, std::memory_order_release); }
		virtual void stop() noexcept { playing.store(false, std::memory_order_release);}

		mutable std::atomic<size_t> index;
		mutable std::atomic<bool> playing;
		std::vector<short> data;

	};



}


#endif /* SOURCE_H_ */
