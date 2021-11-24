#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>

#define MAX_LESSON_LEN 128
#define NUM_OF_STUDENTS_IN_CLASS 3
#define IP_LEN 16
#define INITIAL_NUMBER_OF_CLASSES 128
#define INITIAL_NUMBER_OF_CLIENTS 384
#define MAX_NUM_OF_CLIENTS_IN_QUEUE 32
#define BUFFER_SIZE 128
#define WRITE_FD 1
#define READ_FD 0
#define ERR_FD 2
#define MAX_MSG_LEN 1024
#define COMPUTER 0
#define ELEC 1
#define CIVIL 2
#define MECH 3
#define NUM_OF_CLASSES 4
#define PORT 4000

#define STUDENT_WANTS_TO_JOIN_CLASS 1
#define STUDENT_IS_IN_CLASS 2
#define STUDENT_IN_COMINUCATING 3
#define STUDENT_FINISHED 4

#define CLASS_IS_OPEN 1
#define CLASS_IS_BUSY 2
#define CLASS_IS_CLOSED 3

struct Client
{
	int client_fd;
	char lesson[MAX_LESSON_LEN];
	int situation;
	int class_id;
	int turn;
};

struct Class
{
	int num_of_current_students;
	struct Client* students[NUM_OF_STUDENTS_IN_CLASS];
	int num_of_answered_questions;
	int port;
	int situation;
	char lesson[MAX_LESSON_LEN];
};

struct Clients
{
	int max_num_of_clients;
	int current_max_num_of_clients;
	struct Client* clients;
};

struct Classes
{
	int max_num_of_classes;
	int current_max_num_of_classes;
	struct Class* classes;
	int open_classes[NUM_OF_CLASSES];
	unsigned int last_used_port;
};

void init_clients(struct Clients* clients)
{
	clients->max_num_of_clients = INITIAL_NUMBER_OF_CLIENTS;
	clients->current_max_num_of_clients = 0;
	clients->clients = (struct Client*) malloc(clients->max_num_of_clients * sizeof(struct Client));
}

void init_classes(struct Classes* classes)
{
	classes->max_num_of_classes = INITIAL_NUMBER_OF_CLASSES;
	classes->current_max_num_of_classes = 0;
	classes->classes = (struct Class*) malloc(classes->max_num_of_classes * sizeof(struct Class));
	for (int i = 0; i < NUM_OF_CLASSES; i++)
		classes->open_classes[i] = -1;
	classes->last_used_port = PORT;
}

int setup_server(int port)
{
	write(WRITE_FD, "setting up server ...\n", strlen("setting up server ...\n"));

	struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd < 0)
	{
		write(WRITE_FD, "Problem in socket() function\n",
				strlen("Problem in socket() function\n"));
		exit(EXIT_FAILURE);
	}

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, MAX_NUM_OF_CLIENTS_IN_QUEUE);

    return server_fd;
}

int add_client(int server_fd, struct Clients* clients)
{
	char buffer[MAX_MSG_LEN] = {0};

	write(WRITE_FD, "adding new client ...\n", strlen("adding new client ...\n"));

	int new_client_fd;
	struct sockaddr_in client_address;
	int address_len = sizeof(client_address);
	new_client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);

	while (clients->max_num_of_clients <= new_client_fd)
	{
		clients->max_num_of_clients *= 2;
		clients->clients = realloc(clients->clients, clients->max_num_of_clients);
	}

	if (clients->current_max_num_of_clients < new_client_fd)
		clients->current_max_num_of_clients = new_client_fd;
	clients->clients[new_client_fd].client_fd = new_client_fd;
	clients->clients[new_client_fd].situation = STUDENT_WANTS_TO_JOIN_CLASS;

	memset(buffer, 0, MAX_MSG_LEN);
	sprintf(buffer, "new client connected. fd = %d\n", new_client_fd);
	write(WRITE_FD, buffer, strlen(buffer));

	return new_client_fd;
}

int get_lesson_id(char* lesson)
{
	if (strcmp(lesson, "Computer\n") == 0)
		return COMPUTER;
	else if (strcmp(lesson, "Electronics\n") == 0)
		return ELEC;
	else if (strcmp(lesson, "Civil\n") == 0)
		return CIVIL;
	else
		return MECH;
}

int find_class(struct Classes* classes, char* lesson)
{
	write(WRITE_FD, "finding class...\n", strlen("finding class...\n"));

	int lesson_id = get_lesson_id(lesson);

	if (classes->open_classes[lesson_id] == -1 || classes->classes[classes->open_classes[lesson_id]].situation !=
			CLASS_IS_OPEN)
	{
		classes->current_max_num_of_classes ++;
		if (classes->current_max_num_of_classes >= classes->max_num_of_classes)
		{
			classes->max_num_of_classes *= 2;
			classes->classes = realloc(classes->classes, classes->max_num_of_classes);
		}
		classes->open_classes[lesson_id] = classes->current_max_num_of_classes;
		classes->classes[classes->open_classes[lesson_id]].num_of_current_students = 0;
		classes->classes[classes->open_classes[lesson_id]].num_of_answered_questions = 0;
		classes->classes[classes->open_classes[lesson_id]].situation = CLASS_IS_OPEN;
	}
	return classes->open_classes[lesson_id];
}

