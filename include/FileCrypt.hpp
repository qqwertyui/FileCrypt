#ifndef FILECRYPT_HPP
#define FILECRYPT_HPP

#include <string>
#include <fstream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/rijndael.h>

class FileCrypt;

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

namespace Status {
    constexpr int OK = 0;
    constexpr int FILE_ERROR = 1;
    constexpr int KEY_ERROR = 2;
    constexpr int OVERWRITE_ERROR = 3;
    constexpr int ALREADY_CLOSED = 4;
    constexpr int DECRYPTION_ERROR = 5;
    constexpr int ENCRYPTION_ERROR = 6;
    constexpr int INVALID_ARGS = 8;
    constexpr int INVALID_MODE = 9;
    constexpr int GENERATION_FAILURE = 10;
}

class FileCrypt {
    std::string* decrypt_internal(std::fstream *file, std::string &buffer);
    std::string* encrypt_internal(std::fstream *file, std::string &buffer);

public:
    typedef std::string*(FileCrypt::*Operation)(std::fstream*, std::string&);
    static constexpr FileCrypt::Operation Encrypt = FileCrypt::encrypt_internal;
    static constexpr FileCrypt::Operation Decrypt = FileCrypt::decrypt_internal;

    FileCrypt(std::string keys_path);
    ~FileCrypt();

    void crypt(std::string path, FileCrypt::Operation op);

private:
    std::fstream* open_file(std::string path, std::fstream::openmode mode);
    void close_file(std::fstream *file);

    void load_keys(std::string path);

    void file(std::string path, FileCrypt::Operation op);
    void dir(std::string path, FileCrypt::Operation op);


    CryptoPP::SecByteBlock *key;
    CryptoPP::SecByteBlock *iv;

};

#endif