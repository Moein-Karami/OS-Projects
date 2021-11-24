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
	write(fd, "9121112", strlen("9101112") );
	write(fd, "13141516", strlen("13141516") );
	close(fd);
	fd = open("fif", O_RDONLY);
	char r[128];
	while(read(fd, r, 128))
		cout << r;
}