#include "Image.hpp"

void* sepia_color_row(void* argument)
{
	PicTableCell arg = *((PicTableCell*)argument);
	int color = arg.color;
	int i = arg.row;
	int nxt = 1 - turn;
	float val;

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
		pixel[nxt][color][i][j] = val;
	}
}

void* sepia_color(void* arg)
{
	long color = (long)arg;
	pthread_t threads[rows];
	PicTableCell args[rows];
	int return_code;

	for (int i = 0; i < rows; i++)
	{
		args[i].color = color;
		args[i].row = i;

		return_code = pthread_create(&threads[i], NULL, sepia_color_row, &args[i]);

		if (return_code)
		{
			std::cout << "Error in create thread for sepia_color_row" << std::endl;
			exit(-1);
		}
	}

	for (int i = 0; i < rows; i++)
	{
		return_code = pthread_join(threads[i], NULL);
		if (return_code)
		{
			std::cout << "Error in join thread from sepia_color_row" << std::endl;
			exit(-1);
		}
	}
}

void sepia()
{
	pthread_t threads[3];
	int return_code;

	for (int color = 0; color < 3; color++)
	{
		return_code = pthread_create(&threads[i], NULL, sepia_color, (void*)color);

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