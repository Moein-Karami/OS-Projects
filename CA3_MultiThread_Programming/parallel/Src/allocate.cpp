#include "Image.hpp"
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

		return_code = pthread_create(&threads[i], NULL, allocate_for_row, &args[i]);

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