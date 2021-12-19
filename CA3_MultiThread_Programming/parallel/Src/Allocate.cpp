#include "Image.hpp"
#include <iostream>
#include <pthread.h>

void* allocate_for_color(void* arg)
{
	PicTableCell argument = *((PicTableCell*)arg);
	int seri = argument.turn;
	int color = argument.color;
	pixels[seri][color] = new unsigned char*[rows];

	for (int i = 0; i < rows; i++)
		pixels[seri][color][i] = new unsigned char[cols];

	pthread_exit(NULL);
}

void* allocate_for_turn(void* collection)
{
	long seri = (long)collection;

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
	pthread_exit(NULL);
}

void allocate_array()
{
	turn = 0;
	pthread_t threads[2];
	int return_code;

	for (int collection = 0; collection < 2; collection++)
	{
		return_code = pthread_create(&threads[collection], NULL, allocate_for_turn, (void*)collection);

		if (return_code)
		{
			std::cout << "Error in make thread for allocate turn" << std::endl;
			exit(-1);
		}
	}

	for (int collection = 0; collection < 2; collection++)
	{
		return_code = pthread_join(threads[collection], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from allocate turn" << std::endl;
			exit(-1);
		}
	}
}