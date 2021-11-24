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
#define LOCAL_IP "192.168.1.255"
#define ANSWER_TIME 60
#define EXTRA_QUESTION 4
#define EXTRA_BUFFER 3
#define EXTRA_ANS 4

int connect_to_server(int port)
{
	write(WRITE_FD, "connecting to server ...\n", strlen("connecting to server ...\n"));
    int fd;
    struct sockaddr_in server_address;

    fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		write(WRITE_FD, "Problem in socket() function for connect to server\n",
				strlen("Problem in socket() function for connect to server\n"));
		exit(EXIT_FAILURE);
	}

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { // checking for errors
        printf("Error in connecting to server\n");
		exit(EXIT_FAILURE);
    }

	char msg[MAX_MSG_LEN] = {0};
	memset(msg, 0, MAX_MSG_LEN);
	sprintf(msg, "connected to server. fd = %d\n", fd);
	write(WRITE_FD, msg, strlen(msg));

    return fd;
}

int request_lesson(int fd)
{
	char buffer[MAX_MSG_LEN] = {0};

	write(WRITE_FD, "enter your question subject: ", strlen("enter your question subject: "));
	memset(buffer, 0, MAX_MSG_LEN);
	read(READ_FD, buffer, MAX_MSG_LEN);

	write(WRITE_FD, "request class from server\n", strlen("request class from server\n"));

	send(fd, buffer, strlen(buffer), 0);
	memset(buffer, 0, MAX_MSG_LEN);
	recv(fd, buffer, MAX_MSG_LEN, 0);

	char msg[2 * MAX_MSG_LEN] = {0};
	memset(msg, 0, MAX_MSG_LEN);
	sprintf(msg, "you join to class, your turn is %s\n", buffer);
	write(WRITE_FD, msg, strlen(msg));

	return atoi(buffer);
}

void alarm_handler(int sig)
{
	write(WRITE_FD, "Time is up!\n", strlen("Time is up!\n"));
}

void start_comunicate(int fd, int port, int turn)
{
	char buffer[EXTRA_BUFFER * MAX_MSG_LEN] = {0};
	char dummy[MAX_MSG_LEN] = {0};
	char ans[EXTRA_ANS * NUM_OF_STUDENTS * MAX_MSG_LEN] = {0};

	write(WRITE_FD, "Connecing to class\n", strlen("Connecing to class\n"));

	int sock, broadcast = 1, opt = 1;
    struct sockaddr_in bc_address;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		write(WRITE_FD, "Problem in socket() function for broadcast\n",
				strlen("Problem in socket() function for broadcast\n"));
		exit(EXIT_FAILURE);
	}
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    bc_address.sin_family = AF_INET;
    bc_address.sin_port = htons(port);
    bc_address.sin_addr.s_addr = inet_addr(LOCAL_IP);

    bind(sock, (struct sockaddr *)&bc_address, sizeof(bc_address));

	memset(buffer, 0, MAX_MSG_LEN);
	sprintf(buffer, "you are connected to port : %d\n", port);
	write(WRITE_FD, buffer, strlen(buffer));

	char question[EXTRA_QUESTION * MAX_MSG_LEN] = {0}, answers[2][MAX_MSG_LEN] = {0};


	for (int i = 1; i <= NUM_OF_STUDENTS; i++)
	{
		if (i == turn)
		{
			write(WRITE_FD, "Write your question\n", strlen("Write your question\n"));
			memset(question, 0, MAX_MSG_LEN);
			read(READ_FD, question, MAX_MSG_LEN);

			sendto(sock, question, strlen(question), 0,(struct sockaddr *)&bc_address, sizeof(bc_address));
			recvfrom(sock, dummy, MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));

			memset(answers[0], 0, MAX_MSG_LEN);
			memset(answers[1], 0, MAX_MSG_LEN);

			recvfrom(sock, answers[0], MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));
			memset(buffer, 0, MAX_MSG_LEN);
			sprintf(buffer, "Answer1: %s", answers[0]);
			write(WRITE_FD, buffer, strlen(buffer));
			recvfrom(sock, answers[1], MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));
			memset(buffer, 0, MAX_MSG_LEN);
			sprintf(buffer, "Answer2: %s", answers[1]);
			write(WRITE_FD, buffer, strlen(buffer));

			write(WRITE_FD, "Which one is better?(1, 2)\n", strlen("Which one is better?(1, 2)\n"));
			memset(buffer, 0, MAX_MSG_LEN);
			read(READ_FD, buffer, MAX_MSG_LEN);
			int best = atoi(buffer) - 1;

			memset(buffer, 0, MAX_MSG_LEN);
			sprintf(buffer, "Best answer is: %s", answers[best]);
			sendto(sock, buffer, strlen(buffer), 0,(struct sockaddr *)&bc_address, sizeof(bc_address));
			recvfrom(sock, dummy, MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));


			int len = strlen(answers[best]);
			answers[best][len - 1] = '*';
			answers[best][len] = '\n';
			answers[best][len + 1] = '\0';

			memset(ans, 0, NUM_OF_STUDENTS * MAX_MSG_LEN);
			sprintf(ans, "%s\n%s\n%s\n", question, answers[0], answers[1]);
			write(WRITE_FD, "sending best answer...\n", strlen("sending best answer...\n"));

			send(fd, ans, strlen(ans), 0);
		}
		else
		{
			memset(buffer, 0, MAX_MSG_LEN);
			recvfrom(sock, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));

			memset(question, 0, MAX_MSG_LEN);
			sprintf(question, "question is:\n%s\n", buffer);
			write(WRITE_FD, question, strlen(question));

			int before = turn - 1;
			if (i < turn)
				before--;

			if (before)
				write(WRITE_FD, "Wait..., it's not your turn...\n", strlen("Wait..., it's not your turn...\n"));

			while (before--)
				recvfrom(sock, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));

			write(WRITE_FD, "It's your turn now, write your answer:\n",
					strlen("It's your turn now, write your answer:\n"));

			signal(SIGALRM, alarm_handler);
			siginterrupt(SIGALRM, 1);
			alarm(ANSWER_TIME);
			memset(buffer, 0, MAX_MSG_LEN);
			read(READ_FD, buffer, MAX_MSG_LEN);
			alarm(0);
			if (buffer[0] == '\0')
				strcpy(buffer, "No Idea!\n");
			sendto(sock, buffer, strlen(question), 0,(struct sockaddr *)&bc_address, sizeof(bc_address));
			recvfrom(sock, dummy, MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));

			int after = NUM_OF_STUDENTS - turn;
			if (i > turn)
				after--;
			while (after--)
				recvfrom(sock, dummy, MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));

			memset(buffer, 0, MAX_MSG_LEN);
			recvfrom(sock, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&bc_address, (socklen_t *)sizeof(bc_address));
			write(WRITE_FD, buffer, strlen(buffer));
		}
	}
	close(sock);
}

int main(int argc, char const *argv[])
{
	int fd;
	char buff[MAX_MSG_LEN] = {0}, ans[NUM_OF_STUDENTS * MAX_MSG_LEN] = {0};
	int turn;

	int port = atoi(argv[1]);
	fd = connect_to_server(port);

	turn = request_lesson(fd);

	memset(buff, 0, MAX_MSG_LEN);
	recv(fd, buff, MAX_MSG_LEN, 0);
	start_comunicate(fd, atoi(buff), turn);

	write(WRITE_FD, "done\n", strlen("done\n"));

	close(fd);
}