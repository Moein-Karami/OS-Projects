#ifndef _Reduce_Process_hpp
#define _Reduce_Process_hpp

#include "Defines_Libraries.hpp"

class ReduceProcess
{
	public:
		ReduceProcess(char* argv[]);
		void start();

	private:
		KeyValueMap read_data();
		KeyValueMap read_data_from(int named_pipe_fd);
		void send_data(KeyValueMap key_values);

		int pipe_fd;
		int number_of_files;
};
#endif