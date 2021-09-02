#ifndef FILECRYPTEXCEPTION_HPP
#define FILECRYPTEXCEPTION_HPP

class FileCryptException {
public:
    FileCryptException(const char *error, int code);
    int get_code();
    const char *what();

private:
    const char *msg;
    int code;
};

#endif