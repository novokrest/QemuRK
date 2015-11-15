#include "cryptographer.hpp"

int Cryptographer::GenerateKey(rsa_key *key)
{
    int err, prng_idx;

    prng_idx = find_prng("sprng");
    /* make an RSA-1024 key */
    if ((err = rsa_make_key(NULL,
                            /* PRNG state */
                            prng_idx, /* PRNG idx */
                            1024 / 8,
                            /* 1024-bit key */
                            65537,
                            /* we like e=65537 */
                            key)
         /* where to store the key */
         ) != CRYPT_OK) {
        std::cout << "Fail to generate key: " << error_to_string(err) << std::endl;
        return -1;
    }
    return 0;
}

void Cryptographer::SetKey(rsa_key key)
{
    key_ = key;
}

rsa_key Cryptographer::GetKey() const
{
    return key_;
}

int Cryptographer::Encrypt(const unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen)
{
    int err, hash_idx, prng_idx;
    unsigned char tmp[1024];
    unsigned long tmplen;

    hash_idx = find_hash("sha1");
    prng_idx = find_prng("sprng");
    tmplen = sizeof(tmp);

    if ((err = rsa_encrypt_key(in, /* data we wish to encrypt */
                               inlen, /* data is 16 bytes long */
                               tmp, /* where to store ciphertext */
                               &tmplen, /* length of ciphertext */
                               (const unsigned char *)"Cryptographer", /* our lparam for this program */
                               13, /* lparam is 7 bytes long */
                               NULL, /* PRNG state */
                               prng_idx, /* prng idx */
                               hash_idx, /* hash idx */
                               &key_) /* our RSA key */
         ) != CRYPT_OK) {
        std::cout << "Fail to encrypt by RSA: " << error_to_string(err) << std::endl;
        return -1;
    }

    if (base64_encode(tmp, tmplen, out, outlen) == -1) {
        std::cout << "Fail to encrypt by Base64: " << error_to_string(err) << std::endl;
        return -1;
    }

    return 0;
}

int Cryptographer::Decrypt(const unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen)
{
    int err, res, hash_idx;
    unsigned char tmp[1024];
    unsigned long tmplen;

    hash_idx = find_hash("sha1");
    tmplen = sizeof(tmp);

    if ((err = rsa_decrypt_key(in, /* encrypted data */
                               inlen, /* length of ciphertext */
                               tmp, /* where to put plaintext */
                               &tmplen, /* plaintext length */
                               (const unsigned char *)"Cryptographer", /* lparam for this program */
                               13, /* lparam is 7 bytes long */
                               hash_idx, /* hash idx */
                               &res, /* validity of data */
                               &key_) /* our RSA key */
         ) != CRYPT_OK) {
        std::cout << "Fail to decrypt by RSA: " << error_to_string(err) << std::endl;
        return -1;
    }

    if (base64_decode(tmp, tmplen, out, outlen) == -1) {
        std::cout << "Fail to encrypt by Base64: " << error_to_string(err) << std::endl;
        return -1;
    }

    return 0;
}

void cryptographer_init_environment()
{
    if (register_prng(&sprng_desc) == -1) {
        std::cout << "Error registering sprng" << std::endl;
        return;
    }

    /* register a math library (in this case TomsFastMath) */
    ltc_mp = tfm_desc;

    if (register_hash(&sha1_desc) == -1) {
        std::cout << "Error registering sha1" << std::endl;
    }
}

void cryptographer_free_environment()
{
    if (unregister_hash(&sha1_desc) == -1) {

        std::cout << "Error unregistring sha1" << std::endl;
        return;
    }

    //ltc_mp = (const ltc_math_descriptor) {0};

    if (unregister_prng(&sprng_desc) == -1) {
        std::cout << "Error unregistering sprng" << std::endl;
    }
}
