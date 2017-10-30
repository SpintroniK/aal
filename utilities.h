/*
 * utilities.h
 *
 *  Created on: 30 Oct 2017
 *      Author: jeremy
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <vector>
#include <fstream>
#include <string>
#include <cstdint>

namespace aal
{

	enum class Endianness
	{
		LittleEndian,
		BugEndian
	};

	int32_t bytes_to_int32(std::vector<uint8_t>& source, Endianness endianness)
	{
		int32_t result;

		if(endianness == Endianness::LittleEndian)
		{
			result = (source[3] << 24) | (source[2] << 16) | (source[1] << 8) | source[0];
		}
		else
		{
			result = (source[0] << 24) | (source[1] << 16) | (source[2] << 8) | source[3];
		}

		return result;
	}

	int16_t bytes_to_int16(std::vector<uint8_t>& source, Endianness endianness)
	{
	    int16_t result;

	    if(endianness == Endianness::LittleEndian)
	    {
	        result = (source[1] << 8) | source[0];
	    }
	    else
	    {
	        result = (source[0] << 8) | source[1];
	    }

	    return result;
	}

	static std::vector<short> load_wav_from_disk(const std::string& file_location)
	{

		// Open file
		std::ifstream file(file_location);

		// Check file validity
		if(!file.good())
		{
			throw std::runtime_error("Couldn't load file.");
		}

		// HEADER
		uint32_t data_size = 0;
		{
			std::vector<uint8_t> header_data(44);

			file.read((char*)header_data.data(), header_data.size());

			std::string chunk_id(header_data.begin(), header_data.begin() + 4);
			std::string format(header_data.begin() + 8, header_data.begin() + 12);

//			std::vector<uint8_t> num_channels_data(header_data.begin() + 22, header_data.begin() + 24);
//			uint16_t num_channels = (uint16_t) bytes_to_int16(num_channels_data, Endianness::LittleEndian);
//
//			std::vector<uint8_t> sample_rate_data(header_data.begin() + 24, header_data.begin() + 28);
//			uint32_t sample_rate = (uint32_t) bytes_to_int32(sample_rate_data, Endianness::LittleEndian);

			std::vector<uint8_t> data_data_size(header_data.begin() + 40, header_data.begin() + 44);
			data_size = (uint32_t) bytes_to_int32(data_data_size, Endianness::LittleEndian);


		}

		// DATA
		uint32_t data_size_short = data_size / sizeof(short);
		std::vector<short> data(data_size_short);

		file.read((char*)data.data(), data.size());

		return data;
	}

}


#endif /* UTILITIES_H_ */
