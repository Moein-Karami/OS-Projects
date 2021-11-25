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
	for (int i = 1; i <= number_of_files; i++)
		transfer_data_to_map_process(i);
	write_out_data(get_data_from_reduce_process());
}

void MainProcess::transfer_data_to_map_process(int process_number)
{
	std::string file_path_str = std::to_string(process_number) + CSV;
	const char *file_path = file_path_str.c_str();
	int fd[2];;

	pipe(fd);
	write(fd[1], file_path, strlen(file_path) + 1);
	close(fd[1]);

	char* argv[] = {(char*)MAP_PROC, (char*)(std::to_string(fd[0])).c_str(), NULL};
	int child = fork();

	if (child == 0)
	{
		if (execvp(argv[0], argv) < 0)
		{
			std::cerr << "Error in execute map process" << std::endl;
			exit(1);
		}
	}
	else if (child < 0)
	{
		std::cerr << "Error in fork for map process" << std::endl;
		exit(1);
	}
}

KeyValueMap MainProcess::get_data_from_reduce_process()
{
	int fd[2];
	pipe(fd);
	char fd_str[MAX_STR_LENGTH];
	char number_of_files_str[MAX_STR_LENGTH];
	sprintf(fd_str, "%d", fd[1]);
	sprintf(number_of_files_str, "%d", number_of_files);

	char* argv[] = {(char*)RED_PROC, fd_str, number_of_files_str, NULL};
	int child_number;

	if ((child_number = fork()) < 0)
	{
		std::cerr << "Error in fork for reduce process" << std::endl;
		exit(1);
	}
	else if (child_number == 0)
	{
		if (0 > execvp(argv[0], argv))
		{
			std::cerr << "Error In execute reduce process" << std::endl;
			exit(1);
		}
	}
	else
	{
		char input[MAX_STR_LENGTH];
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
		output << element.key << COLON << element.value << COMMA;
	output.close();
}

int MainProcess::count_number_of_files()
{
	DIR *dir;
	int files = 0;
	struct dirent *ep;
	dir = opendir (TEST_DIR);
	if (dir != NULL)
	{
		while (ep = readdir (dir))
		files++;
		(void) closedir (dir);
	}
	else
    	perror ("Couldn't open the directory");
	return files - 2;
}

int main()
{
	MainProcess main_process;
	main_process.start();
	exit(0);
}