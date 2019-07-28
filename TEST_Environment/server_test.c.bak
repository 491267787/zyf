#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <pthread.h>

#define MYPORT  8887
#define QUEUE   1
#define BUFFER_SIZE 1024

char Rev_Buffer[BUFFER_SIZE];

void *pthread_run(void *arg)
{

    int fd = *(int *)arg;
    char *fp = &Rev_Buffer[0];
    printf("p----thread's pid = %d\n",(int)pthread_self());
    while(1)
    {
        memset(&Rev_Buffer[0],0,sizeof(Rev_Buffer));
        int len = recv(fd, &Rev_Buffer[0], sizeof(Rev_Buffer),0);
	if(0 < len)
	{
		printf("RCV=%s \n",fp);
	}
        if(strncmp(Rev_Buffer,"exit",strlen(Rev_Buffer)) == 0)
            break;
//        fputs(Rev_Buffer, stdout);
        send(fd, Rev_Buffer, len, 0);
    }
    pthread_exit(NULL);
//    close(*arg);
}

int main(int argc,char *argv[])
{
    void *condition_tmp;
    int pthread_num = 0;
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);


    int on = (argc != 1) ? 1 : 0; 
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
    	perror("setsockopt SO_REUSEPORT");
    }


    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
//    char Rev_Buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    pthread_t ptid[5];
    while(1)
    {
	pthread_num++;
	pthread_num = pthread_num > 4 ? 0 : pthread_num;
    	///成功返回非负描述字，出错返回-1
    	int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    	printf("a client has connected--%d\n",conn);
   	printf("client's address is %s\n",(char *)&client_addr);
    	if(conn<0)
    	{
       		perror("connect");
        	exit(1);
    	}

 
#if 1
    	int ret = pthread_create(&ptid[pthread_num],NULL,pthread_run,(void *)&conn);
    	printf("thread's pid = %d----%d\n",(int)ptid[pthread_num],(int)pthread_self());
    	if(0 > ret)
    	{
        	perror("pthread_create");
		return -1;
    	}
	
	sleep(10);


//	pthread_cancel(ptid[pthread_num-1]);//杀死
//    	pthread_join(ptid[pthread_num-1], (void**)&condition_tmp);

//    	printf("last线程的退出状态：condition = %d.\n", (int)condition_tmp);
    }

#endif

#if 0
    while(1)
    {
        memset(Rev_Buffer,0,sizeof(Rev_Buffer));
        int len = recv(conn, Rev_Buffer, sizeof(Rev_Buffer),0);
	if(0 < len)
	{
		printf("RCV=%s \n",Rev_Buffer);
	}
        if(strcmp(Rev_Buffer,"exit\n")==0)
            break;
//        fputs(Rev_Buffer, stdout);
        send(conn, Rev_Buffer, len, 0);
    }
    close(conn);
#endif
    close(server_sockfd);
    return 0;
}
