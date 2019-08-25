Chizhikov Nikita's libev-based hash-server

Build:
To build you need:
1. GCC version 8.3 (C++17 support) 
sudo python3 GCC.py
2. cmake version > 3.8 (C++17 support)
sudo python3 Cmake.py
3. python3

Dependencies:
1. libev, libcrypto++, rapidjson, gtest
sudo python3 dependency_downloader.py

Build order:
1. bash build.sh

Start:
cd tests && bash startServer.sh (starts server)
bash test.sh (starts multiple instances of TestHashServer.py)

Config Params:
1. port: listening port
2. hashFunction: (only sha256 and sha512 are supported).
3. numberOfAcceptors: (number of worker _threads).
4. numberOfProcessors: (number of threads in ThreadPool).

