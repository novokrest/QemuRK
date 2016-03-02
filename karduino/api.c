#include "api.h"
#include <string.h>
#include <rpc/des_crypt.h>

#define DES_CRYPTOR_DECRYPT_SUCCESS  0
#define DES_CRYPTOR_DECRYPT_FAIL    -1

typedef char DesKey[8];
typedef char DesBlock[8];

static DesKey key = {1,2,3,4,5,6,7,8};

static int des_cryptor_decrypt(DesKey key, const DesBlock encryptedBlock, DesBlock decryptedBlock)
{
    for (size_t i = 0; i < sizeof(encryptedBlock); ++i) {
        decryptedBlock[i] = encryptedBlock[i];
    }

    int res = ecb_crypt((char *)key, decryptedBlock, sizeof(decryptedBlock), DES_DECRYPT);
    if (res == DESERR_NONE || res == DESERR_NOHWDEVICE) {
        return DES_CRYPTOR_DECRYPT_SUCCESS;
    }

    return DES_FAILED(res) == 1 ? DES_CRYPTOR_DECRYPT_SUCCESS : DES_CRYPTOR_DECRYPT_FAIL;
}

int cryptor_decode(char *buffer, size_t size)
{
   DesBlock in, out;
   if (size % sizeof(DesBlock)) {
       return -1;
   }

   for (size_t i = 0; i < size; i += sizeof(DesBlock)) {
       memcpy(in, buffer + i, sizeof(DesBlock));
       des_cryptor_decrypt(key, in, out);
       memcpy(buffer, out, sizeof(DesBlock));
   }

   return DES_CRYPTOR_DECRYPT_SUCCESS;
}
