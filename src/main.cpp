#include <iostream>
#include <string>

#include "create.h"
#include "copyto.h"
#include "copyfrom.h"
#include "ls.h"
#include "rm.h"
#include "info.h"

int main(int argc, char *argv[]) {
	std::string filesystemName;
	if (argc <= 1) {
		std::cout << "Please enter the name of the filesystem\n";
		std::cin >> filesystemName;
	}
	else {
		filesystemName = argv[1];
	}

	bool isInteractive = (argc <= 1);

	int choice;
	do {
		if (argc <= 2)  {
			std::cout << "1 - createFilesystem\n2 - copyFileTo\n3 - copyFileFrom\n4 - ls\n5 - rm\n6 - info\n7 - deleteFilesystem\n";
			if (isInteractive)
				std::cout << "8 - exit\n";

			std::cin >> choice;
		}
		else {
			choice = std::stoi(argv[2]);
		}

		if (choice == 1) {
			int size;

			if (argc <= 3) {
				std::cout << "Please input the size of the file system in Bytes \n";
				std::cin >> size;
			}
			else {
				size = std::stoi(argv[3]);
			}

			createFilesystem(size, filesystemName);
		}

		FILE *tempFile = fopen(filesystemName.c_str(), "r");
		if (tempFile == NULL) {
			std::cout << "Given filesystem does not exist" << std::endl;
			return 1;
		}
		fclose(tempFile);

		if (choice == 2) {
			std::string fileName;
			std::string newFileName;

			if (argc <= 3) {
				std::cout << "Please input the name of the file you wish to copy as well as its new filename\n";
				std::cin >> fileName >> newFileName;
			} 
			else if (argc <= 4) {
				fileName = argv[3];
				newFileName = argv[3];
			}
			else {
				fileName = argv[3];
				newFileName = argv[4];
			}
            std::cout << fileName << " " << newFileName << std::endl;

			FILE *file;
			FILE *fileSystem;

			file = fopen(fileName.c_str(), "r");
			fileSystem = fopen(filesystemName.c_str(), "rb+");

			copyTo(fileSystem, file, newFileName);

			fclose(fileSystem);
			fclose(file);
		}	
		if (choice == 3) {
			std::string fileName;
			std::string virtualFileName;

			FILE *fileSystem;
			fileSystem = fopen(filesystemName.c_str(), "r");

			if (argc <= 3) {
				std::cout << "Please input the name of the file you wish to copy from filesystem, as well the new filename:\n";
				ls(fileSystem);
				std::cin >> virtualFileName >> fileName;
			}
			else if (argc <= 4) {
				virtualFileName = argv[3];
				fileName = argv[3];
			}
			else {
				virtualFileName = argv[3];
				fileName = argv[4];
			}



			FILE *file;
			file = fopen(fileName.c_str(), "wb+");

			copyFrom(fileSystem, file, virtualFileName);

			fclose(fileSystem);
			fclose(file);
		}
		if (choice == 4) {
			FILE *fileSystem;
			fileSystem = fopen(filesystemName.c_str(), "r");

			ls(fileSystem);

			fclose(fileSystem);
		}
		if (choice == 5) {
			FILE *fileSystem;

			fileSystem = fopen(filesystemName.c_str(), "rb+");

			std::string fileName;
			
			if(argc <= 3) {
				std::cout << "Please input the name of the file you wish to delete:\n";
				ls(fileSystem);
				std::cin >> fileName;
			}
			else {
				fileName = argv[3];
			}

			rm(fileSystem, fileName);

			fclose(fileSystem);
		}
		if (choice == 6) {
			FILE *fileSystem;

			fileSystem = fopen(filesystemName.c_str(), "r");

			info(fileSystem);
		}
		if (choice == 7) {
			remove(filesystemName.c_str());
		}

		if(isInteractive)
			std::cout << "\n\n\n";
	} while(isInteractive && choice != 8);

	return 0;
}
