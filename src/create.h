#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "superblock.h"
#include "datablock.h"
#include "inode.h"

void createFilesystem(int size, std::string name) {
	FILE *fileSystem;
	Superblock superblock;
	strcpy(superblock.filesystemName, name.c_str());
	superblock.filesystemSize = size;
	superblock.datablockCount = size / BLOCK_SIZE;
	superblock.freeDatablockCount = size / BLOCK_SIZE;
	superblock.iNodeCount = INODE_COUNT;
	
	fileSystem = fopen(name.c_str(), "wb");
	
	fseek(fileSystem, 0, SEEK_SET);
	fwrite(&superblock, sizeof(superblock), 1, fileSystem);
	
	for(int i = 0 ; i < INODE_COUNT; i++) {
		INode tempINode;
		tempINode.firstBlock = -1;
		tempINode.fileSize = 0;
		fseek(fileSystem, 0, SEEK_END);
		fwrite(&tempINode, sizeof(INode), 1, fileSystem);
	}


	for(int i = 0 ; i < superblock.datablockCount; i++) {
		fseek(fileSystem, 0, SEEK_END);
		Datablock tempDatablock;
		tempDatablock.next = -2;
		fwrite(&tempDatablock, sizeof(Datablock), 1, fileSystem);
	}

	fclose(fileSystem);
}
