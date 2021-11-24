#include "Map_Process.hpp"
#include "Common_Tools.hpp"
#include "Reduce_Process.hpp"

MapProcess::MapProcess(char* argv[])
{
	pipe_fd = atoi(argv[1]);
	named_pipe_fd = open(argv[2]);

	char file_name[MAX_STR_LENGTH];
	read(pipe_fd, file_name, MAX_STR_LENGTH);
	file_path = std::string(file_name);
}

MapProcess::~MapProcess()
{
	close(pipe_fd);
	close(named_pipe_fd);
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
	std::string data = CommonTools::convert_map_to_string(key_values);
	write(named_pipe_fd, (char*)data, data.size());
}

int main(int argc, char* argv[])
{
	MapProcess map_process(argv);
	map_process.start();
}





