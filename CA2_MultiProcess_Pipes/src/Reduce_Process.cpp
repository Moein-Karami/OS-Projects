#include "Reduce_Process.hpp"
#include "Common_Tools.hpp"

ReduceProcess::ReduceProcess(char* argv[])
{
	pipe_fd = atoi(argv[1]);
	number_of_files = atoi(argv[2]);
}

KeyValueMap ReduceProcess::read_data()
{
	KeyValueMap key_values;
	for (int i = 1; i <= number_of_files; i++)
	{
		const char* pipe_name = (std::to_string(i) + PIPE_NAME).c_str();
		int named_pipe_fd = -1;
		while (named_pipe_fd < 0)
			named_pipe_fd = open(pipe_name, O_RDONLY);
		KeyValueMap tmp = read_data_from(named_pipe_fd);
		close(named_pipe_fd);
		for (auto element : tmp)
			key_values[element.key] += element.value;
	}
	return key_values;
}

void ReduceProcess::send_data(KeyValueMap key_values)
{
	std::string data = CommonTools::convert_key_value_map_to_string(key_values);
	const char* data_arr = data.c_str();
	write(pipe_fd, data_arr, strlen(data_arr));
	close(pipe_fd);
}

void ReduceProcess::start()
{
	send_data(read_data());
}

int main(int argc, char* argv[])
{
	ReduceProcess reduce_process(argv);
	reduce_process.start();
	exit(0);
}