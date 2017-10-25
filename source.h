/*
 * Source.h
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#ifndef SOURCE_H_
#define SOURCE_H_

#include <fstream>
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

	class buffer : public source
	{

	public:

		buffer(const std::string& file_location)
		{
			load_from_disk(file_location);
		}

		virtual ~buffer(){}

	private:

		bool load_from_disk(const std::string& fileLocation)
		{

			// Open file
			std::ifstream soundFile(fileLocation);

			// Check file validity
			if(!soundFile.good())
			{
				throw std::runtime_error("Couldn't load file.");
			}

			soundFile.seekg (0, std::ios::end);

			// Get file size in bytes
			int fileSize = soundFile.tellg();


			soundFile.seekg(0, std::ios::beg);

			data.resize(fileSize);

			soundFile.read((char*)data.data(), sizeof(short) * fileSize);

			return true;
		}


	};

}


#endif /* SOURCE_H_ */
