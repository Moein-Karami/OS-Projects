#include "Map_Process.hpp"
#include "Common_Tools.hpp"

MapProcess::MapProcess(char* argv[])
{
	pipe_fd = atoi(argv[1]);

	char file_name[MAX_STR_LENGTH];
	read(pipe_fd, file_name, MAX_STR_LENGTH);
	file_path = std::string(file_name);
	for (auto ch : file_path)
	{
		if (ch != '.')
			pipe_name += ch;
		else
			break;
	}
	pipe_name = PIPE_NAME + pipe_name;
	std::cout << file_path << "_cons_map_" << pipe_name << std::endl; 
}

void MapProcess::start()
{
	transfer_data_to_red_process(read_data());
}

KeyValueMap MapProcess::read_data()
{
	std::fstream file;
	file.open(file_path);
	std::string line;
	KeyValueMap key_value_map;

	while (getline(file, line))
	{
		std::vector<std::string> words = CommonTools::divide_by(line, COMMA);
		for (auto word : words)
			key_value_map[word]++;
	}
	return key_value_map;
}

void MapProcess::transfer_data_to_red_process(KeyValueMap key_values)
{
	std::string data = CommonTools::convert_key_value_map_to_string(key_values);
	const char* data_arr= data.c_str();
	mkfifo((char*)(pipe_name.c_str()), NAMED_PIPE_FLAG);
	int named_pipe_fd = open((char*)(pipe_name.c_str()), O_WRONLY);
	if (named_pipe_fd < 0)
	{
		std::cerr << "Error in open named pipe" << std::endl;
		exit(1);
	}
	write(named_pipe_fd, data_arr, strlen(data_arr));
	close(named_pipe_fd);
}

int main(int argc, char* argv[])
{
	std::cout << "map" << std::endl;
	MapProcess map_process(argv);
	std::cout << "map start" << std::endl;
	map_process.start();
	exit(0);
}





