#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<linux/fs.h>
#include<ext2fs/ext2_fs.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>

#define BASE_OFFSET 1024

char Dest[2048][100];
strcpy(Dest[0], "/");

void parse_the_path(char *path){
	int i = 1, j=0;
	// char 
	// for()
}



int main(int argc, char* argv[]){
	int fd;

	struct ext2_super_block super;
	struct ext2_group_desc bgdesc;
	struct ext2_inode inode;
	struct ext2_dir_entry_2 dirent;
	unsigned int block_size;

	fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		perror("Readsuper:");
		exit(erron);
	}

	int i, j, k;
	char str[100];
	char dest[200];
	//printf("%s = argv", argv[2]);
	strcpy(dest, argv[2]);
	//printf("%s = dest\n", dest);
	j = strlen(dest);
	printf("The lenght of string is %d\n", j);
	k=0;
	// for(i=1; i<j; i++){
	// 	if(dest[i] != "/"){
	// 		str[k] = dest[i];
	// 		k++;
	// 	}else{
	// 		str[k] = '\0';
	// 		strcpy(Dest[i])
	// 	}
	// }
	printf("The size of super_block is %lu\n",sizeof(struct ext2_super_block) );
	printf("The size of BG DESC = %lu\n", sizeof(struct ext2_group_desc));

	lseek(fd, BASE_OFFSET, SEEK_CUR);
	read(fd, &super, sizeof(struct ext2_super_block));

	printf("Magic : %x\n", super.s_magic);
	printf("Inodes count: %d\n", super.s_inodes_count);
	printf("Block size : %d\n", super.s_log_block_size);

	block_size = BASE_OFFSET << super.s_log_block_size;
	printf("The block size is : %d\n", block_size);

	return 0;
}








