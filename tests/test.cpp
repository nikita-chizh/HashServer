#include <vector>
#include <gtest/gtest.h> // googletest header file
#include "../HashProtocol.h"

const int clientSock = 1;
std::vector<char> buf = {'H','E','L','L','O',' ',
                         'T','C','P',' ',
                         'S','E','R','V','E','R', '\n'};

std::string hashName = "sha256";

TEST(ProcessTesting, clientNotExist) {
    HashProtocol hashProtocol(hashName);
    try {
        auto [status, processedData] = hashProtocol.processChunck(clientSock, buf.data(), buf.size());
    }
    catch (const std::runtime_error &error){
        ASSERT_STREQ("ERROR processChunck fd=1 doesn't exist", error.what());
    }
}

TEST(ProcessTesting, testOnePacket) {
    HashProtocol hashProtocol(hashName);
    hashProtocol.acceptClient(clientSock);
    auto [status, processedData] = hashProtocol.processChunck(clientSock, buf.data(), buf.size());
    ASSERT_EQ(status, PROCESS_STATUS::FULL_IN_ONE);
    ASSERT_EQ(processedData, std::vector<char>());
}

TEST(ProcessTesting, testMultiplePackets) {
    HashProtocol hashProtocol(hashName);
    hashProtocol.acceptClient(clientSock);
    // First message
    std::vector<char> fbuf = {'F', 'H','E','L','L','O',' '};
    auto [status, processedData] = hashProtocol.processChunck(clientSock, fbuf.data(), fbuf.size());
    ASSERT_EQ(status, PROCESS_STATUS::NOT_FOUND);
    ASSERT_EQ(processedData, std::vector<char>());
    //
    auto [status1, processedData1] = hashProtocol.processChunck(clientSock, buf.data(), buf.size());
    ASSERT_EQ(status1, PROCESS_STATUS::FOUND_IN_MANY);
    std::vector<char> expectedRes = {'F','H','E','L','L','O',' ',
                                     'H','E','L','L','O',' ',
                                     'T','C','P',' ',
                                     'S','E','R','V','E','R', '\n'};
    ASSERT_EQ(processedData1, expectedRes);
}

class HASH_TEST : public HashProtocol, public testing::Test
{

};

TEST_F(HASH_TEST, SHA256){
    setCryptoFunc(SHA256);
    std::vector<char> buf = {'F', 'H','E','L','L','O',' ', '\n'};
    std::vector<char> expbuf = {'F', 'H','E','L','L','O',' '};
    auto res = _cryptoFunc(buf.data(), buf.size() - 1);
    auto expect = SHA256(expbuf.data(), expbuf.size());
    ASSERT_NE(expect.size(), 0);
    ASSERT_NE(res.size(), 0);
    ASSERT_EQ(res, expect);
}

