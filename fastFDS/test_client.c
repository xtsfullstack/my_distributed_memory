#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#include "make_log.h"

#define LOG_MODULE      "test"
#define LOG_PROC        "fastDFS"


#define FILE_ID_LEN     (256)

int str_IP(const char *src, char *out)
{

	char *p1 = NULL, *p2 = NULL;

	p1 = strstr(src, "ip");
	
	p1 = strchr(p1, ':');

	p1++;
	p1++;
	p2 = p1;
	p2 = strchr(p1, '\n');

	memcpy(out, p1, p2 - p1);

	return 0;
}

int main(int argc, char *argv[])
{
	LOG("DEBUG", "FASTDFS_DEBUG", "func main begin\n");

    char file_id[FILE_ID_LEN] = {0};
    char file_info[FILE_ID_LEN] = {0};

    if (argc < 2) {
        printf("./a.out [filename]\n");
        exit(1);
    }

    char *file_name = argv[1];
	

    pid_t pid;
    int pfd[2];

    if (pipe(pfd) < 0) {
        printf("pip error\n");
    }

	LOG("DEBUG", "FASTDFS_DEBUG", "parent process fork child  begin\n");

    pid = fork();
    if (pid < 0) {
        printf("fork error\n");
    }
	
    if (pid == 0) {
		
		LOG("DEBUG", "FASTDFS_DEBUG", "child process begin\n");

        //child
        //关闭读管道
        close(pfd[0]);

        //将标准输出重定向到写管道
        dup2(pfd[1], STDOUT_FILENO);

		LOG("DEBUG", "FASTDFS_DEBUG", "****file_name = %s***\n", file_name);

		LOG("DEBUG", "FASTDFS_DEBUG", "func execlp begin\n");

        execlp("fdfs_upload_file", "fdfs_upload_file", "/etc/fdfs/client.conf", file_name, NULL);
        //execlp("fdfs_file_info", "fdfs_file_info", "/etc/fdfs/client.conf", file_name, NULL);
		
		return 0;
    }

    //关闭写管道
    close(pfd[1]);

    wait(NULL);
	
	LOG("DEBUG", "FASTDFS_DEBUG", "child process end\n");

    //read from 读管道
    read(pfd[0], file_id, FILE_ID_LEN);

	close(pfd[0]);

	file_id[strlen(file_id) - 1] = '\0';

	LOG("DEBUG", "FASTDFS_DEBUG", "***file_id=%s***\n", file_id);

#if 1

	if (pipe(pfd) < 0) {
        printf("pip error\n");
    }

    pid = fork();
    if (pid < 0) {
        printf("fork error\n");
    }

    if (pid == 0) {
        //child
        //关闭读管道
        close(pfd[0]);

        //将标准输出重定向到写管道
        dup2(pfd[1], STDOUT_FILENO);

		LOG("DEBUG", "FASTDFS_DEBUG", "func execlp2 begin\n");

        //execlp("fdfs_upload_file", "fdfs_upload_file", "/etc/fdfs/client.conf", file_name, NULL);
        execlp("fdfs_file_info", "fdfs_file_info", "/etc/fdfs/client.conf", file_id, NULL);

        close(pfd[1]);

		return 0;
    }

	close(pfd[1]);

    wait(NULL);

    //read from 读管道
    read(pfd[0], file_info, FILE_ID_LEN);

	close(pfd[0]);

	char tmpBuf[1024] = { 0 };
	char output[1024] = { 0 };

	str_IP(file_info, tmpBuf);

	sprintf(output, "http://%s/", tmpBuf);
	strcat(output, file_id);

	//printf("%s\n", output);
	write(STDOUT_FILENO, output, strlen(output));

	LOG("DEBUG", "FASTDFS_DEBUG", "***output=%s***\n", output);

#endif

    return 0;
}
