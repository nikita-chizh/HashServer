#include "CryptoFunctions.h"
#include "cryptopp/hex.h"

std::string hexify(const CryptoPP::byte *digest, const size_t size){
    CryptoPP::HexEncoder encoder;
    std::string res;
    encoder.Attach( new CryptoPP::StringSink( res ) );
    encoder.Put( digest, sizeof(digest) );
    encoder.MessageEnd();
    res.push_back('\n');
    return res;
}

std::string SHA256(const char* data, const size_t size){
    auto *dataPtr = reinterpret_cast<const CryptoPP::byte*>(data);
    auto digestSize = CryptoPP::SHA256::DIGESTSIZE;
    CryptoPP::byte digest[digestSize];
    CryptoPP::SHA256().CalculateDigest(digest, dataPtr, size);
    return hexify(digest, digestSize);
}

std::string SHA512(const char* data, const size_t size){
    auto *dataPtr = reinterpret_cast<const CryptoPP::byte*>(data);
    auto digestSize = CryptoPP::SHA512::DIGESTSIZE;
    CryptoPP::byte digest[digestSize];
    CryptoPP::SHA512().CalculateDigest(digest, dataPtr, size);
    return hexify(digest, digestSize);
}
