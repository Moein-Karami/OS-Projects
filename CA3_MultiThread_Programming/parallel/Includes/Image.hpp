#ifndef _Image_hpp_
#define _Image_hpp_

#include <fstream>
#include <string>

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

void read_pixels(int buffer_size, char* file_buffer);
void smooth();
void sepia();
void average();
void cross();
void export_image(char* file_buffer, std::string file_path, int buffer_size);



	// private:
		


// };

#endif