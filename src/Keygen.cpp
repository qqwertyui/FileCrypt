#include <cryptopp/cryptlib.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <fstream>

#include "FileCryptException.hpp"
#include "Keygen.hpp"

void Keygen::generate(std::string path) {
    std::ofstream file(path);
    if(file.good() == false) {
        throw FileCryptException("Couldn't write key.", FileCryptException::GENERATION_ERROR);
    }
    CryptoPP::AutoSeededRandomPool gtor;
    CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    
    gtor.GenerateBlock(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    gtor.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);

    file.write((char*)key.data(), CryptoPP::AES::DEFAULT_KEYLENGTH);
    file.write((char*)iv.data(), CryptoPP::AES::BLOCKSIZE);
    file.close();
}
