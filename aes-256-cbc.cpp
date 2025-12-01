#include <iostream>
#include <string.h>

#include <openssl/evp.h>

int encrypt(EVP_CIPHER_CTX* ctx, FILE* inFile, FILE* outFile, const unsigned char* key, const unsigned char* iv) {
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();

    char inputBuffer[1024];
    char outputBuffer[1024 + EVP_MAX_BLOCK_LENGTH];

    size_t bytesRead;
    while ((bytesRead = std::fread(inputBuffer, 1, sizeof(inputBuffer), inFile)) > 0) {
        int outLen;
        if (1 != EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(outputBuffer), &outLen,
                                   reinterpret_cast<unsigned char*>(inputBuffer), bytesRead)) {
            std::cout << "Encryption failed (line 17)" << std::endl;
            return -1;
        }
        std::fwrite(outputBuffer, 1, outLen, outFile);
    }

    return 0;
}

int main(int argc, char* argv[]) {

    if (argc < 6) {
        std::cout << "(Input File) (Output File) (Key) (Encrypt/Decrypt (E/D)) (Salted? (Y/N))" << std::endl;
        return 1;
    }

    bool encdec; // true = encrypt, false = decrypt
    bool salted; // true = salted, false = not salted
    unsigned char salt[8];
    unsigned char key[32];
    unsigned char iv[16];

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

    //convert with EVP_bytestokey
    EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salted ? salt : nullptr, reinterpret_cast<const unsigned char*>(argv[3]), strlen(argv[3]), 1, key, iv);
    const char *key = argv[3];
    
    // default stuff for setup
    OpenSSL_add_all_algorithms();
    ERR_load_CRYPTO_strings();

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return 1;
    }

    // check what to run based on if enc/dec or salted
    if(encdec && salted) {
        encrypt(ctx, inFile, outFile, key, iv);
    } else if (!encdec) {
        // decrypt function would go here
    }

    // close everything
    EVP_CIPHER_CTX_free(ctx);
    EVP_cleanup();
    fclose(inFile);
    fclose(outFile);

    return 0;
}