#ifndef _Image_hpp_
#define _Image_hpp_

#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <pthread.h>

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

extern int rows;
extern int cols;
extern int buffer_size;
extern unsigned char **pixels[2][3];
extern int turn;
extern char *file_buffer;
extern char *file_name;

extern void* allocate_for_color(void* arg);
extern void* allocate_for_turn(void* collection);
extern void allocate_array();

extern void* delete_for_turn(void* collection);
extern void delete_allocated_array();

extern void* read_pixels_from_row(void* arg);
extern void read_pixels();

extern void smooth();
extern void* smooth_color(void* arg);

extern void sepia();
extern void* sepia_color(void* argument);

extern void average();
extern void *average_color(void* arg);

extern void cross();
extern void* cross_color(void* arg);

extern void export_image(std::string file_path);
void* export_pixels_from_row(void* arg);


#endif