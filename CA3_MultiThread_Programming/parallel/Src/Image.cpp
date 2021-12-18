#include "../Includes/Image.hpp"
#include <iostream>
#include <pthread.h>

void* allocate_for_row(void* arg)
{
	PicTableCell argument = *((PicTableCell*)arg);

	pixels[argument.turn][argument.color][argument.row] = new unsigned char[cols];
}

void* allocate_for_color(void* arg)
{
	PicTableCell argument = *((PicTableCell*)arg);
	int seri = argument.turn;
	int color = argument.color;
	pixels[seri][color] = new unsigned char*[rows];
	pthread_t threads[rows];
	PicTableCell args[rows];
	int return_code;

	for (int i = 0; i < rows; i++)
	{
		args[i].turn = seri;
		args[i].color = color;
		args[i].row = i;

		pthread_create(&threads[i], NULL, allocate_for_row, &args[i]);

		if (return_code)
		{
			std::cout << "Error in make thread for allocate row" << std::endl;
			exit(-1);
		}
	}
	for (int i = 0; i < rows; i++)
	{
		return_code = pthread_join(threads[i], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from allocate row" << std::endl;
			exit(-1);
		}
	}
}

void* allocate_for_turn(void* collection)
{
	int seri = *((int*)collection);

	pthread_t threads[3];
	int return_code;
	PicTableCell arg[3];

	for (int color = 0; color < 3; color++)
	{
		arg[color].turn = seri;
		arg[color].color = color;

		return_code = pthread_create(&threads[color], NULL, allocate_for_color, &arg[color]);

		if (return_code)
		{
			std::cout << "Error in make thread for allocate color" << std::endl;
			exit(-1);
		}
	}

	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_join(threads[color], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from allocate color" << std::endl;
			exit(-1);
		}
	}
}

void allocate_array()
{
	turn = 0;
	pthread_t threads[2];
	int return_code;
	int arg[] = {0, 1};

	for (int i = 0; i < 2; i++)
	{
		return_code = pthread_create(&threads[i], NULL, allocate_for_turn, &arg[i]);

		if (return_code)
		{
			std::cout << "Error in make thread for allocate turn" << std::endl;
			exit(-1);
		}
	}

	for (int collect = 0; collect < 2; collect++)
	{
		return_code = pthread_join(threads[collect], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from allocate turn" << std::endl;
			exit(-1);
		}
	}
}

void delete_allocated_array()
{
	for (int select = 0; select < 2; select++)
	{
		for (int color = 0; color < 3; color++)
		{
			for (int i = 0; i < rows; i++)
			{
				delete(pixels[select][color][i]);
			}
			delete(pixels[select][color]);
		}
	}
}

void read_pixels(int buffer_size, char* file_buffer)
{
	int count = 1;
	int extra = cols % 4;
	for (int i = 0; i < rows; i++)
	{
		count += extra;
		for (int j = cols - 1; j >= 0; j--)
		{
			for (int k = 0; k < 3; k ++)
			{
				pixels[turn][k][i][j] = file_buffer[buffer_size - count];
				count ++;
			}
		}
	}
}

void smooth()
{
	int nxt = 1 - turn;
	int sum, cnt;
	for (int color = 0; color < 3; color++)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				sum = 0;
				cnt = 0;

				for (int delta_x = -1; delta_x <= 1; delta_x++)
					for (int delta_y = -1; delta_y <= 1; delta_y++)
						if (0 <= i + delta_x && i + delta_x < rows && 0 <= j + delta_y && j + delta_y < cols)
							sum += (int)pixels[turn][color][i + delta_x][j + delta_y], cnt++;
				sum /= cnt;
				pixels[nxt][color][i][j] = sum;
			}
		}
	}
	turn = nxt;
}

void sepia()
{
	int nxt = 1 - turn;
	int val;
	for (int color = 0; color < 3; color++)
	{
		for (int i = 0 ; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				switch (color)
				{
					case RED:
						val = (float)pixels[turn][RED][i][j] * 0.393 + (float)pixels[turn][GREEN][i][j] * 0.769 +
								(float)pixels[turn][BLUE][i][j] * 0.189;
						break;
					case GREEN:
						val = (float)pixels[turn][RED][i][j] * 0.349 + (float)pixels[turn][GREEN][i][j] * 0.686 +
								(float)pixels[turn][BLUE][i][j] * 0.168;
						break;
					case BLUE:
						val = (float)pixels[turn][RED][i][j] * 0.272 + (float)pixels[turn][GREEN][i][j] * 0.534 +
								(float)pixels[turn][BLUE][i][j] * 0.131;
						break;
				}
				pixels[nxt][color][i][j] = (val < 255) ? val : 255;
			}
		}
	}
	turn = nxt;
}

void average()
{
	long long int avg[] = {0, 0, 0};
	for (int color = 0; color < 3; color++)
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < rows; j++)
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