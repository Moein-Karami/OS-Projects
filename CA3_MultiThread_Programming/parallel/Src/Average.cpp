#include "Image.hpp"

void* average_color(void* arg)
{
	long color = (long)arg;
	long long avg = 0;

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			avg += pixels[turn][color][i][j];
	
	avg[i] /= (long long)((long long)rows * (long long)cols);


	
	for (int color = 0; color < 3; color++)
		for (int i = 0 ; i < rows; i++)
			for (int j = 0; j < cols; j++)
				pixels[turn][color][i][j] = 0.4 * (float)pixels[turn][color][i][j] + 0.6 * (double)avg[color];
}

void average()
{
	pthread_t threads[3];
	int return_code;

	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_create(&threads[i], NULL, average_color, (void*)color);

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