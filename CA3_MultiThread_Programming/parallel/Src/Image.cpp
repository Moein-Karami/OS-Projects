#include "Image.hpp"
#include <iostream>
#include <pthread.h>
typedef std::pair<int, int> pii;

void* read_pixels_from_row(void* arg)
{
	pii limits = *((pii*)arg);
	int start = limits.first;
	int end = limits.second;
	int count = start * (cols % 4) + 1;
	count += start * cols * 3;

	for (int i = start; i < end; i++)
	{
		count += (cols % 4);
		for (int j = cols - 1; j >= 0; j--)
		{
			for (int k = 0; k < 3; k ++)
			{
				pixels[turn][k][i][j] = file_buffer[buffer_size - count];
				count ++;
			}
		}
	}
	pthread_exit(NULL);
}

void read_pixels()
{
	pthread_t threads[4];
	int return_code;

	pii args[4] = {pii(0, rows / 4), pii(rows / 4, rows / 2), pii(rows / 2, (3 * rows) / 4), pii((3 * rows) / 4, rows)};

	for (int i = 0; i < 4; i++)
	{
		return_code = pthread_create(&threads[i], NULL, read_pixels_from_row, (void*)&args[i]);

		if (return_code)
		{
			std::cout << "Error in creat thread for read pixels from row" << std::endl;
			exit(-1);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		return_code = pthread_join(threads[i], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from  read pixels from row" << std::endl;
			exit(-1);
		}
	}
}

void* export_pixels_from_row(void* arg)
{
	pii limits = *((pii*)arg);
	int start = limits.first;
	int end = limits.second;
	int count = start * (cols % 4) + 1;
	count += start * cols * 3;

	int extra = cols % 4;
	for (int i = start; i < end; i++)
	{
		count += extra;
		for (int j = cols - 1; j >= 0; j--)
		{
			for (int color = 0; color < 3; color++)
			{
				file_buffer[buffer_size - count] = pixels[turn][color][i][j];
				count++;
			}
		}
	}
}

void export_image(std::string file_path)
{
	pthread_t threads[4];
	int return_code;

	pii args[4] = {pii(0, rows / 4), pii(rows / 4, rows / 2), pii(rows / 2, (3 * rows) / 4), pii((3 * rows) / 4, rows)};

	for (int i = 0; i < 4; i++)
	{
		return_code = pthread_create(&threads[i], NULL, export_pixels_from_row, (void*)&args[i]);

		if (return_code)
		{
			std::cout << "Error in creat thread for export pixels from row" << std::endl;
			exit(-1);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		return_code = pthread_join(threads[i], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from export pixels from row" << std::endl;
			exit(-1);
		}
	}

	std::ofstream out(file_path);
	if (!out)
	{
		std::cout << "Fialed to write" << file_path << std::endl;
		return;
	}
	out.write(file_buffer, buffer_size);
	out.close();
}