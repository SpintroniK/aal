/*
 * Device.h
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "driver.h"
#include "voice.h"
#include "source.h"
#include "buffer.h"
#include "doublebuf.h"

#include <thread>
#include <algorithm>
#include <atomic>

namespace aal
{

	class device
	{

	public:

		device() : run_thread(true), internal_buffer(audio_driver.period_size()), temp_buffer(internal_buffer.size()), sources(32)
		{

			std::for_each(sources.begin(), sources.end(), [](auto& source) { source.store(nullptr, std::memory_order_release); });

			device_thread = std::thread(&device::device_thread_loop, this);

			// Set maximum priority to the thread
	        sched_param sch_params;
	        sch_params.sched_priority = sched_get_priority_max(SCHED_FIFO);

	        pthread_setschedparam(device_thread.native_handle(), SCHED_FIFO, &sch_params);

		}

		~device()
		{
			run_thread.store(false, std::memory_order_release);
			device_thread.join();
		}


		voice play_sound(source& src)
		{

			src.play();

			for(size_t i = 0; i < sources.size(); ++i)
			{
				source* src_nullptr = nullptr;
				if(sources[i].compare_exchange_strong(src_nullptr, &src))
				{
					break;
				}
			}

			return voice{src};
		}

	private:

		void device_thread_loop() noexcept
		{

			while(run_thread.load(std::memory_order_acquire))
			{

				auto frames = audio_driver.period_size(); ///2;

				while(frames > 0)
				{

					std::fill(internal_buffer.begin(), internal_buffer.end(), 0);

					for(size_t i = 0; i < sources.size(); ++i)
					{

						source* src_ptr = nullptr;

						if(!sources[i].compare_exchange_strong(src_ptr, nullptr))
						{
							if(src_ptr->is_playing())
							{
								auto length = audio_driver.period_size();
								mix_buffer(src_ptr, length);

								if(length < audio_driver.period_size() && src_ptr->is_circular())
								{
									auto delta = frames - length;
									mix_buffer(src_ptr, delta, length);
								}
							}
							else
							{
								sources[i].store(nullptr, std::memory_order_release);
							}
						}
					}

					frames -= audio_driver.write(internal_buffer.data(), frames);
				}
			}
		}

		inline void mix_buffer(source* src_ptr, size_t& length, size_t offset = 0) noexcept
		{

			auto ptr_buf = src_ptr->get_chunk(length);

			if(!src_ptr->effects.empty()) // Probably not thread-safe.
			{
				// Apply effects
				for(auto e : src_ptr->effects)
				{
					e->process(ptr_buf, temp_buffer.data(), length);
				}

				for(size_t x = 0; x < length; ++x)
				{
					internal_buffer[x + offset] += temp_buffer[x];
				}
			}
			else
			{
				for(size_t x = 0; x < length; ++x)
				{
					internal_buffer[x + offset] += *(ptr_buf + x);
				}
			}

		}

		driver audio_driver;

		std::atomic<bool> run_thread;
		std::thread device_thread;

		std::vector<short> internal_buffer;
		std::vector<short> temp_buffer;
		std::vector<std::atomic<source*>> sources;

	};

}

#endif /* DEVICE_H_ */
