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
	int fd;
	fd = open("fif", O_RDONLY);
	char r[128];
	while(read(fd, r, 128))
		cout << r;
}