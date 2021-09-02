#include <cryptopp/cryptlib.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <fstream>

#include "Log.hpp"
#include "Keygen.hpp"

bool Keygen::generate(std::string path) {
    if(path.empty() == true) {
        return false;
    }
    CryptoPP::AutoSeededRandomPool gtor;
    CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    
    gtor.GenerateBlock(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    gtor.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);

    std::ofstream file(path);
    if(file.good() == false) {
        Log::error("Failed to write key in raw bytes format.\n");
        return false;
    }
    file.write((char*)key.data(), CryptoPP::AES::DEFAULT_KEYLENGTH);
    file.write((char*)iv.data(), CryptoPP::AES::BLOCKSIZE);
    file.close();

    return true;
}
