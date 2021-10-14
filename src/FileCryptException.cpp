#include "FileCryptException.hpp"

FileCryptException::FileCryptException(const char *msg, int code) {
  this->code = code;
  this->msg = msg;
}

const char *FileCryptException::what() { return this->msg; }

int FileCryptException::get_code() { return this->code; }