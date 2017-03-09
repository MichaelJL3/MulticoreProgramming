
#ifndef MD5_HASH_HPP
#define MD5_HASH_HPP

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <crypto++/md5.h>
#include <crypto++/hex.h>

class MD5Hash{
    byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];
    std::string hash;
public:
    void MD5(std::string input);
    void MD5(char* input, int size);
    void clear();
    std::string getHash();
    std::string getHash(std::string input);
    std::string getHash(char* input, int size);
};

#endif