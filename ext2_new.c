#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<linux/fs.h>
#include<ext2fs/ext2_fs.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>


unsigned int block_size;


#define BASE_OFFSET 1024
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)

void read_inode(int* fd, struct ext2_super_block *super, struct ext2_group_desc *group, long inode_num){
	struct ext2_inode inode;
	struct ext2_dir_entry_2 *entry;
	unsigned int size;
	unsigned char block[block_size];

	lseek(*fd, (group->bg_inode_table * block_size) + (inode_num - 1)*sizeof(struct ext2_inode), SEEK_SET);
	read(*fd, &inode, sizeof(struct ext2_inode));
	printf("uid : %u size: %u blocks : %u first block: %u\n", inode.i_uid, inode.i_size, inode.i_blocks, inode.i_block[0]);
	return;
}


int main(int argc, char* argv[]){

	int fd;
	struct ext2_super_block sb;
	struct ext2_group_desc bgdesc;
	struct ext2_inode inode;
	struct ext2_dir_entry_2 dirent;
	
	
	//opening the drive
	fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		perror("readsuper:");
		exit(errno);
	}
	
	//reading the super block
	lseek(fd, BASE_OFFSET, SEEK_CUR);
	read(fd, &sb, sizeof(struct ext2_super_block));
	
	//if(sb.s_magic != EXT2_MAGIC){
	//	
	//}
	
	//Reading block descriptor
	block_size = BASE_OFFSET << sb.s_log_block_size;
	lseek(fd, block_size, SEEK_SET);
	read(fd, &bgdesc, sizeof(struct ext2_group_desc));
	printf("Inode Table: %d\n", bgdesc.bg_inode_table);
	printf("Block Size : %d\n", block_size);
	
	//reading inode 
	lseek(fd, (bgdesc.bg_inode_table * block_size) + 2*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, &inode, sizeof(struct ext2_inode));
	printf("uid = %u, size = %u, blocks = %u, first block = %u\n", inode.i_uid, inode.i_size, inode.i_blocks, inode.i_block[0]);
	
	lseek(fd, inode.i_block[0]*block_size, SEEK_SET);
	read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	printf("Inode no : %u Name: %s File type: %d\n", dirent.inode, dirent.name, dirent.file_type);
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	printf("Inode no: %u, Name: %s  File type: %d\n", dirent.inode, dirent.name, dirent.file_type);
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	printf("Inode no: %u, Name: %s  File type: %d\n", dirent.inode, dirent.name, dirent.file_type);
	lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	printf("Inode no: %u Name: %s  File type: %d\n", dirent.inode, dirent.name, dirent.file_type);
	
	printf("\n\nInside the read_inode\n");

	read_inode(&fd, &sb, &bgdesc, 3);
	//int inode_num = dirent.inode;
	//Trying to read not available dirs
	//lseek(fd, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	//read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
	//printf("Inode no: %u, Name: %s \n", dirent.inode, dirent.name);
	
	
	//long group_no, local_inode_num;
	//struct ext2_group_desc gdesc;
	//group_no = (long)(inode_num - 1)/(sb.s_inodes_per_group);
	//lseek(fd, (long)block_size + ((long)group_no * (sizeof(struct ext2_group_desc))), SEEK_SET);
	//read(fd, &gdesc, sizeof(struct ext2_group_desc));
	//local_inode_num = (long)(inode_num -1)%(sb.s_inodes_per_group);
	//lseek(fd, ((long)gdesc.bg_inode_table * block_size) + ((long)local_inode_num * (sb.s_inode_size)), SEEK_SET);
	//read(fd, &inode, sizeof(struct ext2_inode));
	
	//printf("Mode: %d Size : %d Access Time: %d", inode.i_mode, inode.i_size, inode.i_atime);
	/*
	read_inode(fd, &sb, &bgdesc, 2, &inode);
	for(int i = 0; i<EXT2_N_BLOCKS; i++){
		if(i < EXT2_NDIR_BLOCKS)
			printf("Block %2u : %u\n", i, inode.i_block[i]);
		else if(i== EXT2_IND_BLOCK)
			printf("Single : %u\n", inode.i_block[i]);
		else if(i == EXT2_DIND_BLOCK)
			printf("Double : %u\n", inode.i_block[i]);
		else if(i == EXT2_TIND_BLOCK)
			printf("Tripel : %u\n", inode.i_block[i]);
	}	
	*/
	/*int inode_no = dirent.inode;
	long num_inodes_per_group = sb.s_inodes_per_group;
	int group_no = inode_no/sb.s_inodes_per_group;
	int inode_off = inode_no - (group_no * num_inodes_per_group) - 1;
	 
	lseek(fd, BLOCK_OFFSET(bgdesc.bg_inode_table) + inode_off * sizeof(struct ext2_inode), SEEK_SET);
	read(fd, &inode, sizeof(struct ext2_inode));	
	printf("Mode: %d Size : %d Access Time: %d", inode.i_mode, inode.i_size, inode.i_atime);
	*/
	/*
	struct ext2_dir_entry_2 *entry;
	unsigned int size;
	unsigned char block[block_size];
	
	lseek(fd, BLOCK_OFFSET[inode->i_block[0]), SEEK_SET);
	read(fd, block, block_size);
	size = 0;
	
	entry = (struct ext2_dir_entry_2 *) block;
	while(size < inode->i_size){
		char file_name[EXT2_NAME_LEN + 1];
		memcpy(file_name, entry->name, entry->name_len);
		file_name[entry->name_len] = '\0';
		printf("%d %s\n", entry->inode, file_name);
		entry = (void*) entry + entry->rec_len;
		size += entry->rec_len;
	}
	*/
	printf("Inode reading complete\n");
	close(fd);
	return 0;
}





