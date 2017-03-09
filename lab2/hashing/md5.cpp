
#include "md5.hpp"

void MD5Hash::MD5(std::string input){
    CryptoPP::Weak::MD5 hashMD5;
    hashMD5.CalculateDigest(digest, (byte*)input.c_str(), input.length());
}

void MD5Hash::MD5(char* input, int size){
    CryptoPP::Weak::MD5 hashMD5;
    hashMD5.CalculateDigest(digest, (byte*)input, size);
}

void MD5Hash::clear(){
    hash="";
}

std::string MD5Hash::getHash(std::string input){
    MD5(input);
    return getHash();
}

std::string MD5Hash::getHash(char* input, int size){
    MD5(input, size);
    return getHash();
}

std::string MD5Hash::getHash(){
    CryptoPP::HexEncoder encode;

    encode.Attach(new CryptoPP::StringSink(hash));
    encode.Put(digest, sizeof(digest));
    encode.MessageEnd();

    return hash;
}
