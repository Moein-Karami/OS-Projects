#include "Image.hpp"

void* smooth_color_row(void* argument)
{
	PicTableCell arg = *((PicTableCell*)argument);
	int color = arg.color;
	int i = arg.row;
	int nxt = 1 - turn;
	int cnt = 0;
	int sum = 0;
	for (int j = 0; j < cols; j++)
	{
		sum = 0;
		cnt = 0;
		for (int delta_x = -1; delta_x <= 1; delta_x++)
		{
			for (int delta_y = - 1; delta_y <= 1; delta_y++)
			{	
				if (i + delta_x >= 0 && i + delta_x < rows && j + delta_y < cols && j + delta_y >= 0)
					cnt++, sum += pixels[turn][color][i + delta_x][j + delta_y];
			}
		}
		pixels[1 - turn][color][i][j] = sum / cnt;
	}
	pthread_exit(NULL);
}

void* smooth_color(void* arg)
{
	long color = (long)arg;
	pthread_t threads[rows];
	PicTableCell args[rows];
	int return_code;

	for (int i = 0; i < rows; i++)
	{
		args[i].color = color;
		args[i].row = i;
		return_code = pthread_create(&threads[i], NULL, smooth_color_row, &args[i]);

		if (return_code)
		{
			std::cout << "Error in creat thread for smooth_color_row" << std::endl;
			exit(-1);
		}
	}

	for (int i = 0; i < rows; i++)
	{
		return_code = pthread_join(threads[i], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from smooth color row" << std::endl;
			exit(-1);
		}
	}
	pthread_exit(NULL);
}

void smooth()
{
	pthread_t threads[3];
	int return_code;

	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_create(&threads[color], NULL, smooth_color, (void*)color);
		if (return_code)
		{
			std::cout << "Error in create thread for smooth_color" << std::endl;
			exit(-1);
		}
	}
	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_join(threads[color], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from smooth color" << std::endl;
			exit(-1);
		}
	}
	turn = 1 - turn;
}