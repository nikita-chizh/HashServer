#include <vector>
#include <gtest/gtest.h> // googletest header file
#include "../src/HashProtocol.h"

const int clientSock = 1;
std::vector<char> testMsg = {'H', 'E', 'L', 'L', 'O', ' ',
                             'T', 'C', 'P', ' ',
                             'S', 'E', 'R', 'V', 'E', 'R'};

std::string hashName = "sha256";
std::atomic<bool> stop {false};
ThreadPool threadPool(0, stop);

TEST(ProcessTesting, clientNotExist) {
    HashProtocol hashProtocol(hashName, threadPool);
    try {
        auto res = hashProtocol.processChunk(clientSock, testMsg.data(), testMsg.size());
    }
    catch (const std::runtime_error &error){
        ASSERT_STREQ("ERROR processChunk fd=1 doesn't exist", error.what());
    }
}

TEST(ProcessTesting, testOnePacket) {
    HashProtocol hashProtocol(hashName, threadPool);
    hashProtocol.acceptClient(clientSock);
    auto onePack = testMsg;
    onePack.push_back('\n');
    auto res = hashProtocol.processChunk(clientSock, onePack.data(), onePack.size());
    ASSERT_EQ(res, testMsg);
}

TEST(ProcessTesting, testOnePacketInAMiddle) {
    HashProtocol hashProtocol(hashName, threadPool);
    hashProtocol.acceptClient(clientSock);
    auto onePack = testMsg;
    onePack[5] = '\n';
    std::vector<char> expected = {'H', 'E', 'L', 'L', 'O'};
    auto res = hashProtocol.processChunk(clientSock, onePack.data(), onePack.size());
    ASSERT_EQ(res, expected);
}

//
TEST(ProcessTesting, testMultiplePackets) {
    HashProtocol hashProtocol(hashName, threadPool);
    hashProtocol.acceptClient(clientSock);
    // First message
    std::vector<char> firstMsg = {'F', 'H', 'E', 'L', 'L', 'O', ' '};
    auto res = hashProtocol.processChunk(clientSock, firstMsg.data(), firstMsg.size());
    ASSERT_EQ(res, std::vector<char>());
    //
    auto _testMsg = testMsg;
    _testMsg.push_back('\n');
    res = hashProtocol.processChunk(clientSock, _testMsg.data(), _testMsg.size());
    std::vector<char> expectedRes = {'F','H','E','L','L','O',' ',
                                     'H','E','L','L','O',' ',
                                     'T','C','P',' ',
                                     'S','E','R','V','E','R'};
    ASSERT_EQ(res, expectedRes);
}
//
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