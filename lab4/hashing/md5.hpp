
#ifndef MD5_HASH_HPP
#define MD5_HASH_HPP

/*****************************************\

 md5.hpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 contains a class skeleton for md5 hashing

 **uses the crypto++ library

\*****************************************/

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "cryptopp565/md5.h"
#include "cryptopp565/hex.h"

class MD5Hash{
    CryptoPP::Weak::MD5 hashMD5;
    CryptoPP::HexEncoder encode;
    byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];
    std::string hash;
public:
    MD5Hash();
    void MD5(std::string input);                //hash a string
    void MD5(char* input, int size);            //hash c char*
    void clear();                               //clear the hash
    std::string getHash();                      //retrieve the hash
    std::string getHash(std::string input);     //calculate and return the hash (string)
    std::string getHash(char* input, int size); //calculate and return the hash (char*)
};

#endif
