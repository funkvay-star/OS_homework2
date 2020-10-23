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
	


	//if there is file = -1 then there is a  problem and we can't open file  
	if(file == -1)
	{
		std::cout << "I couldn't open file \n";
		exit(errno);
	}

	//if there is write_in_file = -1 then there is a problem and we can't open file
	int write_in_file = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
	if(write_in_file == -1)
	{
		std::cout << "I couldn't open file  \n";
		exit(errno);//return 0;
	}


	long hole = 0, data = 0, offset = 0;

	long source_physical = 0, destination_physical = 0, source_logical = 0, destination_logical = 0;

	char* buffer = new char[0];
	long buffer_size = 0;

	while(true)
	{
		//we find where starts hole
		hole = lseek(file, offset, SEEK_HOLE);
		source_physical  += hole - data;

		//offset = from 0 to the hole start
		offset = hole;

		//create string
		buffer_size = hole - data;
		buffer = new char[buffer_size];


		//we write in buffer data that is in file and then rewrite in write_in_file
		lseek(file, -buffer_size, SEEK_CUR);
		read(file, buffer, buffer_size);
		destination_physical += write(write_in_file, buffer, buffer_size);
		


		data = lseek(file, offset, SEEK_DATA);
		
		//if data = -1 then there is a problem with getting data from file
		if(data == -1)
		{
			//if errno = ENXIO then there is no more data that we can find
			if(errno == ENXIO)
			{
				//data = end file
				data  = lseek(file, 0, SEEK_END);

				//in write_in_file we write holes from file
				lseek(write_in_file, data - hole, SEEK_END);
				destination_logical += data - hole;
				
				break;
			}

			//if errno != ENXIO then there is an another problem so we stop code
			else
			{
				std::cout << "There is some error\n";
				return -1;
			}
		}
		source_logical += data - hole;
		
		offset = data;
		lseek(write_in_file, data - hole, SEEK_END);
		destination_logical += data - hole;
	}
	std::cout << "Source physical: " << source_physical << ", Source logical: " << source_logical;
	std::cout << ", Destination physical: " << destination_physical;
	std::cout << ", Destination logical: " << destination_logical;
}
