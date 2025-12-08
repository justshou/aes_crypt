# AES Crypt
AES Crypt is an OpenSSL EVP Implementation for AES Encryption using AES-256-CBC

## Installation and Compilation

Run git clone on the repo:

`
git clone https://github.com/justshou/aes_crypt.git
`

To compile, make sure to link the lcrypto library

`
g++ aes-256-cbc.cpp -o aes -lcrypto
`

## Usage

`
./aes (Input File) (Output File) (Key) (Encrypt/Decrypt (E/D)) (Salted? (Y/N))
`

Example encryption on file "image.jpg" with key "password" and salted.

`
./aes image.jpg image.enc password E Y
`

Example decryption on file "image.enc" with key "password" and salted.

`
./aes image.enc image.jpg password D Y
`

# IMPORTANT
This was made by me for my operating systems class using the following resources. I am not responsible if you somehow break anything because you misused it.
[OpenSSL](https://docs.openssl.org/3.2/man3/EVP_EncryptInit/)
[OpenSSL Wiki](https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption)
