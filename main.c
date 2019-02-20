#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/wait.h>

void child_access_file(int rpid) {

	// File name/buffers
	char fname[32] = "seq_number";
	char buf[32];
	char wbuf[32];

	// File containing the sequence number
	int in_fd;
	int out_fd;
	int read_data;

	int pid = fork();

	// Perform file read/write
	if(pid == 0) {
		//  a. Open F
		in_fd = open(fname, O_RDONLY);

		//	b. Read the sequence number N from the file
		read_data = read(in_fd, buf, 32);
		if(read_data < 0) {
			printf("Error: unable to complete file read.\n");
			return;
		}

		//	c. Close F
		close(in_fd);

		//	d. Output N and the process' PID (either on screen or test file)
		printf("\n********************************************\nProcess ID: %d <> Read value: %s\n", rpid, buf);

		//	e. Increment N by 1
		int new_value = atoi(buf) + 1;
		sprintf(wbuf, "%d", new_value);
		strcat(wbuf, "\0");

		//	f. Open F
		out_fd = open(fname, O_WRONLY);

		//	g. Write N to F
		write(out_fd, wbuf, strlen(wbuf));

		//	h. Close F.
		close(out_fd);
		exit(0);
	}
	else {
		// Wait for this function call to finish executing
		int alive, status;
		while((alive = wait(&status)) != -1) {
			printf("\t%d done reading/writing to file...\n********************************************\n\n", rpid);
		}
	}
}

int main() {

	srand(time(0));

	// Create 3 children with random 1-500 PID, isolate them into
	// a function so we don't have branching fork() calls
	child_access_file(rand() % 500);
	child_access_file(rand() % 500);
	child_access_file(rand() % 500);

/*
	// 3 processes: base parent process, two children
	pid_t p1_pid = getpid();
	pid_t p2_pid = fork();
	pid_t p3_pid = -1;
	// Stop the 1st child process from creating an extra child...
	if(getpid() == p1_pid) {
		p3_pid = fork();
	}

	if(p3_pid == 0) {
		printf("Process %d: accessing file...", p3_pid);
		//  a. Open F
		in_fd = open(fname, O_RDONLY);

		//	b. Read the sequence number N from the file
		read_data = read(in_fd, buf, 32);
		if(read_data < 0) {
	        printf("Error: unable to complete file read.\n");
	        return -1;
	    }

		//	c. Close F
		close(in_fd);

		//	d. Output N and the process' PID (either on screen or test file)
		printf("Process ID: %d <> Read value: %s\n", getpid(), buf);

		//	e. Increment N by 1
		int new_value = atoi(buf) + 1;
		sprintf(wbuf, "%d", new_value);
		strcat(wbuf, "\0");

		//	f. Open F
		out_fd = open(fname, O_WRONLY);

		//	g. Write N to F
		write(out_fd, wbuf, strlen(wbuf));

		//	h. Close F.
		close(out_fd);

	}
	*/


	return 0;
}
