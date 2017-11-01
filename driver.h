/*
 * driver.h
 *
 *  Created on: 25 Oct 2017
 *      Author: jeremy
 */

#ifndef DRIVER_H_
#define DRIVER_H_

#include "alsa/asoundlib.h"

#include <thread>
#include <chrono>

namespace aal
{

	class driver
	{

	public:

		driver()
		{
			int err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

			if(err >= 0)
			{
				snd_pcm_hw_params_alloca(&hw_parameters);
				set_hw_parameters();
			}
			else
			{
				throw std::runtime_error("Couldn't open device.");
			}

		}

		~driver()
		{
			snd_pcm_drain(handle);
			snd_pcm_close(handle);
		}

		size_t period_size() const noexcept { return _period_size; }

		template <typename T>
		int write(T data_ptr, size_t frames) const noexcept
		{
			int err = snd_pcm_writei(handle, data_ptr, frames);

			if(err < 0)
			{
				xrun_recovery(err);
			}

			return err;
		}

		size_t get_num_channels() const noexcept { return num_channels; }
		size_t get_sample_rate() const noexcept { return _sample_rate; }

	private:


		int set_hw_parameters()
		{

			unsigned int sample_rate = static_cast<unsigned int>(_sample_rate);
			snd_pcm_uframes_t size;
			int dir;

			snd_pcm_hw_params_any(handle, hw_parameters);
			snd_pcm_hw_params_set_rate_resample(handle, hw_parameters, 1);
			snd_pcm_hw_params_set_access(handle, hw_parameters, SND_PCM_ACCESS_RW_INTERLEAVED);
			snd_pcm_hw_params_set_format(handle, hw_parameters, SND_PCM_FORMAT_S16_LE);
			snd_pcm_hw_params_set_channels(handle, hw_parameters, num_channels);

			snd_pcm_hw_params_set_rate_near(handle, hw_parameters, &sample_rate, 0);
			_sample_rate = static_cast<size_t>(sample_rate);

			snd_pcm_hw_params_set_buffer_time_near(handle, hw_parameters, &buffer_time, &dir);

			snd_pcm_hw_params_get_buffer_size(hw_parameters, &size);

			_buffer_size = static_cast<size_t>(size);

			snd_pcm_hw_params_set_period_time_near(handle, hw_parameters, &period_time, &dir);
			snd_pcm_hw_params_get_period_size(hw_parameters, &size, &dir);

			_period_size = static_cast<size_t>(size);

			snd_pcm_hw_params(handle, hw_parameters);


			return 0;
		}

		void xrun_recovery(int& err) const noexcept
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


		snd_pcm_t* handle;
		snd_pcm_hw_params_t* hw_parameters;

		unsigned int buffer_time = 80000;
		unsigned int period_time = 40000;
		size_t _buffer_size;
		size_t _period_size;

		size_t num_channels = 2;
		size_t _sample_rate = 44100;
	};

}



#endif /* DRIVER_H_ */
