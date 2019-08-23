#pragma once
#include "Utils.h"
#include <cryptopp/sha.h>
#include <cryptopp/config.h>

using CryptoFunc = std::function<std::string(const char* data, const size_t size)>;

std::string hexify(const byte *digest, const size_t size);

std::string SHA256(const char* data, const size_t size);

std::string SHA512(const char* data, const size_t size);