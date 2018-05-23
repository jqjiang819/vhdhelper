#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "vhdhelper.h"

void display_info(vhd_ftr footer);
int copy_bin(int start, char *bin, char *vhd);
int check_vhd_ftr(vhd_ftr footer);
uint32_t get_vhd_checksum(vhd_ftr footer);
uint32_t generate_geo(uint64_t size);
int create_vhd(int size, char *vhd);
vhd_ftr construct_ftr(int size);

int main(int argc, char *argv[]) {
	int ch;
	struct{
		int type;
		int size;
		int lba_start;
		char *vhd, *bin;
	}args;
	
	if(argc==1) {
		printf("vhdhelper: missing operand\n");
		printf("Try 'vhdhelper -h' for more information.\n");
		return 0;
	}
	args.type = 0;
	args.size = 4;
	args.lba_start = 0;
	args.vhd = args.bin = NULL;
	while ((ch = getopt(argc, argv, "caihb:v:s:l:")) != -1) {
		switch (ch) {
			case 'c':
				args.type = 0x10;
				break;
			case 'a':
				args.type = 0x11;
				break;
			case 'i':
				args.type = 0x12;
				break;
			case 'h':
				args.type = 0x01;
				break;
			case 'b':
        		args.bin = optarg;
        		break;
        	case 'v':
        		args.vhd = optarg;
        		break;
			case 's':
				args.size = atoi(optarg);
				break;
			case 'l':
				args.lba_start = atoi(optarg);
                break;			
		} 
	}
	if(args.type == 0x01) {
		printf("Usages:\n");
		printf("vhdhelper -c [-b BIN_FILE] -v VHD_FILE [-s SIZE] [-l LBA_START]  create new VHD file\n");
		printf("          -a -b BIN_FILE -v VHD_FILE [-l LBA_START]              append BIN file to VHD\n");
		printf("          -i -v VHD_FILE                                         get VHD file information\n");
	} else if(args.type == 0x10) {
		if(create_vhd(args.size, args.vhd)) {
			printf("create vhd success\n");
			if(args.bin != NULL) {
				if(copy_bin(args.lba_start, args.bin, args.vhd)) {
					printf("write vhd success\n");
				} else {
					printf("write vhd error\n");
				}
			}
		} else {
			printf("create vhd error\n");
		}
	} else if(args.type == 0x11) {
		if(copy_bin(args.lba_start, args.bin, args.vhd)) {
			printf("write vhd success\n");
		} else {
			printf("write vhd error\n");
		}
	} else if(args.type == 0x12){
		FILE *infile = fopen(args.vhd, "rb");
		vhd_ftr footer;
		if(infile) {
			fseek(infile, -512L, SEEK_END);
			fread(&footer, sizeof(vhd_ftr), 1, infile);
			if(check_vhd_ftr(footer)) {
				display_info(footer);
			} else {
				printf("vhdhelper: vhd file damaged\n");
			}
		} else {
			printf("vhdhelper: vhd file read error\n");
		}
		fclose(infile);
	}
	
	return 0;
}

int copy_bin(int start, char *bin, char *vhd) {
	FILE *_bin = fopen(bin,"rb");
	FILE *_vhd = fopen(vhd,"rb+");
	if(_bin != NULL && _vhd != NULL) {
		uint8_t tmp_byte;
		fseek(_bin, 0, SEEK_SET);
		fseek(_vhd, 512L * start, SEEK_SET);
		fread(&tmp_byte, 1, 1, _bin);
		while(!feof(_bin)) {
			fwrite(&tmp_byte, 1, 1, _vhd);
			fread(&tmp_byte, 1, 1, _bin);
		}
		fclose(_bin);
		fclose(_vhd);
		return 1;
	} else {
		return 0;
	}
}

