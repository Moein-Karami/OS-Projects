#include <iostream>
#include <unistd.h>
#include <fstream>
#include "Image.hpp"
#include <chrono>

#ifndef _read_img_cpp
#define _read_img_cpp

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

#pragma pack(1)
#pragma once

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

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

int rows;
int cols;

#endif

bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
{
  	std::ifstream file(fileName);

	if (file)
	{
	file.seekg(0, std::ios::end);
	std::streampos length = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer = new char[length];
	file.read(&buffer[0], length);

	PBITMAPFILEHEADER file_header;
	PBITMAPINFOHEADER info_header;

	file_header = (PBITMAPFILEHEADER)(&buffer[0]);
	info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
	rows = info_header->biHeight;
	cols = info_header->biWidth;
	bufferSize = file_header->bfSize;
	return 1;
	}
	else
	{
		cout << "File" << fileName << " doesn't exist!" << endl;
		return 0;
	}
}

int main(int argc, char *argv[])
{
	auto start_time = std::chrono::high_resolution_clock::now();

	char *fileBuffer;
	int bufferSize;
	char *fileName = argv[1];
	if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize))
	{
		cout << "File read error" << endl;
		return 1;
	}

	// auto filter_start = std::chrono::high_resolution_clock::now();

	Image img(rows, cols);

	img.read_pixels(bufferSize, fileBuffer);

	img.smooth();

	img.sepia();

	img.average();

	img.cross();

	// auto filter_stop = std::chrono::high_resolution_clock::now();

	// auto filter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(filter_stop - filter_start);

	img.export_image(fileBuffer, "Image.bmp", bufferSize);

	auto stop_time = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

	cout << "Runnig Time: " << duration.count() << std::endl;
	// std::cout << "Filter Runnig Time: " <<  filter_duration.count() << std::endl;

	return 0;
}