#ifndef _read_img_cpp
#define _read_img_cpp

#include "Image.hpp"

#pragma pack(1)
#pragma once

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

int rows;
int cols;
int buffer_size;
unsigned char **pixels[2][3];
int turn;
char *file_buffer;
char *file_name;

typedef struct tagBITMAPFILEHEADER
{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#endif

bool fillAndAllocate()
{
  	std::ifstream file(file_name);

	if (file)
	{
		file.seekg(0, std::ios::end);
		std::streampos length = file.tellg();
		file.seekg(0, std::ios::beg);

		file_buffer = new char[length];
		file.read(&file_buffer[0], length);

		PBITMAPFILEHEADER file_header;
		PBITMAPINFOHEADER info_header;

		file_header = (PBITMAPFILEHEADER)(&file_buffer[0]);
		info_header = (PBITMAPINFOHEADER)(&file_buffer[0] + sizeof(BITMAPFILEHEADER));
		rows = info_header->biHeight;
		cols = info_header->biWidth;
		buffer_size = file_header->bfSize;
		return 1;
	}
	else
	{
		std::cout << "File" << file_name << " doesn't exist!" << std::endl;
		return 0;
	}
}

int main(int argc, char *argv[])
{
	auto start_time = std::chrono::high_resolution_clock::now();

	file_name = argv[1];

	if (!fillAndAllocate())
	{
		std::cout << "File read error" << std::endl;
		return 1;
	}

	// auto filter_start = std::chrono::high_resolution_clock::now();

	allocate_array();

	read_pixels();

	smooth();

	sepia();

	average();

	cross();

	// auto filter_stop = std::chrono::high_resolution_clock::now();

	// auto filter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(filter_stop - filter_start);

	export_image("Image.bmp");

	delete_allocated_array();
	
	auto stop_time = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

	std::cout << "Total Runnig Time: " <<  duration.count() << std::endl;
	// std::cout << "Filter Runnig Time: " <<  filter_duration.count() << std::endl;

	return 0;
}