void display_info(vhd_ftr footer) {
	printf("Creator:        %4s\n", footer.crtr_app);
	printf("Creator OS:     %4s\n", footer.crtr_os);
	printf("Time Stamp:     %d\n", REVERSE_BYTES_U32(footer.timestamp) + HD_TIMESTAMP_START);
	printf("Original Size:  %llu MB\n", REVERSE_BYTES_U64(footer.orig_size)/1024/1024);
	printf("Current Size:   %llu MB\n", REVERSE_BYTES_U64(footer.curr_size)/1024/1024);
	printf("Disk Geometry:  C%d, H%d, S%d\n", GEO_GET_C(footer.geometry), GEO_GET_H(footer.geometry), GEO_GET_S(footer.geometry));
	printf("Disk Type:      %d\n", REVERSE_BYTES_U32(footer.type));
}

int create_vhd(int size, char *vhd) {
	FILE *_vhd = fopen(vhd, "wb+");
	if(_vhd != NULL) {
		vhd_ftr footer = construct_ftr(size);
		uint8_t *zero = (uint8_t*)calloc(MB_SIZE, sizeof(uint8_t));
		while(size--) {
			fwrite(zero, MB_SIZE, 1, _vhd);
		}
		fwrite(&footer, sizeof(footer), 1, _vhd);
		fclose(_vhd);
		return 1;
	} else {
		return 0;
	}
}

vhd_ftr construct_ftr(int size) {
	vhd_ftr footer = {0};
	memcpy(footer.cookie, HD_COOKIES, 8);
	memcpy(footer.crtr_app, HD_CREATOR_APP, 4);
	memcpy(footer.crtr_os, HD_CREATOR_OS_WIN, 4);
	footer.features = REVERSE_BYTES_U32(HD_RESERVED);
	footer.ff_version = REVERSE_BYTES_U32(HD_FILE_FORMAT_VER);
	footer.data_offset = REVERSE_BYTES_U64(HD_FIXED_OFFSET);
	footer.timestamp = REVERSE_BYTES_U32((uint32_t)(time(NULL) - HD_TIMESTAMP_START));
	footer.crtr_ver = REVERSE_BYTES_U32(HD_CREATOR_VER);
	footer.orig_size = REVERSE_BYTES_U64((uint64_t)size * 1024 *1024);
	footer.curr_size = REVERSE_BYTES_U64((uint64_t)size * 1024 *1024);
	footer.geometry = REVERSE_BYTES_U32(generate_geo((uint64_t)size * 1024 *1024));
	footer.type = REVERSE_BYTES_U32(HD_TYPE_FIXED);
	footer.uuid = uuid_get_uuid();
	footer.checksum = REVERSE_BYTES_U32(get_vhd_checksum(footer));
	return footer;
}



int check_vhd_ftr(vhd_ftr footer) {
	if(get_vhd_checksum(footer) == REVERSE_BYTES_U32(footer.checksum)){
		return 1;
	} else {
		return 0;
	}
}

uint32_t get_vhd_checksum(vhd_ftr footer) {
	uint32_t sum = 0;
	uint8_t *p = (uint8_t*)&footer;
	footer.checksum = 0;
	for(int i = 0; i < sizeof(footer); i++){
		sum += *(p + i);
	}
	return ~sum;
}

uint32_t generate_geo(uint64_t size) {
	uint32_t total_sec = (uint32_t)(size / 512);
	uint32_t sec_per_track, heads, cylinders, cyltimesheads;
	if(total_sec > 65535 * 16 * 255) {
		total_sec = 65535 * 16 * 255;
	}
	if(total_sec >= 65535 * 16 * 63) {
		sec_per_track = 255;
		heads = 16;
		cyltimesheads = total_sec / sec_per_track;
	} else {
		sec_per_track = 17;
		cyltimesheads = total_sec / sec_per_track;
		heads = (cyltimesheads + 1023) / 1024;
		if(heads < 4) {
			heads = 4;
		}
		if(cyltimesheads >= heads * 1024 || heads > 16) {
			sec_per_track = 31;
			heads = 16;
			cyltimesheads = total_sec / sec_per_track;
		}
		if(cyltimesheads >= heads * 1024) {
			sec_per_track = 63;
			heads = 16;
			cyltimesheads = total_sec / sec_per_track;
		}
	}
	cylinders = cyltimesheads / heads;
	return (cylinders << 16) | (heads << 8) | sec_per_track;
}
