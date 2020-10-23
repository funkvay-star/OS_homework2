#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "There is some Error\n";
		return -1;
	}

	int file = open(argv[1], O_RDONLY);
	

	//char* file1 = argv[1];
	//char* file2 = argv[2];

	if(file == -1)
	{
		std::cout << "I couldn't open file \n";
		exit(errno);//return 0;
	}

	int write_in_file = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
	if(write_in_file == -1)
	{
		std::cout << "I couldn't open file  \n";
		exit(errno);//return 0;
	}

	long bytes = 0, hole = 0, data = 0, offset = 0;

	long source_physical = 0, destination_physical = 0, source_logical = 0, destination_logical = 0;

	char* buffer = new char[0];
	long buffer_size = 0;

	while(true)
	{
		hole = lseek(file, offset, SEEK_HOLE);
		source_logical  += hole - data;
		
		offset = hole;

		//std::cout << hole << " " << data << "\n";
		
		buffer_size = hole - data;
		buffer = new char[buffer_size];

		lseek(file, -buffer_size, SEEK_CUR);
		read(file, buffer, buffer_size); ///////////////
		
		destination_logical += write(write_in_file, buffer, buffer_size);
		


		data = lseek(file, offset, SEEK_DATA);
		
		
		if(data == -1)
		{
			if(errno == ENXIO)
			{
				data  = lseek(file, 0, SEEK_END);
				//source_physical += data - hole;

				
				//destination_physical += lseek(write_in_file, data - hole, SEEK_END);
				lseek(write_in_file, data - hole, SEEK_END);
				destination_physical += data - hole;
				
				break;
			}
			else
			{
				std::cout << "There is some error\n";
				return -1;
			}
		}
		source_physical += data - hole;
		
		offset = data;
		lseek(write_in_file, data - hole, SEEK_END);
		destination_physical += data - hole;
		//SEEK_DATA

		//SEEK_HOLE
	}
	std::cout << "Source physical: " << source_physical << ", Source logical: " << source_logical;
	std::cout << ", Destination physical: " << destination_physical;
	std::cout << ", Destination logical: " << destination_logical;
}
