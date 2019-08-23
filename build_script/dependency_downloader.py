import os
update = 'apt-get update'
libev = 'apt-get install libev-dev'
libcrypt = 'apt-get install libcrypto++8 libcrypto++8-dbg libcrypto++-dev'
json_lib = 'apt install rapidjson-dev'

cmd_list = [update, libev, libcrypt, json_lib]
try:
    for cmd in cmd_list:
        os.system(cmd)
except Exception as err:
    print(err)

gtest_buid= ['apt-get install libgtest-dev'
        , 'cd /usr/src/gtest && cmake CMakeLists.txt && make -j4 && cp *.a /usr/lib']
try:
    for cmd in gtest_buid:
        os.system(cmd)
except Exception as err:
    print(err)
