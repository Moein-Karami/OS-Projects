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

#define TEST_DIR "./testcases/"
#define PIPE_NAME "./Pipes/pipe"
#define NAMED_PIPE_FLAG 0666
#define MAX_STR_LENGTH 1024
#define MAP_PROC "./Build/Map_Proc.out"
#define RED_PROC "./Build/Red_Proc.out"
#define CSV ".csv"
#define COMMA ','
#define SPACE ' '
#define OUTPUT_PATH "./output.csv"
#define COLON ':'
#define PIPE ".pipe"

typedef std::pair<std::string, int> KeyValueStruct;
#define key first
#define value second

typedef std::map<std::string, int> KeyValueMap;

#endif