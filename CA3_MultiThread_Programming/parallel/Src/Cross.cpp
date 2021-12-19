#include "Image.hpp"

void* cross_color(void* arg)
{
	long color = (long)arg;
	int j;

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
	pthread_exit(NULL);
}

void cross()
{
	pthread_t threads[3];
	int return_code;

	for (int color = 0 ; color < 3; color++)
	{
		return_code = pthread_create(&threads[color], NULL, cross_color, (void*)color);

		if (return_code)
		{
			std::cout << "Error in creat thread for corss_color" << std::endl;
			exit(-1);
		}
	}
	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_join(threads[color], NULL);

		if (return_code)
		{
			std::cout << "Error in join thread from cross_color" << std::endl;
			exit(-1);
		}
	}
}