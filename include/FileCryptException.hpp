#ifndef FILECRYPTEXCEPTION_HPP
#define FILECRYPTEXCEPTION_HPP

class FileCryptException {
public:
    enum {
        ENCRYPTION_ERROR = 10,
        DECRYPTION_ERROR = 11,
        FILE_ERROR = 12,
        KEY_ERROR = 13,
        GENERATION_ERROR = 14
    };
    FileCryptException(const char *error, int code);
    int get_code();
    const char *what();

private:
    const char *msg;
    int code;
};

#endif