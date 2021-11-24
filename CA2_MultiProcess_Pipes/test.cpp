#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
	mkfifo("fif", 0666);
	int fd = open("fif", O_WRONLY);
	write(fd, "1234\n", strlen("1234\n"));
	write(fd, "5678\n", strlen("5678\n"));
	close(fd);
}