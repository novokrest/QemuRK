#ifndef CRYPTOGRAPHER_HPP
#define CRYPTOGRAPHER_HPP

#include "common/common.hpp"

class Cryptographer
{
    rsa_key key_;

public:
    static int GenerateKey(rsa_key *key);

    void SetKey(rsa_key key);
    rsa_key GetKey() const;

    int Encrypt(const unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen);
    int Decrypt(const unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen);
};


void cryptographer_init_environment()
__attribute__((constructor));

void cryptographer_free_environment()
__attribute__((destructor));


#endif // CRYPTOGRAPHER_HPP
