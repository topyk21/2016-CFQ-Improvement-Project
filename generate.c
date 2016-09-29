#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
 
#include <sys/time.h>
#include <time.h>

#define READ_BUF_SIZE 4096
#define WRITE_BUF_SIZE 4096

const char* journal_filename = "journal.log";
struct timeval delay_start_point, delay_end_point;
double delaying_time = 0;

void write_journal_entry (void *filename, char* entry)
{
        int fd = open (filename, O_WRONLY | O_CREAT | O_APPEND, 0660);
        write (fd, entry, strlen (entry));
        write (fd, "\n", 1);
        fsync (fd);
        close (fd);
}

void * read_thread(void *filename)
{
	char buf[READ_BUF_SIZE];
	FILE * fp;
	if ((fp = fopen(filename, "r")) < 0)
		printf("file open error\n");
	printf("%s Read Thread TID = %lu\n", filename, pthread_self());

	while(fread(buf, READ_BUF_SIZE, 1, fp) != 0) {
	    // do stuff
	}

	printf("Finished reading 1!\n");
	fclose(fp);
/*	
	fp = fopen(filename, "r");

	while(fread(buf, 4096, 1, fp) != 0) {
	    // do stuff
	}
	printf("Finished reading 2!\n");
	fclose(fp);
	
	fp = fopen(filename, "r");

	while(fread(buf, 4096, 1, fp) != 0) {
	    // do stuff
	}
	printf("Finished reading 3!\n");
	fclose(fp);
*/
	return filename;
}

void * start_thread_fsync(void *filename)
{
//	printf("%s\n", (const char *) filename);
	printf("%s Write Thread TID = %lu\n", filename, pthread_self());
	char *other_filename = "/usr/src/Thing1.txt";
	int f  = open(filename, O_RDWR | O_CREAT | O_SYNC);
	int f2 = open(other_filename, O_RDWR | O_CREAT | O_SYNC);
	char *buf  = NULL;
	int i;
	
	buf = malloc(WRITE_BUF_SIZE);
	memset (buf, 0xcafe, WRITE_BUF_SIZE);

//	sleep(1);
        //lseek (f, 0, SEEK_SET);
	//int records = 10*1000;
	for(i = 0; i < 1000; i++) {
		// time start
		gettimeofday(&delay_start_point, NULL);

		switch (i % 2) {
		case 0:
			write(f, buf, WRITE_BUF_SIZE);
			break;
		case 1:
			write(f2, buf, WRITE_BUF_SIZE);
			break;
		default:
			break;
		}
		fsync(f);
		// time end
		gettimeofday(&delay_end_point, NULL);
		delaying_time += (double)(delay_end_point.tv_sec) +
			(double)(delay_end_point.tv_usec) / 1000000.0 - 
			(double)(delay_start_point.tv_sec) - (double)(delay_start_point.tv_usec) / 1000000.0;
	}
//	fsync(f);
	printf("Write finish!\n");
	close (f);
	return filename;
}

void * start_thread_fdatasync(void *filename)
{
//	printf("%s\n", (const char *) filename);
	
	int f = open(filename, O_RDWR | O_CREAT | O_APPEND);
	char *buf = NULL;
	int i;
	buf = malloc(1024 * 4);
	memset (buf, 0xcafe, 1024 *4);
        //lseek (f, 0, SEEK_SET);
        //int records = 10*1000;
        for(i = 0; i < 2; i++) {
                write(f, buf, 1024 *4);
                fdatasync(f);
        }
	
	close (f);
	return filename;
}

int main(void)
{
	pthread_t thing1, thing2, thing3, thing4, thing5, thing6, thing7;
	
	time_t startTime, endTime;
	struct timeval start_point, end_point;
	double operating_time = 0;
//	startTime = clock();
	gettimeofday(&start_point, NULL);

	const char *filename1 = "./Thing1.txt";
	const char *filename2 = "/usr/src/Thing1.txt";
	//const char *filename2 = "./Thing2.txt";
	//const char *filename3 = "./Thing3.txt";
	//const char *filename4 = "./Thing4.txt";
	//const char *filename5 = "./Thing5.txt";
	const char *read_filename1 = "./sample1.txt";
	const char *read_filename2 = "./sample2.txt";
	const char *read_filename3 = "./sample3.txt";
	const char *read_filename4 = "./sample4.txt";

	/*Create two threads and each with different messages*/
	pthread_create (&thing1, NULL, read_thread, (void *)read_filename1);
	pthread_create (&thing2, NULL, read_thread, (void *)read_filename2);
	pthread_create (&thing3, NULL, read_thread, (void *)read_filename3);
	pthread_create (&thing4, NULL, read_thread, (void *)read_filename4);
	pthread_create (&thing5, NULL, start_thread_fsync, (void *)filename1);
//	pthread_create (&thing6, NULL, start_thread_fsync, (void *)filename2);

	//pthread_create (&thing2, NULL, start_thread_fsync, (void *)filename2);
	//pthread_create (&thing3, NULL, start_thread_fsync, (void *)filename3);
	//pthread_create (&thing4, NULL, start_thread_fsync, (void *)filename4);
	//pthread_create (&thing5, NULL, start_thread_fsync, (void *)filename5);

	/*
	* wait for the threads to exit.
	*/
	pthread_join(thing1, NULL);
	pthread_join(thing2, NULL);
	pthread_join(thing3, NULL);
	pthread_join(thing4, NULL);
	pthread_join(thing5, NULL);
//	pthread_join(thing6, NULL);
//	pthread_join(thing6, NULL);

//	pthread_join(thing7, NULL);

//	endTime = clock();
	gettimeofday(&end_point, NULL);
	operating_time = (double)(end_point.tv_sec) +
		(double)(end_point.tv_usec) / 1000000.0 - (double)(start_point.tv_sec) - (double)(start_point.tv_usec) / 1000000.0;

//	printf("Start Time: %ld\n", startTime);
//	printf("Finish Time: %ld\n", endTime);
//	printf("Time Interval: %lf\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
	printf("TIME : %f\n", operating_time);
	printf("FSYNC DELAY : %f\n", delaying_time);
	printf("AVEG FSYNC DELAY : %f\n", delaying_time / 2000);
	//sync();
	return 0;
}
