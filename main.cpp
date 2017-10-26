/*
 * main.cpp
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#include "audio.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::literals::chrono_literals;


int main(int argc, char* argv[])
{

//	try
//	{
		aal::buffer buf{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/HiHat/hihat.raw"};
		aal::buffer buf2{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/SnareDrum/snare1.raw"};
		std::vector<aal::buffer> vbuf(64, buf2);

		aal::device device;

		aal::voice v{device.play_sound(buf)};
		aal::voice v2{device.play_sound(buf2)};

		size_t i = 0;
		while(v.is_playing() && i < vbuf.size())
		{
			std::this_thread::sleep_for(200ms);
			device.play_sound(vbuf[i]);
			i++;
		}
//	}
//	catch(const std::exception& e)
//	{
//		std::cerr << e.what() << '\n';
//	}

	return 0;
}


