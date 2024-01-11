void ls(FILE* filesystem) {
	INode inodes[INODE_COUNT];

	fseek(filesystem, sizeof(struct Superblock), SEEK_SET);
	fread(inodes, sizeof(struct INode), INODE_COUNT, filesystem);

	for(int i = 0 ; i < INODE_COUNT ; i++) {
		if (inodes[i].firstBlock != -1)
			std::cout << inodes[i].filename << std::endl;
	}
}
