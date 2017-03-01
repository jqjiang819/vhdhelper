#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vhdhelper.h"

hd_ftr foot;
void get_info(char *vhd);
void copy_bin(int start, char *bin, char *vhd);

int main(int argc, char *argv[]) {
	int flag;
	int ch;
	int lba_start=0;
	char *vhd,*bin;
	if(argc==1) {
		printf("error\n");
		return 0;
	}
	while ((ch = getopt(argc, argv, "cl:b:v:i")) != -1) {
		switch (ch) {
			case 'c':
				flag=0x11;
				break;
			case 'l':
				lba_start = atoi(optarg);
                break;
        	case 'b':
        		bin = optarg;
        		break;
        	case 'v':
        		vhd = optarg;
        		break;
        	case 'i':
        		get_info(vhd);
				printf("C/H/S = %d/%d/%d\n",GEO_GET_C(foot.geometry),
								GEO_GET_H(foot.geometry),GEO_GET_S(foot.geometry));
				printf("Size = %d Bytes\n",foot.orig_size);
				break;
		} 
	}
	if(flag==0x11){
		copy_bin(lba_start,bin,vhd);
		printf("Success.\n");
	}
	return 0;
}

void get_info(char *vhd) {
	FILE *infile;
	infile = fopen(vhd,"rb");
	if(infile){
		fseek(infile,-512L,SEEK_END);
		fread(&foot,sizeof(hd_ftr),1,infile);
	}else{
		printf("error\n");
	}
	fclose(infile);
}

void copy_bin(int start, char *bin, char *vhd) {
	FILE *_bin, *_vhd;
	u8 tmp_byte;
	_bin = fopen(bin,"rb");
	_vhd = fopen(vhd,"rb+");
	fseek(_bin,0,SEEK_SET);
	fseek(_vhd,512L*start,SEEK_SET);
	fread(&tmp_byte,1,1,_bin);
	while(!feof(_bin)) {
		fwrite(&tmp_byte,1,1,_vhd);
		fread(&tmp_byte,1,1,_bin);
	}
	fclose(_bin);
	fclose(_vhd);
}


