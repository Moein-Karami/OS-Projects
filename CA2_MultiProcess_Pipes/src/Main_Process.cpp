#include "Main_Process.hpp"
#include "Map_Process.hpp"
#include "Reduce_Process.hpp"
#include "Common_Tools.hpp"

MainProcess::MainProcess():
named_pipe_fd(-1)
{
}

MainProcess::~MainProcess():
{
	for (auto fds : pipes_fd)
	{
		close(fds[0]);
		close(fds[1]);
		free(fds);
	}
	close(named_pipe_fd);
}

void MainProcess::start()
{
	int number_of_files = count_number_of_files();
	mkfifo(PIPE_NAME, NAMED_PIPE_FLAG);
	named_pipe_fd = open((char*)PIPE_NAME, O_RDONLY);

	for (int i = 1; i <= number_of_files; i++)
		transfer_data_to_map_process(i);

	write_out_data(get_data_from_reduce_process());
}

void MainProcess::transfer_data_to_map_process(int process_number)
{
	std::string test_path = std::to_string(process_number)+ CSV;
	int* fd = new int[2];
	pipes_fd.push_back(fd);
	std::string file_path = to_string(process_number) + CSV;
	pipe(fd);
	write(fd[1], (char*)file_path, file_path.size());
	char* argv[] = {(char*)MAP_PROC, (char*)(std::to_string(fd[0])).c_str(), (char*)file_path, (char*)PIPE_NAME};
	int child_number;

	if ((child_number = fork()) < 0)
	{
		std::cerr << "Error in fork" << std::endl;
		exit(1);
	}
	else if (child_number == 0)
		execvp(argv[0], argv);
}

KeyValueMap MainProcess::get_data_from_reduce_process()
{
	char* argv[] = {(char*)RED_PROC, (char*)PIPE_NAME};
	int child_number;

	if ((child_number = fork()) < 0)
		std::cerr << "Error in fork" << std::endl;
	else if (child_number == 0)
		execvp(argv[0], argv);
	else
	{
		char input[MAX_STR_LENGTH];
		read(named_pipe_fd, input, MAX_STR_LENGTH);
		std::string str_input(input);
		return CommonTools::convert_string_to_key_value_map(str_input);
	}
}

void MainProcess::write_out_data(KeyValueMap key_values)
{
	std::ofstream output;
	output.open(OUTPUT_PATH);
	for (auto element : key_values)
		output << element.key << COLON << SPACE << element.value << std::endl;
	output.close();
}

int MainProcess::number_of_files()
{
	std::experimental::filesystem::path path(TEST_DIR);
	int number_of_files = 0;
	for (auto& p : std::experimental::filesystem::directory_iterator(path))
		number_of_files++;
	return number_of_files;
}

int main()
{
	MainProcess main_process;
	main_process.start();
}