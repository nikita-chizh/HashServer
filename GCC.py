import os
gcc = [ 'add-apt-repository ppa:ubuntu-toolchain-r/test'
        , 'apt-get update'
        , 'sudo apt-get install gcc-8 g++-8'
        , 'update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8']


try:
    for cmd in gcc:
        os.system(cmd)
except Exception as err:
    print(err)