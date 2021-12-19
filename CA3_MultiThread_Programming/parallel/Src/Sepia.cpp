#include "Image.hpp"

void* sepia_color(void* arg)
{
	long color = (long)arg;
	float val;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j <= cols; j++)
		{
			switch (color)
			{
				case RED:
					val = 0.393 * (float)pixels[turn][RED][i][j] + 0.769 * (float)pixels[turn][GREEN][i][j] + 0.189 *
							(float)pixels[turn][BLUE][i][j];
					break;
				case GREEN:
					val = 0.349 * (float)pixels[turn][RED][i][j] + 0.686 * (float)pixels[turn][GREEN][i][j] + 0.168 *
							(float)pixels[turn][BLUE][i][j];
					break;
				case BLUE:
				val = 0.272 * (float)pixels[turn][RED][i][j] + 0.534 * (float)pixels[turn][GREEN][i][j] + 0.131 *
							(float)pixels[turn][BLUE][i][j];
					break;
			}
			val = (val > 255) ? 255 : val;
			pixels[1 - turn][color][i][j] = val;
		}
	}

	pthread_exit(NULL);
}

void sepia()
{
	pthread_t threads[3];
	int return_code;

	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_create(&threads[color], NULL, sepia_color, (void*)color);

		if (return_code)
		{
			std::cout<< "Error in create thread for sepia_color" << std::endl;
			exit(-1);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		return_code = pthread_join(threads[i], NULL);
		if (return_code)
		{
			std::cout << "Error in thread join from sepia_color" << std::endl;
			exit(-1);
		}
	}
	turn = 1 - turn;
}