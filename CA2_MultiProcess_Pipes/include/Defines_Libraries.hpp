#ifndef _Defines_Libraries_hpp
#define _Defines_Libraries_hpp

#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <vector>
#include <fcntl.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <map>
#include <algorithm>
#include <iomanip>
#include <experimental/filesystem>

#define TEST_DIR "./testcases"
#define PIPE_NAME "./my_named_pipe"
#define NAMED_PIPE_FLAG 0666
#define MAX_STR_LENGTH 1024
#define MAP_PROC "./Map_Proc"
#define RED_PROC "./Red_Proc"
#define CSV ".csv"
#define COMMA ','
#define SPACE ' '
#define OUTPUT_PATH "output.txt"
#define COLON ':'

typedef std::pair<std::string, int> KeyValueStruct;
#define key first
#define value second

typedef std::map<std::string, int> KeyValueMap;

#endif