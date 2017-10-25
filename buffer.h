/*
 * buffer.h
 *
 *  Created on: 25 Oct 2017
 *      Author: jeremy
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "source.h"

#include <string>
#include <fstream>
#include <vector>

namespace aal
{

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


#endif /* BUFFER_H_ */
