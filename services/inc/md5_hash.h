/*
 * MD5 internal definitions
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef MD5_HASH_H_
#define MD5_HASH_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


struct MD5Context {
    uint32_t buf[4];
    uint32_t bits[2];
    uint8_t  in[64];
};

int md5_vector(size_t num_elem, const uint8_t *addr[], const size_t *len, uint8_t *mac);
void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, uint8_t const *buf, uint32_t len);
void MD5Final(uint8_t digest[16], struct MD5Context *context);


#ifdef __cplusplus
}
#endif


#endif /* MD5_HASH_H_ */
