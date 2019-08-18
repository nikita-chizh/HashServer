#include <vector>
#include <gtest/gtest.h> // googletest header file
#include "../HashProtocol.h"
const int clientSock = 1;
const size_t bsize = 1024;
char buf[bsize] = "HELLO TCP SERVER\n";
const ssize_t dataSize = 17;


TEST(ProcessTesting, clientNotExist) {
    HashProtocol hashProtocol;
    try {
        auto [status, processedData] = hashProtocol.processChunck(clientSock, buf, dataSize);
    }
    catch (const std::runtime_error &error){
        ASSERT_STREQ("socket fd==1 is not exist", error.what());
    }
}

TEST(ProcessTesting, testOnePacket) {
    HashProtocol hashProtocol;
    hashProtocol.acceptClient(clientSock);
    auto [status, processedData] = hashProtocol.processChunck(clientSock, buf, dataSize);
    ASSERT_EQ(status, PROCESS_STATUS::FULL_IN_ONE);
    ASSERT_EQ(processedData, std::vector<char>());
}

TEST(ProcessTesting, testMultiplePackets) {
    HashProtocol hashProtocol;
    hashProtocol.acceptClient(clientSock);
    // First message
    const size_t fsize = 1024;
    char fbuf[fsize] = "HELLO ";
    ssize_t fdataSize = 6;
    auto [status, processedData] = hashProtocol.processChunck(clientSock, fbuf, fdataSize);
    ASSERT_EQ(status, PROCESS_STATUS::NOT_FOUND);
    ASSERT_EQ(processedData, std::vector<char>());
    //
    auto [status1, processedData1] = hashProtocol.processChunck(clientSock, buf, dataSize);
    ASSERT_EQ(status1, PROCESS_STATUS::FOUND_IN_MANY);
    std::vector<char> expectedRes = {'H','E','L','L','O',' ',
                                     'H','E','L','L','O',' ',
                                     'T','C','P',' ',
                                     'S','E','R','V','E','R'};
    ASSERT_EQ(processedData1, expectedRes);
}




