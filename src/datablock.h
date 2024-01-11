#include <string>

#define BLOCK_SIZE 512

struct Datablock {
	int next; //Index of the next datablock the file occupies, -1 if last elem, -2 if unused
	char data[BLOCK_SIZE];	
};
