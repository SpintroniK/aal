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
		aal::buffer buf{"/home/jeremy/Desktop/1.wav"};
		//aal::buffer buf2{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/SnareDrum/snare1.raw"};
		//std::vector<aal::buffer> vbuf(64, buf2);

		//std::vector<short> wav_data = aal::load_wav_from_disk("/home/jeremy/Desktop/1.wav");

		aal::device device;

		aal::voice v{device.play_sound(buf)};
		v.add_effect<aal::Amplify>();


		//aal::voice v2{device.play_sound(buf2)};

		size_t i = 0;
		while(v.is_playing())// && i < vbuf.size())
		{
			sleep_for(1s);
			//device.play_sound(vbuf[i]);
			std::cout << "Playing..." << std::endl;
			i++;
		}
//	}
//	catch(const std::exception& e)
//	{
//		std::cerr << e.what() << '\n';
//	}

	return 0;
}


