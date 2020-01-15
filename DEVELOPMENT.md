# Development

## Compiling
For compiling the software please make sure that all required libraries are installed. Furthermore cmake and a C++ compiler with support for C++17 is needed:
````bash
mkdir build
cmake ..
make -j4
````

## Unit tests
Unit tests may be executed by using the run_tests binary:
````bash
./run_tests
````

## Libraries
The following libraries are required for compiling.

### Boost
The following Boost components with version >= 1.62 needs to be available:
* system
* filesystem

### CAN Utils
CAN bus inverter communication dependes on [linux-can/can-utils](https://github.com/linux-can/can-utils):
````bash
mkdir -p includes && cd includes
git clone https://github.com/linux-can/can-utils
mkdir -p can-utils/build
cd can-utils/build
cmake .. && make && sudo make install && sudo ldconfig
````
 
### WebsocketPP
For browser communication a websocket connection is used depending on [zaphoyd/websocketpp](https://github.com/zaphoyd/websocketpp):
````bash
mkdir -p includes && cd includes
git clone https://github.com/zaphoyd/websocketpp
mkdir -p websocketpp/build
cd websocketpp/build
cmake .. && make && sudo make install && sudo ldconfig
````

### JSON
UI and brake input data is JSON encoded, encoding and decoding is done by using the single-header [JSON library of Nlohmann](https://github.com/nlohmann/json):
````bash
mkdir -p includes/nnlohmann_json && cd includes/nlohmann_json
wget https://github.com/nlohmann/json/releases/download/v3.7.3/json.hpp
````

### Testing and mocking
Unit testing is done by using the [Catch2 framework](https://github.com/catchorg/Catch2) in association of the [Fake It library](https://github.com/eranpeer/FakeIt) used for mocking:
````bash
mkdir -p includes/catch2 && cd includes/catch2
wget https://github.com/catchorg/Catch2/releases/download/v2.10.2/catch.hpp

cd ..
git clone https://github.com/eranpeer/FakeIt
cd fake_it/single_header/catch/
ln -s ../../../catch2/catch.hpp catch.hpp
````


