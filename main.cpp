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



	aal::buffer buf{"/home/jeremy/Desktop/2.wav"};
	aal::buffer buf2{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/SnareDrum/snare1.wav"};
	std::vector<aal::buffer> vbuf(64, buf2);

	aal::device device;

	aal::voice v{device.play_sound(buf)};
	v.add_effect<aal::Volume>(0.5);

	size_t i = 0;
	while(v.is_playing() && i < vbuf.size())
	{
		sleep_for(1s);
		device.play_sound(vbuf[i]);
		std::cout << "Playing..." << std::endl;
		i++;
	}

	std::cout << "Finished." << std::endl;


	return 0;
}


