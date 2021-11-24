#ifndef _Map_Process_hpp
#define _Map_Process_hpp

#include "Defines_Libraries.hpp"

class MapProcess
{
	public:
		MapProcess(char* argv[]);
		void start();

	private:
		KeyValueMap read_data();
		void transfer_data_to_red_process(KeyValueMap key_values);

	int pipe_fd;
	std::string file_path;
	std::string pipe_name;
};

#endif