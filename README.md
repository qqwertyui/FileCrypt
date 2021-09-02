File encryption project created for learning purposes.

1. Dependecies:
- CryptoPP
- Gflags (slightly modified version, but normal one does job too)
- Boost (filesystem module)

2. Description:
It uses AES-CBC mode with 128 bit key length. Can be used to protect files on disk by encrypting using previously generated secret key.

3. Basic usage:

- Help: ./fcrypt --help

- Generate key: ./fcrypt --mode generate --path ./key

- Encrypt file: ./fcrypt --mode encrypt --path $env:USERPROFILE/Desktop/secret.txt --key path/to/key

- Decrypt file: ./fcrypt --mode decrypt --path $env:USERPROFILE/Desktop/secret.txt --key path/to/key

4. Build
- git clone https://github.com/qqwertyui/FileCrypt.git && cd FileCrypt
- mkdir build && cd build
- cmake .. -G "Unix Makefiles"
- make
