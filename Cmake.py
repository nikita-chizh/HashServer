import os
cmake = [ 'wget https://cmake.org/files/v3.15/cmake-3.15.2.tar.gz'
        , 'tar -xzvf cmake-3.15.2.tar.gz'
        , 'cd cmake-3.15.2 && ./bootstrap && make -j4 && make install'
	, 'rm cmake-3.15.2.tar.gz'
	, 'rm -r cmake-3.15.2']
try:
    for cmd in cmake:
        os.system(cmd)
except Exception as err:
    print(err)
