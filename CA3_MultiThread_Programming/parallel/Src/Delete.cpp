#include "Image.hpp"
#include <iostream>
#include <pthread.h>

void* delete_for_turn(void* collection)
{
	long seri = (long)collection;

	for (int color = 0; color < 3; color++)
	{
		for (int i = 0; i < rows; i++)
			delete(pixels[seri][color][i]);
		delete(pixels[seri][color]);
	}

	pthread_exit(NULL);
}

void delete_allocated_array()
{
	pthread_t threads[2];
	int return_code;
	int arg[] = {0, 1};

	for (int collection = 0; collection < 2; collection++)
	{
		return_code = pthread_create(&threads[collection], NULL, delete_for_turn, (void*)collection);

		if (return_code)
		{
			std::cout << "Error in make thread for delete turn" << std::endl;
			exit(-1);
		}
	}

	for (int collection = 0; collection < 2; collection++)
	{
		return_code = pthread_join(threads[collection], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from delete turn" << std::endl;
			exit(-1);
		}
	}
}