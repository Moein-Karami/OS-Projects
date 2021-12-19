#include "Image.hpp"

void* average_color_row(void* argument)
{
	PicTableCell arg = *((PicTableCell*)argument);
	int color = arg.color;
	int i = arg.row;
	int avg = arg.turn;
	int nxt = 1 - turn;

	for (int j = 0; j < cols; j++)
		pixels[turn][color][i][j] = 0.4 * (float)pixels[turn][color][i][j] + 0.6 * (float)avg;
	pthread_exit(NULL);
}

void* average_color(void* arg)
{
	long color = (long)arg;
	long long avg = 0;
	// pthread_t threads[rows];
	// int return_code;
	// PicTableCell args[rows];

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			avg += pixels[turn][color][i][j];

	avg /= (long long)((long long)rows * (long long)cols);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			pixels[turn][color][i][j] = 0.4 * (float)pixels[turn][color][i][j] + 0.6 * (float)avg;
	// for (int i = 0 ; i < rows; i++)
	// {
	// 	args[i].color = color;
	// 	args[i].row = i;
	// 	args[i].turn = avg;
	// 	return_code = pthread_create(&threads[i], NULL, average_color_row, (void*)&args[i]);
	// 	for (int j = 0; j < cols; j++)
	// 		pixels[turn][color][i][j] = 0.4 * (float)pixels[turn][color][i][j] + 0.6 * (double)avg;

	// 	if (return_code)
	// 	{
	// 		std::cout << "Error in creat thread for average_color_row" << std::endl;
	// 		exit(-1);
	// 	}
	// }

	// for (int i = 0; i < rows; i++)
	// {
	// 	return_code = pthread_join(threads[i], NULL);
	// 	if (return_code)
	// 	{
	// 		std::cout << "Error in join thread from avarage_color_row" << std::endl;
	// 		exit(-1);
	// 	}
	// }
	pthread_exit(NULL);
}

void average()
{
	pthread_t threads[3];
	int return_code;

	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_create(&threads[color], NULL, average_color, (void*)color);

		if (return_code)
		{
			std::cout << "Error in creat pthread for average_color" << std::endl;
			exit(-1);
		}
	}
	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_join(threads[color], NULL);

		if (return_code)
		{
			std::cout << "Error in join thread from average_color" << std::endl;
			exit(-1);
		}
	}
}