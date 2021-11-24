#ifndef _Main_Process_hpp
#define _Main_Process_hpp

#include "Defines_Libraries.hpp"

class MainProcess
{
	public:
		MainProcess();
		void start();

	private:
		void transfer_data_to_map_process(int process_number);
		KeyValueMap get_data_from_reduce_process();
		void write_out_data(KeyValueMap key_valus);
		int count_number_of_files();

		int number_of_files;
};

#endif