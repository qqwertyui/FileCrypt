#include "FileCrypt.hpp"
#include "FileCryptException.hpp"
#include "Log.hpp"

#include <cinttypes>
#include <cstdint>

#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

#include <boost/filesystem.hpp>

FileCrypt::FileCrypt(std::string keys_path) { this->load_keys(keys_path); }

FileCrypt::~FileCrypt() {
  if (this->key) {
    delete this->key;
  }
  if (this->iv) {
    delete this->iv;
  }
}

std::string *FileCrypt::encrypt_internal(std::fstream *file,
                                         std::string &buffer) {
  CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
  enc.SetKeyWithIV(*this->key, this->key->size(), *this->iv);

  // StringSink leaks memory
  CryptoPP::StringSink *ss = new CryptoPP::StringSink(buffer);
  try {
    CryptoPP::FileSource src(
        *file, true,
        new CryptoPP::StreamTransformationFilter(
            enc, ss, CryptoPP::StreamTransformationFilter::PKCS_PADDING));
  } catch (CryptoPP::InvalidCiphertext &e) {
    throw FileCryptException("Cannot encrypt file",
                             FileCryptException::ENCRYPTION_ERROR);
  }
  return &buffer;
}

std::string *FileCrypt::decrypt_internal(std::fstream *file,
                                         std::string &buffer) {
  CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
  dec.SetKeyWithIV(*this->key, this->key->size(), *this->iv);

  // StringSink leaks memory
  CryptoPP::StringSink *ss = new CryptoPP::StringSink(buffer);
  try {
    CryptoPP::FileSource src(
        *file, true,
        new CryptoPP::StreamTransformationFilter(
            dec, ss, CryptoPP::StreamTransformationFilter::PKCS_PADDING));
  } catch (CryptoPP::InvalidCiphertext &e) {
    throw FileCryptException("Cannot decrypt file",
                             FileCryptException::DECRYPTION_ERROR);
  }

  return &buffer;
}

void FileCrypt::crypt(std::string path, FileCrypt::Operation op) {
  if (boost::filesystem::is_directory(path) == true) {
    uint64_t total_bytes = 0;
    uint64_t total_files = 0;

    boost::filesystem::recursive_directory_iterator end1;
    for (boost::filesystem::recursive_directory_iterator it(path.c_str());
         it != end1; ++it) {
      boost::filesystem::path boost_path = it->path();
      if (boost::filesystem::is_directory(boost_path)) {
        continue;
      }
      total_bytes += boost::filesystem::file_size(boost_path);
      total_files += 1;
    }
    Log::infof("%" SCNd64 " file(s) (%" SCNd64 " bytes) will be altered.\n",
               total_files, total_bytes);
    this->dir(path, op);
  } else {
    this->file(path, op);
  }
}

void FileCrypt::file(std::string path, FileCrypt::Operation op) {
  std::fstream *file =
      this->open_file(path, std::fstream::in | std::fstream::binary);

  std::string buffer;
  try {
    CALL_MEMBER_FN(this, op)(file, buffer);
  } catch (FileCryptException &e) {
    this->close_file(file);
    throw FileCryptException(e);
  }
  this->close_file(file);

  file = this->open_file(path, std::fstream::out | std::fstream::binary);
  file->write(buffer.c_str(), buffer.size());
  this->close_file(file);
}

void FileCrypt::dir(std::string path, FileCrypt::Operation op) {
  boost::filesystem::recursive_directory_iterator end2;
  for (boost::filesystem::recursive_directory_iterator it(path.c_str());
       it != end2; ++it) {
    boost::filesystem::path boost_path = it->path();
    if (boost::filesystem::is_directory(boost_path)) {
      continue;
    }
    try {
      puts(boost_path.string().c_str());
      this->file(boost_path.string(), op);
    } catch (FileCryptException &e) {
      continue;
    }
  }
}

std::fstream *FileCrypt::open_file(std::string path,
                                   std::fstream::openmode mode) {
  std::fstream *file = new std::fstream(path, mode);
  if (file->good() == false) {
    throw FileCryptException("Failed to open file.",
                             FileCryptException::FILE_ERROR);
  }
  return file;
}

void FileCrypt::close_file(std::fstream *file) {
  if (file) {
    file->close();
    delete file;
  }
}

void FileCrypt::load_keys(std::string path) {
  std::fstream *key_file = nullptr;
  try {
    key_file = this->open_file(path, std::fstream::in | std::fstream::binary);
  } catch (FileCryptException &e) {
    throw FileCryptException("Failed to load key file",
                             FileCryptException::KEY_ERROR);
  }

  unsigned char key[CryptoPP::AES::DEFAULT_KEYLENGTH] = {};
  unsigned char iv[CryptoPP::AES::BLOCKSIZE] = {};

  key_file->read((char *)key, CryptoPP::AES::DEFAULT_KEYLENGTH);
  key_file->read((char *)iv, CryptoPP::AES::BLOCKSIZE);

  this->key = new CryptoPP::SecByteBlock(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
  this->iv = new CryptoPP::SecByteBlock(iv, CryptoPP::AES::BLOCKSIZE);

  this->close_file(key_file);
}