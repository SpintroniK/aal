/*
 * main.cpp
 *
 *  Created on: 21 Oct 2017
 *      Author: jeremy
 */

#include "Aal.h"
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;


int main(int argc, char* argv[])
{

	aal::device device;

	aal::buffer buf{"/home/jeremy/Desktop/Prog/eXaDrums/eXaDrums/Data/SoundBank/HiHat/hihat.raw"};

	aal::voice v{device.play_sound(buf)};

	while(v.is_playing())
	{
		std::this_thread::sleep_for(100ms);
	}


	return 0;
}


