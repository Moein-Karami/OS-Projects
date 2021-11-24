#include "Main_Process.hpp"
#include "Common_Tools.hpp"
#include <dirent.h>

MainProcess::MainProcess():
number_of_files(0)
{
}

void MainProcess::start()
{
	number_of_files = count_number_of_files();
	number_of_files = 4;
	for (int i = 1; i <= number_of_files; i++)
		transfer_data_to_map_process(i);
	std::cout << "wtf" << std::endl;
	write_out_data(get_data_from_reduce_process());
}

void MainProcess::transfer_data_to_map_process(int process_number)
{
	std::string file_path_str = TEST_DIR + std::to_string(process_number) + CSV;
	const char *file_path = file_path_str.c_str();
	int fd[2];;

	pipe(fd);
	write(fd[1], file_path, strlen(file_path));
	close(fd[1]);

	char* argv[] = {(char*)MAP_PROC, (char*)(std::to_string(fd[0])).c_str()};

	if (fork() == 0)
	{
		std::cout << process_number << " exec" << std::endl;
		std::cout << "" << argv[0] << "_*_" << argv[1] << std::endl;
		execvp(argv[0], argv);
		exit(0);
	}
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
	{
		execvp(argv[0], argv);
		exit(0);
	}
	else
	{
		char input[MAX_STR_LENGTH];
		std::cout << "WATING" << std::endl;
		read(fd[0], input, MAX_STR_LENGTH);
		close(fd[0]);
		std::string str_input(input);
		return CommonTools::convert_string_to_key_value_map(str_input);
	}
	return KeyValueMap();
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
	int num = 0;
	struct dirent *entry;
	DIR *dir = opendir(TEST_DIR);
	while ((entry = readdir(dir)) != NULL)
		num++;
	return num;
}

int main()
{
	MainProcess main_process;
	main_process.start();
}