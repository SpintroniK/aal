/*
 * doublebuf.h
 *
 *  Created on: 26 Oct 2017
 *      Author: jeremy
 */

#ifndef DOUBLEBUF_H_
#define DOUBLEBUF_H_

#include "source.h"

#include <string>
#include <fstream>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>

namespace aal
{

	class doublebuf : public source
	{

	public:

		explicit doublebuf(const std::string& file_location)
		: _file_location(file_location), buf_file{file_location}, is_read{true}
		{
			is_loop.store(true, std::memory_order_release);

			// Check file
			if(!buf_file.good())
			{
				throw std::runtime_error("Couldn't load file.");
			}

			buf_file.seekg(0, std::ios::end);

			// Get file size in bytes
			file_size = buf_file.tellg();

			buf_file.seekg(0, std::ios::beg);

			file_data.resize(segment_size);
			data.resize(segment_size * 2);

			// Read first segment
			copy_segment();

			// Start thread
			read_thread = std::thread(&doublebuf::thread_loop, this);
		}

		virtual ~doublebuf()
		{
			is_read.store(false, std::memory_order_release);
			read_thread.join();
		}

	private:

		void thread_loop()
		{
			using namespace std::literals::chrono_literals;
			using namespace std::this_thread;

			while(is_read.load(std::memory_order_acquire))
			{

				if(index.load(std::memory_order_acquire) <= segment_size)
				{
					if(segment_number % 2 != 0)
					{
						copy_segment();
					}
				}
				else
				{
					if(segment_number % 2 == 0)
					{
						copy_segment();
					}
				}

				sleep_for(100ms);
			}
		}

		void copy_segment()
		{

			buf_file.read((char*)file_data.data(), sizeof(short) * segment_size);
			data_index += segment_size;

			size_t k;
			if(segment_number % 2 == 0)
			{
				k = 0;
			}
			else
			{
				k = 1;
			}

			std::copy(file_data.begin(), file_data.end(), data.begin() + k * segment_size);
			segment_number++;
		}

		std::string _file_location;
		std::ifstream buf_file;

		size_t file_size;
		size_t segment_size = 256*1024;
		size_t segment_number = 0;
		size_t data_index = 0;
		std::vector<short> file_data;

		std::atomic<bool> is_read;
		std::thread read_thread;

	};

}


#endif /* DOUBLEBUF_H_ */
