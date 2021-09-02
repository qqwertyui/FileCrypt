#ifndef FILECRYPT_HPP
#define FILECRYPT_HPP

#include <string>
#include <fstream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/rijndael.h>

class FileCrypt;

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

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