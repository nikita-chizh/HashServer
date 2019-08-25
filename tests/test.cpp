#include <vector>
#include <gtest/gtest.h> // googletest header file
#include "../src/HashProtocol.h"

const int clientSock = 1;
std::vector<char> testMsg = {'H', 'E', 'L', 'L', 'O', ' ',
                             'T', 'C', 'P', ' ',
                             'S', 'E', 'R', 'V', 'E', 'R', '\n'};

std::string hashName = "sha256";
std::atomic<bool> stop {false};
ThreadPool threadPool(0, stop);

TEST(ProcessTesting, clientNotExist) {
    HashProtocol hashProtocol(hashName, threadPool);
    try {
        PROCESS_STATUS status;
        std::vector<char> processedData;
        std::tie(status, processedData) = hashProtocol.processChunk(clientSock, testMsg.data(), testMsg.size());
    }
    catch (const std::runtime_error &error){
        ASSERT_STREQ("ERROR processChunk fd=1 doesn't exist", error.what());
    }
}

TEST(ProcessTesting, testOnePacket) {
    HashProtocol hashProtocol(hashName, threadPool);
    hashProtocol.acceptClient(clientSock);
    PROCESS_STATUS status;
    std::vector<char> processedData;
    std::tie(status, processedData) = hashProtocol.processChunk(clientSock, testMsg.data(), testMsg.size());
    ASSERT_EQ(status, PROCESS_STATUS::FULL_IN_ONE);
    ASSERT_EQ(processedData, std::vector<char>());
}

TEST(ProcessTesting, testMultiplePackets) {
    HashProtocol hashProtocol(hashName, threadPool);
    hashProtocol.acceptClient(clientSock);
    // First message
    std::vector<char> firstMsg = {'F', 'H', 'E', 'L', 'L', 'O', ' '};
    PROCESS_STATUS status;
    std::vector<char> processedData;
    std::tie(status, processedData) = hashProtocol.processChunk(clientSock, firstMsg.data(), firstMsg.size());
    ASSERT_EQ(status, PROCESS_STATUS::NOT_FOUND);
    ASSERT_EQ(processedData, std::vector<char>());
    //
    std::tie(status, processedData) = hashProtocol.processChunk(clientSock, testMsg.data(), testMsg.size());
    ASSERT_EQ(status, PROCESS_STATUS::FOUND_IN_MANY);
    std::vector<char> expectedRes = {'F','H','E','L','L','O',' ',
                                     'H','E','L','L','O',' ',
                                     'T','C','P',' ',
                                     'S','E','R','V','E','R', '\n'};
    ASSERT_EQ(processedData, expectedRes);
}

class HASH_TEST :public HashProtocol, public testing::Test
{
public:
    HASH_TEST():HashProtocol(hashName, threadPool), testing::Test(){}
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

TEST_F(HASH_TEST, SHA512){
    setCryptoFunc(SHA512);
    std::vector<char> buf = {'F', 'H','E','L','L','O',' ', '\n'};
    std::vector<char> expbuf = {'F', 'H','E','L','L','O',' '};
    auto res = _cryptoFunc(buf.data(), buf.size() - 1);
    auto expect = SHA512(expbuf.data(), expbuf.size());
    ASSERT_NE(expect.size(), 0);
    ASSERT_NE(res.size(), 0);
    ASSERT_EQ(res, expect);
}