#include "Image.hpp"

Image::Image(int rows, int cols):
rows(rows),
cols(cols)
{
	turn = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pixels[i][j] = new unsigned char*[rows];
			for (int k = 0; k < rows; k++)
				pixels[i][j][k] = new unsigned char[cols];
		}
	}
}

void Image::read_pixels(int buffer_size, char* file_buffer)
{
	int count = 1;
	int extra = cols % 4;
	for (int i = 0; i < rows; i++)
	{
		count += extra;
		for (int j = cols - 1; j >= 0; j--)
		{
			for (int k = 0; k < 3; k ++)
			{
				pixels[turn][k][i][j] = file_buffer[buffer_size - count];
				count ++;
			}
		}
	}
}

void Image::smooth()
{
	int nxt = 1 - turn;
	int sum, cnt;
	for (int color = 0; color < 3; color++)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				sum = 0;
				cnt = 0;

				for (int delta_x = -1; delta_x <= 1; delta_x++)
					for (int delta_y = -1; delta_y <= 1; delta_y++)
						if (0 <= i + delta_x && i + delta_x < rows && 0 <= j + delta_y && j + delta_y < cols)
							sum += (int)pixels[turn][color][i + delta_x][j + delta_y], cnt++;
				sum /= cnt;
				pixels[nxt][color][i][j] = sum;
			}
		}
	}
	turn = nxt;
}

void Image::sepia()
{
	int nxt = 1 - turn;
	int val;
	for (int color = 0; color < 3; color++)
	{
		for (int i = 0 ; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				switch (color)
				{
					case RED:
						val = (float)pixels[turn][RED][i][j] * 0.393 + (float)pixels[turn][GREEN][i][j] * 0.769 +
								(float)pixels[turn][BLUE][i][j] * 0.189;
						break;
					case BLUE:
						val = (float)pixels[turn][RED][i][j] * 0.349 + (float)pixels[turn][GREEN][i][j] * 0.686 +
								(float)pixels[turn][BLUE][i][j] * 0.168;
						break;
					case GREEN:
						val = (float)pixels[turn][RED][i][j] * 0.272 + (float)pixels[turn][GREEN][i][j] * 0.534 +
								(float)pixels[turn][BLUE][i][j] * 0.131;
						break;
				}
				pixels[nxt][color][i][j] = (val < 255) ? val : 255;
			}
		}
	}
	turn = nxt;
}

void Image::average()
{
	long long int avg[] = {0, 0, 0};
	for (int color = 0; color < 3; color++)
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < rows; j++)
				avg[color] += pixels[turn][color][i][j];
	for (int i = 0; i < 3; i++)
		avg[i] /= (long long)((long long)rows * (long long)cols);

	for (int color = 0; color < 3; color++)
		for (int i = 0 ; i < rows; i++)
			for (int j = 0; j < cols; j++)
				pixels[turn][color][i][j] = 0.4 * (float)pixels[turn][color][i][j] + 0.6 * (double)avg[color];
}

void Image::cross()
{
	int j;
	for (int color = 0 ; color < 3; color++)
	{
		for (int i = 0; i < rows; i++)
		{
			j = cols - i - 1;
			for (int delta = -1; delta <= 1; delta++)
			{
				if (i + delta < cols && i + delta >= 0)
					pixels[turn][color][i][i + delta] = 255;
				if (j + delta < cols && j + delta >= 0)
					pixels[turn][color][i][j] = 255;
			}
		}
	}
}