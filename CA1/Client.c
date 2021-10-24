#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>


#define MAX_MSG_LEN 512
#define WRITE_FD 1
#define READ_FD 0
#define ERR_FD 2
#define NUM_OF_STUDENTS 3
#define SO_REUSEPORT	15

int connect_to_server(int port)
{
	write(WRITE_FD, "connecting to server ...\n", MAX_MSG_LEN);
    int fd;
    struct sockaddr_in server_address;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { // checking for errors
        printf("Error in connecting to server\n");
    }

	char msg[MAX_MSG_LEN];
	sprintf(msg, "connected to server. fd = %d\n", fd);
	write(WRITE_FD, msg, strlen(msg));

    return fd;
}

int request_lesson(int fd)
{
	char buffer[MAX_MSG_LEN];

	write(WRITE_FD, "enter your question subject: ", MAX_MSG_LEN);
	read(READ_FD, buffer, MAX_MSG_LEN);

	write(WRITE_FD, "request class from server\n", MAX_MSG_LEN);

	send(fd, buffer, strlen(buffer), 0);
	recv(fd, buffer, strlen(buffer), 0);

	char msg[MAX_MSG_LEN];
	sprintf(msg, "you join to class, your turn is %s", buffer);
	write(WRITE_FD, msg, strlen(msg));

	return atoi(buffer);
}

void alarm_handler(int sig)
{
	write(WRITE_FD, "Time is up!\n", MAX_MSG_LEN);
}

void start_cominucate(int fd, char* ip, int port, int turn, char* ans)
{
	char buffer[MAX_MSG_LEN];

	write(WRITE_FD, "Connecing to class\n", MAX_MSG_LEN);

	int sock, broadcast = 1, opt = 1;
    struct sockaddr_in bc_address;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    bc_address.sin_family = AF_INET;
    bc_address.sin_port = htons(port);
    bc_address.sin_addr.s_addr = inet_addr(ip);

    bind(sock, (struct sockaddr *)&bc_address, sizeof(bc_address));

	write(WRITE_FD, "you are connected\n", MAX_MSG_LEN);

	char question[MAX_MSG_LEN], answers[2][MAX_MSG_LEN];


	for (int i = 1; i <= NUM_OF_STUDENTS; i++)
	{
		if (i == turn)
		{
			write(WRITE_FD, "Write your question\n", MAX_MSG_LEN);
			read(READ_FD, question, MAX_MSG_LEN);

			sendto(sock, question, strlen(question), 0,(struct sockaddr *)&bc_address, sizeof(bc_address));

			recv(sock, answers[0], MAX_MSG_LEN, 0);
			recv(sock, answers[1], MAX_MSG_LEN, 0);

			write(WRITE_FD, "Which one is better?(1, 2)\n", MAX_MSG_LEN);
			read(READ_FD, buffer, MAX_MSG_LEN);
			int best = atoi(buffer);
			int len = strlen(answers[best]);
			answers[best][len] = '*';
			answers[best][len + 1] = (char)NULL;

			sprintf(ans, "%s\n%s\n%s\n", question, answers[0], answers[1]);
		}
		else
		{
			recv(sock, buffer, MAX_MSG_LEN, 0);

			sprintf(question, "question is:\n%s\n", buffer);
			write(WRITE_FD, question, strlen(question));

			int before = turn - 1;
			if (i < turn)
				turn--;

			while (before--)
				recv(sock, buffer, MAX_MSG_LEN, 0);

			write(WRITE_FD, "write your answer:\n", MAX_MSG_LEN);

			signal(SIGALRM, alarm_handler);
			siginterrupt(SIGALRM, 1);
			alarm(60);
			read(READ_FD, buffer, MAX_MSG_LEN);
			alarm(0);
			sendto(sock, buffer, strlen(question), 0,(struct sockaddr *)&bc_address, sizeof(bc_address));
		}
		close(sock);
	}
}

int main(int argc, char const *argv[])
{
	int fd;
	char buff[MAX_MSG_LEN], ans[NUM_OF_STUDENTS * MAX_MSG_LEN];
	int turn;

	int port = atoi(argv[1]);

	fd = connect_to_server(port);

	turn = request_lesson(fd);

	recv(fd, buff, MAX_MSG_LEN, 0);
	start_cominucate(fd, buff, port, turn, ans);

	write(WRITE_FD, "sending best answer...\n", MAX_MSG_LEN);

	send(fd, ans, strlen(ans), 0);

	write(WRITE_FD, "done\n", MAX_MSG_LEN);

	close(fd);
}