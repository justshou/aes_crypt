#include <iostream>
#include <string.h>

#include <openssl/evp.h>

int encrypt(EVP_CIPHER_CTX* ctx, FILE* inFile, FILE* outFile, const unsigned char* key, const unsigned char* iv) {
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();

    unsigned char inputBuffer[1024];
    unsigned char outputBuffer[1024 + EVP_MAX_BLOCK_LENGTH];
    size_t bytesRead;
    int final;

    if (EVP_EncryptInit_ex(ctx, cipher, nullptr, key, iv) != 1) {
        std::cout << "EncryptionInit failed (line 12)" << std::endl;
        return -1;
    }

    while ((bytesRead = std::fread(inputBuffer, 1, sizeof(inputBuffer), inFile)) > 0) {
        int outLen;
        if (1 != EVP_EncryptUpdate(ctx, outputBuffer, &outLen, inputBuffer, bytesRead)) {
            std::cout << "EncryptionUpdate failed (line 17)" << std::endl;
            return -1;
        }
        std::fwrite(outputBuffer, 1, outLen, outFile);
    }

    if (EVP_EncryptFinal_ex(ctx, outputBuffer, &final) != 1) {
        std::cout << "EncryptionFinal failed (line 24)" << std::endl;
        return -1;
    }
    std::fwrite(outputBuffer, 1, final, outFile);
    return 0;
}

int decrypt(EVP_CIPHER_CTX* ctx, FILE* inFile, FILE* outFile, const unsigned char* key, const unsigned char* iv) {
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();

    unsigned char inputBuffer[1024];
    unsigned char outputBuffer[1024 + EVP_MAX_BLOCK_LENGTH];
    size_t bytesRead;
    int final;

    if (EVP_DecryptInit_ex(ctx, cipher, nullptr, key, iv) != 1) {
        std::cout << "DecryptionInit failed" << std::endl;
        return -1;
    }

    while ((bytesRead = std::fread(inputBuffer, 1, sizeof(inputBuffer), inFile)) > 0) {
        int outLen;
        if (1 != EVP_DecryptUpdate(ctx, outputBuffer, &outLen, inputBuffer, bytesRead)) {
            std::cout << "DecryptionUpdate failed" << std::endl;
            return -1;
        }
        std::fwrite(outputBuffer, 1, outLen, outFile);
    }

    if (EVP_DecryptFinal_ex(ctx, outputBuffer, &final) != 1) {
        std::cout << "DecryptionFinal failed" << std::endl;
        return -1;
    }
    std::fwrite(outputBuffer, 1, final, outFile);
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
    EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), nullptr, reinterpret_cast<const unsigned char*>(argv[3]), strlen(argv[3]), 1, key, iv);
    // std::cout << "key test: " << key << std::endl;
    // std::cout << "iv test: " << iv << std::endl;
    
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
    } else if (encdec && !salted) {
        encrypt(ctx, inFile, outFile, key, iv);
    }

    
    if (!encdec && salted) {
        decrypt(ctx, inFile, outFile, key, iv);
    } else if (!encdec && !salted) {
        decrypt(ctx, inFile, outFile, key, iv);
    }

    // close everything
    EVP_CIPHER_CTX_free(ctx);
    EVP_cleanup();
    fclose(inFile);
    fclose(outFile);

    return 0;
}