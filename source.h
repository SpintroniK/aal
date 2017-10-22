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

		source() : index{0} {}
		virtual ~source(){}

		virtual void update_index(size_t increment) { index.fetch_add(increment); }
		virtual short get_chunk(size_t i) const { return (i+index.load()<data.size())? data[index + i]:0; }
		virtual bool is_over() const { return index.load() > data.size(); }

	protected:

		std::atomic<size_t> index;
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
				throw - 1;
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
