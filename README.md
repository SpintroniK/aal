# aal
Audio Abstraction Library

Aal is an audio abstraction library for C++(14). 
It is a wrapper around Alsa and gives you a very easy way to use audio in C++.
The interface is based on Guy Somberg's talk at CppCon 2017, as described here: 
https://www.youtube.com/watch?v=M8Bd7uHH4Yg&t=32m27s 

## Table of content

- [Installation and Configuration](#installation)
    - [Dependencies](#dependencies)
    - [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Links](#links)


## Installation

### Dependencies

Aal depends on Alsa, which you can install as follows: 
```
sudo apt-get install libasound2-dev
```
After that, you can clone the repository: 
```
sudo apt-get install git
git clone https://github.com/SpintroniK/aal.git
```

### Installation

Aal is a collection of header files, so there's nothing to do but include audio.h in your cpp files.
Then, you can compile them like this: 
```
g++ -std=c++14 -o aal_example aal_example.cpp -lasound -lpthread
```
Note that you need to link against alsa and pthread.

## Usage

There is an example code in this repository, which you can find in the main.cpp file.
Here the most basic way to use the library: 
```C++
#include "audio.h"
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;
using namespace std::this_thread;

int main(int argc, char* argv[])
{

	aal::buffer buf{"/home/username/sound_file.wav"};

	aal::device device;

	aal::voice v{device.play_sound(buf)};
  
	while(v.is_playing())
	{
		sleep_for(100ms);
	}

	return 0;
}
```

## License

## Links
