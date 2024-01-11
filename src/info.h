void info(FILE* filesystem) {
	Superblock superblock;
	fseek(filesystem, 0, SEEK_SET);
	fread(&superblock, sizeof(struct Superblock), 1, filesystem);

	INode inodes[INODE_COUNT];
	fread(inodes, sizeof(struct INode), INODE_COUNT, filesystem);

	Datablock datablocks[superblock.datablockCount];
	fread(datablocks, sizeof(struct Datablock), superblock.datablockCount, filesystem);

	std::cout << "\nSUPERBLOCK\n------------------------------\n";
	std::cout << "File System Name : " << superblock.filesystemName << '\n';
	std::cout << "File System Size(B) : " << superblock.filesystemSize << '\n';
	std::cout << "Datablock count : " << superblock.datablockCount << '\n';
	std::cout << "Free datablocks : " << superblock.freeDatablockCount << '\n';
	std::cout << "INode count : " << superblock.iNodeCount << '\n';
	
	std::cout << "\nINODES\n----------------------------------\n";
	for(int i = 0 ; i < INODE_COUNT ; i++) {
		std::cout << "INODE" << i << ":\n";
		std::cout << "Filename : " << inodes[i].filename << '\n';
		std::cout << "File size : " << inodes[i].fileSize << '\n';
		std::cout << "First block : " << inodes[i].firstBlock << (inodes[i].firstBlock == -1 ? " (empty)" : "") << "\n\n";
	}

	std::cout << "\nDATABLOCKS\n------------------------------\n";
	for(int i = 0 ; i < superblock.datablockCount ; i++) {
		std::cout << "DATABLOCK" << i << ":\n";
		std::cout << "Next : " << datablocks[i].next << (datablocks[i].next == -2 ? " (empty)" : "") << '\n';

		char displayData[BLOCK_SIZE+1];
		strncpy(displayData, datablocks[i].data, BLOCK_SIZE);
		displayData[BLOCK_SIZE] = '\0';
		std::cout << "Data : " << displayData << "\n\n\n";
	}
}
