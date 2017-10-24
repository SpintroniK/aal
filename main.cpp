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
		aal::device device;

		aal::buffer buf{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/HiHat/hihat.raw"};
		aal::buffer buf2{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/SnareDrum/SD_TB.raw"};

		aal::voice v{device.play_sound(buf)};
		aal::voice v2{device.play_sound(buf2)};

		while(v.is_playing())
		{
			std::this_thread::sleep_for(100ms);
		}
//	}
//	catch(const std::exception& e)
//	{
//		std::cerr << e.what() << '\n';
//	}

	return 0;
}


