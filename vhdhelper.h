#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

#define HD_NO_FEATURES     0x00000000 
#define HD_TEMPORARY       0x00000001 /* disk can be deleted on shutdown */ 
#define HD_RESERVED        0x00000002 /* NOTE: must always be set        */ 

#define HD_TYPE_NONE       0 
#define HD_TYPE_FIXED      2  /* fixed-allocation disk */ 
#define HD_TYPE_DYNAMIC    3  /* dynamic disk */ 
#define HD_TYPE_DIFF       4  /* differencing disk */ 

#define GEO_GET_C(_g)  (((_g) & 0xff) << 8) | (((_g) & 0xff00) >> 8)
#define GEO_GET_H(_g)  (((_g) >> 16)  & 0xff) 
#define GEO_GET_S(_g)  ((_g) >> 24)

typedef struct vhd_uuid_t{
	u32 f1;
	u16 f2;
	u16 f3;
	u8  f4[8];
} vhd_uuid_t;

typedef struct hd_ftr{ 
  char   cookie[8];       /* Identifies original creator of the disk      */ 
  u32    features;        /* Feature Support -- see below                 */ 
  u32    ff_version;      /* (major,minor) version of disk file           */ 
  u64    data_offset;     /* Abs. offset from SOF to next structure       */ 
  u32    timestamp;       /* Creation time.  secs since 1/1/2000GMT       */ 
  char   crtr_app[4];     /* Creator application                          */ 
  u32    crtr_ver;        /* Creator version (major,minor)                */ 
  char   crtr_os[4];      /* Creator host OS                              */ 
  u64    orig_size;       /* Size at creation (bytes)                     */ 
  u64    curr_size;       /* Current size of disk (bytes)                 */ 
  u32    geometry;        /* Disk geometry                                */ 
  u32    type;            /* Disk type                                    */ 
  u32    checksum;        /* 1's comp sum of this struct.                 */ 
  vhd_uuid_t uuid;        /* Unique disk ID, used for naming parents      */ 
  char   saved;           /* one-bit -- is this disk/VM in a saved state? */ 
  char   hidden;          /* tapdisk-specific field: is this vdi hidden?  */ 
  char   reserved[426];   /* padding                                      */ 
} hd_ftr;
