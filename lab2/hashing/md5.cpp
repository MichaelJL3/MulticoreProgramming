
#include "md5.hpp"

/*****************************************\

 md5.cpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 contains an implementation of md5 class
 from md5.hpp

 **not thread safe should only be accessed
   by a single thread or created locally

 **uses the crypto++ library

\*****************************************/

//constructor 
MD5Hash::MD5Hash(){
    encode.Attach(new CryptoPP::StringSink(hash));
}

//hash a string
void MD5Hash::MD5(std::string input){
    hashMD5.CalculateDigest(digest, (byte*)input.c_str(), input.length());
}

//hash a char*
void MD5Hash::MD5(char* input, int size){
    hashMD5.CalculateDigest(digest, (byte*)input, size);
}

//clear the hash
void MD5Hash::clear(){
    hash="";
}

//calculate and return the hash (string)
std::string MD5Hash::getHash(std::string input){
    MD5(input);
    return getHash();
}

//calculate and return the hash (char*)
std::string MD5Hash::getHash(char* input, int size){
    MD5(input, size);
    return getHash();
}

//calculate and return the hash
std::string MD5Hash::getHash(){
    encode.Put(digest, sizeof(digest));
    encode.MessageEnd();

    return hash;
}
