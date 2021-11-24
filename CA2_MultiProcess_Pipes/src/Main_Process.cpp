#include "Main_Process.hpp"
#include "Map_Process.hpp"
#include "Reduce_Process.hpp"
#include "Common_Tools.hpp"

MainProcess::MainProcess():
number_of_files(0)
{
}

void MainProcess::start()
{
	number_of_files = count_number_of_files();

	for (int i = 1; i <= number_of_files; i++)
		transfer_data_to_map_process(i);

	write_out_data(get_data_from_reduce_process());
}

void MainProcess::transfer_data_to_map_process(int process_number)
{
	char file_path[MAX_STR_LENGTH];
	sprintf(file_path, "%s%s", std::to_string(process_number), CSV);
	int fd[2];;

	pipe(fd);
	write(fd[1], file_path, strlen(file_path));
	close(fd[1]);

	char* argv[] = {(char*)MAP_PROC, (char*)(std::to_string(fd[0])).c_str()};
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
	int fd[2];
	pipe(fd);
	char* argv[] = {(char*)RED_PROC, (char*)(std::to_string(fd[1])).c_str(),
			(char*)(std::to_string(number_of_files)).c_str()};
	int child_number;

	if ((child_number = fork()) < 0)
		std::cerr << "Error in fork" << std::endl;
	else if (child_number == 0)
		execvp(argv[0], argv);
	else
	{
		char input[MAX_STR_LENGTH];
		read(fd[0], input, MAX_STR_LENGTH);
		close(fd[0]);
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

int MainProcess::count_number_of_files()
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