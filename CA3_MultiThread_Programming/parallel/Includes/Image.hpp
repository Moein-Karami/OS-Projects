#ifndef _Image_hpp_
#define _Image_hpp_

#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>

#define RED 0
#define BLUE 2
#define GREEN 1

struct PicTableCell
{
	int turn;
	int color;
	int row;
	int col;

};

int rows;
int cols;
int buffer_size;
unsigned char **pixels[2][3];
int turn;
char *file_buffer;
char *file_name;
// class Image
// {
// 	public:

void* allocate_for_row(void* arg);
void* allocate_for_color(void* arg);
void* allocate_for_turn(void* collection);
void allocate_array();

void* delete_for_row(void* arg);
void* delete_for_color(void* arg);
void* delete_for_turn(void* collection);
void delete_allocated_array();

void* read_pixels_from_row(void* arg);
void read_pixels();

void smooth();
void* smooth_color(void* arg);
void* smooth_color_row(void* argument);

void sepia();
void* sepia_color(void* argument);
void* sepia_color_row(void* argument);

void average();
void *average_color(void* arg);
void *average_color_row(void* arguments);

void cross();
void* cross_color(void* arg);

void export_image(std::string file_path);

	// private:

// };

#endif