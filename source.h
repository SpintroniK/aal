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

	class source
	{

		friend class device;

	public:

		source() : index{0}, playing{true} {}
		virtual ~source(){}


		virtual const short* get_chunk(size_t& length) const noexcept
		{

			const auto data_length = data.size();
			const auto old_index = index;

			// Truncate if length is greater than the buffer's size
			if(index + length > data_length)
			{
				length = std::min(data_length, data_length - index);
				playing.store(false, std::memory_order_release);
			}

			index = old_index + length;
			return &data[index];
		}

		virtual bool is_over() const { return !playing.load(std::memory_order_acquire); }

	protected:

		mutable size_t index;
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
