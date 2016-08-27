#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * echo.c --
 *
 *	Produce a page containing all FastCGI inputs
 *
 *
 * Copyright (c) 1996 Open Market, Inc.
 *
 * See the file "LICENSE.TERMS" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */
#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include "fcgi_config.h"

#include <stdlib.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#include "fcgi_stdio.h"

char* memstr(char* full_data, int full_data_len, char* substr) 
{ 
	if (full_data == NULL || full_data_len <= 0 || substr == NULL) { 
		return NULL; 
	} 

	if (*substr == '\0') { 
		return NULL; 
	} 

	int sublen = strlen(substr); 

	int i; 
	char* cur = full_data; 
	int last_possible = full_data_len - sublen + 1; 
	for (i = 0; i < last_possible; i++) { 
		if (*cur == *substr) { 
			//assert(full_data_len - i >= sublen);  
			if (memcmp(cur, substr, sublen) == 0) { 
				//found  
				return cur; 
			} 
		} 
		cur++; 
	} 

	return NULL; 
} 

static void PrintEnv(char *label, char **envp)
{
	printf("%s:<br>\n<pre>\n", label);
	for ( ; *envp != NULL; envp++) {
		printf("%s\n", *envp);
	}
	printf("</pre><p>\n");
}

int main ()
{
	char **initialEnv = environ;
	int count = 0;


	while (FCGI_Accept() >= 0) {
		int mylen = atoi(getenv("CONTENT_LENGTH"));
		char *tmpBuf = (char*)malloc(mylen*sizeof(char));
		bzero(tmpBuf, mylen);
		char emp[45] = { 0 };
		char *context = (char*)malloc(mylen*sizeof(char));
		bzero(context, mylen);

		char *contentLength = getenv("CONTENT_LENGTH");
		int len;

		printf("Content-type: text/html\r\n"
				"\r\n"
				"<title>FastCGI echo</title>"
				"<h1>FastCGI echo</h1>\n"
				"Request number %d,  Process ID: %d<p>\n", ++count, getpid());

		if (contentLength != NULL) {
			len = strtol(contentLength, NULL, 10);
		}
		else {
			len = 0;
		}

		if (len <= 0) {
			printf("No data from standard input.<p>\n");
		}
		else {
			int i, ch;
			char *p1 = NULL ,*p2 = NULL;
			for(i = 0; i < len; ++i)
			{
				//读取字符
				ch = getchar();

				//将读出来的内容缓存子啊buf中
				tmpBuf[i] = ch;
			}
			
			p1 = strstr(tmpBuf, "filename");
			p1 = strchr(p1, '"');
			p1++;
			p2 = p1;
			p2 = strchr(p2, '"');
			p2--;
			//char *name = (char*)malloc(p2 - p1 + 5);
			char *name = (char*)malloc(p2 - p1 + 2);
			bzero(name, p2 - p1 + 2);
			//strcat(name, "../");
			strncat(name, p1, p2 - p1 + 1);
			//name[strlen(name)] = '\0';
			printf("***len = %d***filename = %s\n", strlen(name), name);
#if 1

			//把参照物截取出来	 ------WebKitFormBoundaryaTR45e74m129chya
			p1 = tmpBuf;
			p2 = strchr(tmpBuf, '\r');
			memcpy(emp, p1, p2 - p1);
			printf("emp = ***%s*%d**",emp, strlen(emp));

			p1 = strstr(tmpBuf, "Type:");
			p1 = strchr(p1, '\n');
			p1++;
			p1++;
			p1++;
			printf("str = %s\n", p1);


			printf("len = %d\n", strlen);


			printf("***********************len = %d\n", mylen - (p1 - tmpBuf));
			p2 = memstr(p1, mylen - (p1 - tmpBuf), emp);
			p2--;
			p2--;
			memcpy(context, p1, p2 - p1);
#endif	
			int fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0664);

			write(fd, context, p2 - p1);

			close(fd);

			int pfd[2];

			if (pipe(pfd) < 0) {
				printf("pip error\n");
			}  

			pid_t pid = fork();

			if(pid == 0)
			{
				//关闭读管道
				close(pfd[0]);

				//将标准输出重定向到写管道
				dup2(pfd[1], STDOUT_FILENO);

				execl("fastfds_client", "fastfds_client", name, NULL);

				return 0;
			}

			//关闭写管道
			close(pfd[1]);

			char url[256] = { 0 };

			//read from 读管道
			read(pfd[0], url, 256);

			close(pfd[0]);

			wait(NULL);

			printf("***url=%s***\n", url);

			free(tmpBuf);
			free(context);
			free(name);

			printf("\n</pre><p>\n");
		}

	} /* while */

	return 0;
}
