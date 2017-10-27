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
using namespace std::this_thread;


int main(int argc, char* argv[])
{

//	try
//	{
		//aal::buffer buf{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/HiHat/hihat.raw"};
		aal::doublebuf buf{"/home/jeremy/Desktop/1.raw"};
		aal::buffer buf2{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/SnareDrum/snare1.raw"};
		std::vector<aal::buffer> vbuf(64, buf2);

		aal::device device;

		aal::voice v{device.play_sound(buf)};
		aal::voice v2{device.play_sound(buf2)};

		size_t i = 0;
		while(v.is_playing() && i < vbuf.size())
		{
			sleep_for(1s);
			//device.play_sound(vbuf[i]);
			i++;
		}
//	}
//	catch(const std::exception& e)
//	{
//		std::cerr << e.what() << '\n';
//	}

	return 0;
}


