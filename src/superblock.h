#include <string>

#define INODE_COUNT 16

struct Superblock {
	int filesystemSize; // How big is the filesystem in bytes
	int datablockCount; // How many datablocks it has
	int freeDatablockCount; // How many free datablocks are left
	int iNodeCount; // How many files exist in this superblock
	char filesystemName[32];
};

