/*
 * Device.h
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "voice.h"
#include "alsa/asoundlib.h"

#include <thread>
#include <algorithm>
#include <atomic>
#include <iostream>
#include <chrono>
#include "source.h"

namespace aal
{

	class device
	{

	public:

		device() : run_thread(true)
		{

			int err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

			if(err >= 0)
			{
				snd_pcm_hw_params_alloca(&hwParams);
				SetHwParams();
			}
			else
			{
				throw -1;
			}

			device_thread = std::thread(&device::thread, this);

			// Set maximum priority to the thread
	        sched_param sch_params;
	        sch_params.sched_priority = sched_get_priority_max(SCHED_FIFO);

	        pthread_setschedparam(device_thread.native_handle(), SCHED_FIFO, &sch_params);


		}

		~device()
		{
			run_thread.store(false);
			device_thread.join();
			snd_pcm_drain(handle);
			snd_pcm_close(handle);
		}

		voice play_sound(source& source)
		{
			sources.push_back(&source);
			return voice{source};
		}

	private:

		void thread() noexcept
		{
			while(run_thread.load())
			{

				int frames = period_length;

				while(frames > 0)
				{

					std::fill(internal_buffer.begin(), internal_buffer.end(), 0);
					for(const auto& s : sources)
					{
						for(size_t i = 0; i < period_length; ++i)
						{
							internal_buffer[i] += s->get_chunk(i);
						}
						s->update_index(period_length);
					}

					int err = snd_pcm_writei(handle, internal_buffer.data(), frames);

					if (err == -EAGAIN)
					{
						continue;
					}

					if (err < 0)
					{
						//XrunRecovery(err);
					}

					frames -= err;

				}
			}
		}

		int SetHwParams()
		{

			unsigned int realRate;
			snd_pcm_uframes_t size;
			int dir;

			snd_pcm_hw_params_any(handle, hwParams);
			snd_pcm_hw_params_set_rate_resample(handle, hwParams, 1);
			snd_pcm_hw_params_set_access(handle, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED);
			snd_pcm_hw_params_set_format(handle, hwParams, SND_PCM_FORMAT_S16_LE);
			snd_pcm_hw_params_set_channels(handle, hwParams, 1);

			realRate = 48000;

			snd_pcm_hw_params_set_rate_near(handle, hwParams, &realRate, 0);

			snd_pcm_hw_params_set_buffer_time_near(handle, hwParams, &buffer_length, &dir);

			snd_pcm_hw_params_get_buffer_size(hwParams, &size);

			buffer_length = size;

			unsigned int periodTime;
			snd_pcm_hw_params_set_period_time_near(handle, hwParams, &periodTime, &dir);
			snd_pcm_hw_params_get_period_size(hwParams, &size, &dir);

			period_length = size;
			internal_buffer.resize(size);

			snd_pcm_hw_params(handle, hwParams);


			return 0;
		}

		void XrunRecovery(int& err)
		{
			using namespace std::literals::chrono_literals;

			if (err == -EPIPE)
			{
				err = snd_pcm_prepare(handle);
			}
			else if (err == -ESTRPIPE)
			{

				while ((err = snd_pcm_resume(handle)) == -EAGAIN)
				{
					std::this_thread::sleep_for(1ms);
				}

				if (err < 0)
				{
					err = snd_pcm_prepare(handle);
				}
			}

			return;
		}

		std::atomic<bool> run_thread;
		std::thread device_thread;
		std::vector<short> internal_buffer;

		std::vector<source*> sources;

		// Low level
		snd_pcm_t* handle;
		snd_pcm_hw_params_t* hwParams;
		unsigned int buffer_length = 5000;
		unsigned int period_length = 2500;

	};

}

#endif /* DEVICE_H_ */