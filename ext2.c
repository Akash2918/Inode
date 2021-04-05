#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<linux/fs.h>
#include<ext2fs/ext2_fs.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>


#define BASE_OFFSET 1024

int main(int argc, char *argv[]){
	// printf("value of argc %d\n", argc);
	// if(argc != 4){
	// 	printf("\nWrong input format\n\n");
	// 	printf("Please specify the input in the following format\n");
	// 	printf("./a.out\t device-file-name\t path-on-partition\t inode/data\n\n");
	// 	exit(errno);
	// }

	// int fd = open(argv[1], O_RDONLY);

	// close(fd);
	// printf("Hello world\n");
	int fd = open(argv[1], O_RDONLY);
	int count;
	struct ext2_super_block sb;
	struct ext2_group_desc bgdesc;
	struct ext2_inode inode;
	struct ext2_dir_entry_2 dirent;
	unsigned int block_size;
	if(fd == -1){
		perror("readsuper:");
		exit(errno);
	}
	printf("Size of super block = %lu\n", sizeof(struct ext2_super_block));
	printf("size of BG DESC = %lu\n", sizeof(struct ext2_group_desc));

	lseek64(fd, 1024, SEEK_CUR);
	count = read(fd, &sb, sizeof(struct ext2_super_block));
	printf("Magic: %x\n", sb.s_magic);
	printf("Inodes Count: %d\n", sb.s_inodes_count);
	//printf("Blocks Count: %d\n", sb.s_log_block_count);
	printf("Blocks Size: %d\n", sb.s_log_block_size);
	block_size = 1024 << sb.s_log_block_size;
	printf("Last mounted : %s\n", sb.s_last_mounted);
	lseek64(fd, block_size, SEEK_SET);
	count = read(fd, &bgdesc, sizeof(struct ext2_group_desc));
	printf("Inode Table : %d\n", bgdesc.bg_inode_table);
	printf("Block Size: %d\n", block_size);
	lseek64(fd, (bgdesc.bg_inode_table * block_size) + 2*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, &inode, sizeof(struct ext2_inode));
	printf("uid = %u size = %u blocks = %u first_block = %u\n",
		inode.i_uid,
		inode.i_size,
		inode.i_blocks,
		inode.i_block[0]);
	lseek64(fd, inode.i_block[0]*block_size, SEEK_SET);
	read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	printf("Inode no: %u Name : %s \n", dirent.inode, dirent.name);
	lseek64(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	printf("Inode no: %u Name:%s \n", dirent.inode, dirent.name);
	lseek64(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	printf("Inode no: %u Name:%s \n", dirent.inode, dirent.name);
	lseek64(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	printf("Inode no: %u Name:%s \n", dirent.inode, dirent.name);

	close(fd); 
	return 0;
}


