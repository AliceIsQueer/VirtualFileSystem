#include <stdio.h>
#include <iostream>

long getFreeDatablockOffset(FILE*, int, bool);
long getFreeINodeOffset(FILE*);
long getFileSize(FILE*);
int getDatablockNum(int);

void copyTo(FILE *filesystem, FILE *file, std::string fileName) {
	Superblock superblock;
	fseek(filesystem, 0, SEEK_SET);
	fread(&superblock, sizeof(struct Superblock), 1, filesystem);


	fseek(filesystem, sizeof(struct Superblock), SEEK_SET); // Move to where Inodes are

	long firstINode = getFreeINodeOffset(filesystem);
	INode inode;
	fseek(filesystem, firstINode, SEEK_SET);
	fread(&inode, sizeof(struct INode), 1, filesystem);
	
	long fileSize = getFileSize(file);
	if(fileSize > superblock.freeDatablockCount * BLOCK_SIZE) {
		std::cout << "Not enough space left" << std::endl;
		return;
	}

	long firstDatablock = getFreeDatablockOffset(filesystem, superblock.datablockCount, false);
	int datablockNum = getDatablockNum(firstDatablock);

	inode.firstBlock = datablockNum;
	inode.fileSize = fileSize;
	strcpy(inode.filename, fileName.c_str());
	fseek(filesystem, firstINode, SEEK_SET);
	fwrite(&inode, sizeof(struct INode), 1, filesystem);

	long bytesWritten = 0;


	fseek(file, 0, SEEK_SET);
	fseek(filesystem, firstDatablock, SEEK_SET);
	long nextDatablockOffset;
	while(bytesWritten < fileSize) {
		Datablock datablock;
		int toRead = BLOCK_SIZE;
		if(fileSize - bytesWritten < BLOCK_SIZE)
			toRead = fileSize - bytesWritten;
		memset(datablock.data, 0, BLOCK_SIZE);
		fread(datablock.data, 1, toRead, file);

		nextDatablockOffset = getFreeDatablockOffset(filesystem, superblock.datablockCount, true);   // Needed to know the next datablock's number in advance, before write;

		datablock.next = getDatablockNum(nextDatablockOffset);

		if (bytesWritten + BLOCK_SIZE >= fileSize) {// If this is the last datablock,set to -1 
			datablock.next = -1;
		}


		fseek(filesystem, firstDatablock, SEEK_SET);
		fwrite(&datablock, sizeof(struct Datablock), 1, filesystem);
		superblock.freeDatablockCount--;
		firstDatablock = nextDatablockOffset;

		bytesWritten += BLOCK_SIZE;
		fseek(file, bytesWritten, SEEK_SET);
	}
	
	fseek(filesystem, 0, SEEK_SET);
	fwrite(&superblock, sizeof(struct Superblock), 1, filesystem); //Update superblock with new free amount of datablocks;
}

long getFreeDatablockOffset(FILE *filesystem, int datablock_count, bool next) {
	long offset = sizeof(struct Superblock) + INODE_COUNT * sizeof(struct INode);
	fseek(filesystem, offset, SEEK_SET);

	Datablock datablock;
	bool checkNext = next;

	long datablockOffset = 0;
	next = false;
	for(int i = 0 ; i < datablock_count; i++) {
		fseek(filesystem, offset + datablockOffset, SEEK_SET);
		fread(&datablock, sizeof(struct Datablock), 1, filesystem);
		if (datablock.next == -2) {
			if(checkNext) 
				checkNext = false;
			else
				return offset + datablockOffset;
		}
		datablockOffset += sizeof(struct Datablock);
	}
	return -1;
}

int getDatablockNum(int datablockOffset) {
	return (datablockOffset - sizeof(struct Superblock) - sizeof(struct INode) * INODE_COUNT) / BLOCK_SIZE;
}

long getFreeINodeOffset(FILE *filesystem) {
	long offset = sizeof(struct Superblock);
	fseek(filesystem, offset, SEEK_SET);
	
	INode inode;

	long inodeOffset = 0;
	for(int i = 0 ; i < INODE_COUNT; i++) {
		fseek(filesystem, offset + inodeOffset, SEEK_SET);
		fread(&inode, sizeof(struct INode), 1, filesystem);
		if (inode.firstBlock == -1)
			return offset + inodeOffset;
		inodeOffset += sizeof(struct INode);
	}
	return -1;
}

long getFileSize(FILE *file) {
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}
