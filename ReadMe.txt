Chizhikov Nikita's libev-based hash-server

1. Dependencies:
    1. libev (brew install libev / apt-get install libev)
    2. nlohmann-json (brew install nlohmann-json / apt-get install nlohmann-json)
    3. cryptopp (brew install cryptopp / apt-get install cryptopp)
    4. gtest

2. How to start:
    1. HashServer <path_to_config>
    2. cd tests && bash test.sh

3. Config Params
    1. port: listening port
    2. hashFunction: (only sha256 and sha512 are supported).
    3. numberOfAcceptors: (number of worker threads).

4. TODOS
    1. Caching (HashServer should be able to cache already computated hashes).
    2. Thread pull for async hash computation.
