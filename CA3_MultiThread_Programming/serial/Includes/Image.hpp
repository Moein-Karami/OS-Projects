#include <fstream>
#include <string>

#define RED 0
#define BLUE 2
#define GREEN 1

class Image
{
	public:
		Image(int rows, int cols);
		~Image();
		void read_pixels(int buffer_size, char* file_buffer);
		void smooth();
		void sepia();
		void average();
		void cross();
		void export_image(char* file_buffer, std::string file_path, int buffer_size);

	private:
		int rows;
		int cols;
		unsigned char **pixels[2][3];
		int turn;

};