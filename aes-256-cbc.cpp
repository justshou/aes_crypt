#include <iostream>
#include <string.h>

#include <openssl/evp.h>

int encrypt(EVP_CIPHER_CTX* ctx, FILE* inFile, FILE* outFile, const char* key, bool salted) {
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();

    return 0;
}

int main(int argc, char* argv[]) {

    if (argc < 6) {
        std::cout << "(Input File) (Output File) (Key) (Encrypt/Decrypt (E/D)) (Salted? (Y/N))" << std::endl;
        return 1;
    }

    bool encdec; // true = encrypt, false = decrypt
    bool salted; // true = salted, false = not salted

    if (strcmp(argv[4], "E") == 0) {
        encdec = true;
    } else if (strcmp(argv[4], "D") == 0) {
        encdec = false;
    } 

    if (strcmp(argv[5], "Y") == 0) {
        salted = true;
    } else if (strcmp(argv[5], "N") == 0) {
        salted = false;
    }

    FILE *inFile = fopen(argv[1], "rb");
    FILE *outFile = fopen(argv[2], "wb");
    const char *key = argv[3];
    
    OpenSSL_add_all_algorithms();

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return 1;
    }

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    EVP_cleanup();

    std::cout << "Freed and cleaned up EVP." << std::endl;
    return 0;
}