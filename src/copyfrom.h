#include <string>

long findINodeName(FILE*, std::string);
long getOffsetFromDatablockNum(int);

void copyFrom(FILE* filesystem, FILE* file, std::string virtualFileName) {
	long inodeOffset = findINodeName(filesystem, virtualFileName);
	
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
	long writeFileOffset = 0;
	do {
		fseek(filesystem, datablockOffset, SEEK_SET);
		fread(&datablock, sizeof(struct Datablock), 1, filesystem);
		
		fseek(file, writeFileOffset, SEEK_SET);
		int toRead = 512;
		if (fileSize < toRead)
			toRead = fileSize;
		fwrite(datablock.data, 1, toRead, file);
		fileSize -= toRead;

		datablockOffset = getOffsetFromDatablockNum(datablock.next);	
		writeFileOffset += BLOCK_SIZE;
	} while (datablock.next != -1);
		
}

long findINodeName(FILE* filesystem, std::string filename) {
	long offset = sizeof(struct Superblock);
	
	long inodeOffset = 0;	
	for(int i = 0 ; i < INODE_COUNT ; i++) {
		fseek(filesystem, offset + inodeOffset, SEEK_SET);
		INode inode;
		
		fread(&inode, sizeof(struct INode), 1, filesystem);
		
		if (strcmp(inode.filename, filename.c_str()) == 0) {
			return offset + inodeOffset;
		}

		inodeOffset += sizeof(struct INode);
	}

	return -1;
}

long getOffsetFromDatablockNum(int datablockNum) {
	return sizeof(struct Superblock) + sizeof(struct INode) * INODE_COUNT + datablockNum * sizeof(struct Datablock);
}
