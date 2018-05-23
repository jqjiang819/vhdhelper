#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long

#define HD_COOKIES         "conectix"
#define HD_CREATOR_APP     "vhlp"
#define HD_CREATOR_VER     0x00010000
#define HD_CREATOR_OS_WIN  "Wi2k"

#define HD_NO_FEATURES     0x00000000 
#define HD_TEMPORARY       0x00000001 /* disk can be deleted on shutdown */ 
#define HD_RESERVED        0x00000002 /* NOTE: must always be set        */ 

#define HD_FILE_FORMAT_VER 0x00010000 /* must be set */

#define HD_FIXED_OFFSET    0xffffffffffffffff

#define HD_TIMESTAMP_START 946684800

#define HD_TYPE_NONE       0 
#define HD_TYPE_FIXED      2  /* fixed-allocation disk */ 
#define HD_TYPE_DYNAMIC    3  /* dynamic disk */ 
#define HD_TYPE_DIFF       4  /* differencing disk */ 

#define MB_SIZE            1024 * 1024

#define REVERSE_BYTES_U16(_b)  ((((_b) & 0xff) << 8) | (((_b) & 0xff00) >> 8))
#define REVERSE_BYTES_U32(_b)  ((((_b) & 0xff) << 24) | (((_b) & 0xff00) << 8) | (((_b) & 0xff0000) >> 8) | (((_b) & 0xff000000) >> 24))
#define REVERSE_BYTES_U64(_b)  ((REVERSE_BYTES_U32((_b) & 0xffffffff) << 32) | (REVERSE_BYTES_U32((_b) >> 32)))

#define GEO_GET_C(_g)  (((_g) & 0xff) << 8) | (((_g) & 0xff00) >> 8)
#define GEO_GET_H(_g)  (((_g) >> 16)  & 0xff) 
#define GEO_GET_S(_g)  ((_g) >> 24)

#include "vhduuid.h"

typedef struct { 
  uint8_t     cookie[8];       /* Identifies original creator of the disk      */ 
  uint32_t    features;        /* Feature Support -- see below                 */ 
  uint32_t    ff_version;      /* (major,minor) version of disk file           */ 
  uint64_t    data_offset;     /* Abs. offset from SOF to next structure       */ 
  uint32_t    timestamp;       /* Creation time.  secs since 1/1/2000GMT       */ 
  uint8_t     crtr_app[4];     /* Creator application                          */ 
  uint32_t    crtr_ver;        /* Creator version (major,minor)                */ 
  uint8_t     crtr_os[4];      /* Creator host OS                              */ 
  uint64_t    orig_size;       /* Size at creation (bytes)                     */ 
  uint64_t    curr_size;       /* Current size of disk (bytes)                 */ 
  uint32_t    geometry;        /* Disk geometry                                */ 
  uint32_t    type;            /* Disk type                                    */ 
  uint32_t    checksum;        /* 1's comp sum of this struct.                 */ 
  vhd_uuid_t  uuid;        /* Unique disk ID, used for naming parents      */ 
  uint8_t     saved;           /* one-bit -- is this disk/VM in a saved state? */ 
  uint8_t     hidden;          /* tapdisk-specific field: is this vdi hidden?  */ 
  uint8_t     reserved[426];   /* padding                                      */ 
} vhd_ftr;
