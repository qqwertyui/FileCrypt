#ifndef CMD_HPP
#define CMD_HPP

#include <gflags/gflags.h>

enum Status {
    OK = 0,
    INVALID_ARGS = 1
};

DEFINE_string(mode, "", "Operation mode [encrypt, decrypt, generate]");
DEFINE_string(path, "", "Path to file/directory to encrypt/decrypt/generate.");
DEFINE_string(key, "", "Path to key");

#endif