#include "Image.hpp"
#include <iostream>
#include <pthread.h>


void* read_pixels_from_row(void* arg)
{
	long row = (long)arg;
	int count = (rows - row) * (cols % 4);
	count += (rows - row - 1) * cols * 3;

	for (int j = cols - 1; j >= 0; j--)
	{
		for (int k = 0; k < 3; k ++)
		{
			pixels[turn][k][row][j] = file_buffer[buffer_size - count];
			count ++;
		}
	}
}

void read_pixels()
{
	int count = 1;
	pthread_t threads[rows];
	int return_code;

	for (int i = 0; i < rows; i++)
	{
		return_code = pthread_create(&threads[i], NULL, read_pixels_from_row, (void*)i);

		if (return_code)
		{
			std::cout << "Error in creat thread for read pixels from row" << std::endl;
			exit(-1);
		}
	}

	for (int i = 0; i < rows; i++)
	{
		return_code = pthread_join(threads[i], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from  read pixels from row" << std::endl;
			exit(-1);
		}
	}
}

void average()
{
	long long int avg[] = {0, 0, 0};
	for (int color = 0; color < 3; color++)
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				avg[color] += pixels[turn][color][i][j];
	for (int i = 0; i < 3; i++)
		avg[i] /= (long long)((long long)rows * (long long)cols);

	for (int color = 0; color < 3; color++)
		for (int i = 0 ; i < rows; i++)
			for (int j = 0; j < cols; j++)
				pixels[turn][color][i][j] = 0.4 * (float)pixels[turn][color][i][j] + 0.6 * (double)avg[color];
}

void cross()
{
	int j;
	for (int color = 0 ; color < 3; color++)
	{
		for (int i = 0; i < rows; i++)
		{
			j = cols - i - 1;
			for (int delta = -1; delta <= 1; delta++)
			{
				if (i + delta < cols && i + delta >= 0)
					pixels[turn][color][i][i + delta] = 255;
				if (j + delta < cols && j + delta >= 0)
					pixels[turn][color][i][j + delta] = 255;
			}
		}
	}
}

void export_image(char* file_buffer, std::string file_path, int buffer_size)
{
	std::ofstream out(file_path);
	if (!out)
	{
		std::cout << "Fialed to write" << file_path << std::endl;
		return;
	}
	int count = 1;
	int extra = cols % 4;
	for (int i = 0; i < rows; i++)
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
	out.write(file_buffer, buffer_size);
	out.close();
}