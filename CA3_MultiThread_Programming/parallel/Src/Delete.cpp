#include "Image.hpp"
#include <iostream>
#include <pthread.h>

void* delete_for_row(void* arg)
{
	PicTableCell argument = *((PicTableCell*)arg);

	delete(pixels[argument.turn][argument.color][argument.row]);
	pthread_exit(NULL);
}

void* delete_for_color(void* arg)
{
	PicTableCell argument = *((PicTableCell*)arg);
	int seri = argument.turn;
	int color = argument.color;
	pthread_t threads[rows];
	PicTableCell args[rows];
	int return_code;

	for (int i = 0; i < rows; i++)
	{
		args[i].turn = seri;
		args[i].color = color;
		args[i].row = i;

		pthread_create(&threads[i], NULL, delete_for_row, &args[i]);

		if (return_code)
		{
			std::cout << "Error in make thread for delete row" << std::endl;
			exit(-1);
		}
	}
	for (int i = 0; i < rows; i++)
	{
		delete(pixels[seri][color][i]);
		return_code = pthread_join(threads[i], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from delete row" << std::endl;
			exit(-1);
		}
	}
	pthread_exit(NULL);
}

void* delete_for_turn(void* collection)
{
	int seri = *((int*)collection);

	pthread_t threads[3];
	int return_code;
	PicTableCell arg[3];

	for (int color = 0; color < 3; color++)
	{
		arg[color].turn = seri;
		arg[color].color = color;

		return_code = pthread_create(&threads[color], NULL, delete_for_color, &arg[color]);

		if (return_code)
		{
			std::cout << "Error in make thread for delete color" << std::endl;
			exit(-1);
		}
	}

	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_join(threads[color], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from delete color" << std::endl;
			exit(-1);
		}
	}
	pthread_exit(NULL);
}

void delete_array()
{
	pthread_t threads[2];
	int return_code;
	int arg[] = {0, 1};

	for (int i = 0; i < 2; i++)
	{
		return_code = pthread_create(&threads[i], NULL, delete_for_turn, &arg[i]);

		if (return_code)
		{
			std::cout << "Error in make thread for delete turn" << std::endl;
			exit(-1);
		}
	}

	for (int collect = 0; collect < 2; collect++)
	{
		return_code = pthread_join(threads[collect], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from delete turn" << std::endl;
			exit(-1);
		}
	}
}