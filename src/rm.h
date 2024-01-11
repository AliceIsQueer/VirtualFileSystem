#include <string>

void rm(FILE* filesystem, std::string fileName) {
	fseek(filesystem, 0, SEEK_SET);
	Superblock superblock;
	fread(&superblock, sizeof(struct Superblock), 1, filesystem);

	long inodeOffset = findINodeName(filesystem, fileName);
	
	if (inodeOffset == -1) {
		std::cout << "No such file exists" << std::endl;
		return;
	}

	INode inode;
	fseek(filesystem, inodeOffset, SEEK_SET);
	fread(&inode, sizeof(struct INode), 1, filesystem);

	Datablock datablock;
	int fileSize = inode.fileSize;
	long datablockOffset = getOffsetFromDatablockNum(inode.firstBlock);
	int rememberNext;
	do {
		fseek(filesystem, datablockOffset, SEEK_SET);
		fread(&datablock, sizeof(struct Datablock), 1, filesystem);
		rememberNext = datablock.next;

		datablock.next = -2;
		memset(datablock.data, 0, BLOCK_SIZE);

		fseek(filesystem, datablockOffset, SEEK_SET);
		fwrite(&datablock, sizeof(struct Datablock), 1, filesystem);
		superblock.freeDatablockCount++;
		
		datablockOffset = getOffsetFromDatablockNum(rememberNext);	


	} while (rememberNext != -1);

	memset(inode.filename, 0, sizeof(inode.filename));
	inode.firstBlock = -1;
	inode.fileSize = 0;
	fseek(filesystem, inodeOffset, SEEK_SET);
	fwrite(&inode, sizeof(struct INode), 1, filesystem);

	fseek(filesystem, 0, SEEK_SET);
	fwrite(&superblock, sizeof(struct Superblock), 1, filesystem);	
}