int enter_class(struct Client* client, struct Classes* classes, char* lesson)
{
	char msg[MAX_MSG_LEN] = {0};

	memset(msg, 0, MAX_MSG_LEN);
	sprintf(msg, "client %d wants enter %s\n", client->client_fd, lesson);
	write(WRITE_FD, msg, strlen(msg));

	client->situation = STUDENT_IS_IN_CLASS;
	strcpy(client->lesson, lesson);
	int class_id = find_class(classes, lesson);
	classes->classes[class_id].students[classes->classes[class_id].num_of_current_students] = client;
	classes->classes[class_id].num_of_current_students++;
	client->class_id = class_id;
	client->turn = classes->classes[class_id].num_of_current_students;
	return class_id;
}

void start_class(struct Classes* classes, int class_id)
{
	char msg[MAX_MSG_LEN] = {0};
	classes->last_used_port++;

	memset(msg, 0, MAX_MSG_LEN);
	sprintf(msg, "class %d is ready to start on port %d\n", class_id, classes->last_used_port);
	write(WRITE_FD, msg, strlen(msg));

	memset(msg, 0, MAX_MSG_LEN);
	sprintf(msg, "%d", classes->last_used_port);
	classes->classes[class_id].port = classes->last_used_port;
	for (int i = 0; i < NUM_OF_STUDENTS_IN_CLASS; i++)
	{
		send(classes->classes[class_id].students[i]->client_fd, msg, strlen(msg), 0);
		classes->classes[class_id].students[i]->situation = STUDENT_IN_COMINUCATING;
	}
	write(WRITE_FD, "port sent to student\n", strlen("port sent to student\n"));

	classes->classes[class_id].situation = CLASS_IS_BUSY;
}

int main(int argc, char const *argv[])
{
	write(WRITE_FD, "Start initial classes and clients ...\n", strlen("Start initial classes and clients ...\n"));
	struct Clients clients;
	struct Classes classes;
	init_clients(&clients);
	init_classes(&classes);
	char msg[MAX_MSG_LEN] = {0};

	write(WRITE_FD, "Reading port ...\n", strlen("Reading port ...\n"));
	char buffer[MAX_MSG_LEN] = {0};
	int port = atoi(argv[1]);
	fd_set master_set, working_set;

	memset(buffer, 0, MAX_MSG_LEN);
	sprintf(buffer, "port %d activated\n", port);
	write(WRITE_FD, buffer, strlen(buffer));

	int server_fd = setup_server(port);
	int max_fd = server_fd;

	FD_ZERO(&master_set);
	FD_SET(server_fd, &master_set);

	memset(buffer, 0, MAX_MSG_LEN);
	sprintf(buffer, "server is runing... fd = %d\n", server_fd);
	write(WRITE_FD, buffer, strlen(buffer));

	while(1)
	{
		working_set = master_set;

		write(WRITE_FD, "waiting for request...\n", strlen("waiting for request...\n"));

		select(max_fd + 1, &working_set, NULL, NULL, NULL);

		write(WRITE_FD, "request received\n", strlen("request received\n"));

		for (int i = 0; i <= max_fd; i++)
		{
			if (FD_ISSET(i, &working_set))
			{
				if (i == server_fd) // connect request
				{
					int new_fd = add_client(server_fd, &clients);
					FD_SET(new_fd, &master_set);
					if (new_fd > max_fd)
						max_fd = new_fd;
				}
				else
				{
					if (clients.clients[i].situation == STUDENT_WANTS_TO_JOIN_CLASS) // request room for lesson
					{
						memset(buffer, 0, MAX_MSG_LEN);
						recv(i, buffer, MAX_MSG_LEN, 0);

						int class_id = enter_class(&clients.clients[i], &classes, buffer);

						memset(msg, 0, MAX_MSG_LEN);
						sprintf(msg, "class %d is proper\n", class_id);
						write(WRITE_FD, msg, strlen(msg));

						memset(msg, 0, MAX_MSG_LEN);
						sprintf(msg, "%d", clients.clients[i].turn);
						send(i, msg, strlen(msg), 0);

						if (classes.classes[class_id].num_of_current_students == NUM_OF_STUDENTS_IN_CLASS)
							start_class(&classes, class_id);
					}
					else if (clients.clients[i].situation == STUDENT_IS_IN_CLASS)
					{
						memset(buffer, 0, MAX_MSG_LEN);
						recv(i, buffer, MAX_MSG_LEN, 0);
						send(i, "wait until class start\n", strlen("wait until class start\n"), 0);
					}
					else // recieve q&a
					{
						write(WRITE_FD, "some one wants to send answer\n", strlen("some one wants to send answer\n"));
						int file_fd = open("data.txt", O_CREAT | O_APPEND | O_RDWR, 0666);
						if (file_fd < 0)
						{
							write(WRITE_FD, "Error in openning file\n", strlen("Error in openning file\n"));
							exit(EXIT_FAILURE);
						}
						memset(buffer, 0, MAX_MSG_LEN);
						recv(i, buffer, MAX_MSG_LEN, 0);
						write(file_fd, buffer, strlen(buffer));
						write(file_fd, "\n", strlen("\n"));
						close(i);
						FD_CLR(i, &master_set);
						close(file_fd);

						memset(msg, 0, MAX_MSG_LEN);
						sprintf(msg, "student %i has sent q&a\n", i);
						send(WRITE_FD, msg, strlen(msg), 0);

						clients.clients[i].situation = STUDENT_FINISHED;
						classes.classes[clients.clients[i].class_id].num_of_answered_questions++;
						if (classes.classes[clients.clients[i].class_id].num_of_answered_questions == 3)
							classes.classes[clients.clients[i].class_id].situation = CLASS_IS_CLOSED;
					}

				}
			}
		}
	}
}