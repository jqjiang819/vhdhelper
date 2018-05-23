#include <string.h>
#if defined(__unix) || defined(__linux) || defined(__APPLE__)
#include <uuid/uuid.h>
#elif defined(_WIN32)
#include <stdio.h>
#include <objbase.h>
#endif

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long

typedef struct {
	uint32_t f1;
	uint16_t f2;
	uint16_t f3;
	uint8_t  f4[8];
} vhd_uuid_t;

extern vhd_uuid_t uuid_get_uuid();