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
#include "comport.h"

#define MYPORT  8887
#define QUEUE   1
#define BUFFER_SIZE 1024

char Rev_Buffer[BUFFER_SIZE];

void *pthread_run(void *arg)
{

    int fd = *(int *)arg;
    char *fp = &Rev_Buffer[0];
    memset(&Rev_Buffer[0],0,BUFFER_SIZE);
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


/*
func:handle ethernet's data
*/
void *pthread_run_listen(void *arg)
{
    pthread_t ptid[4];
    int pthread_num = 0;
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);


    int on = 1;//(argc != 1) ? 1 : 0; 
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
    	perror("setsockopt SO_REUSEPORT");
	pthread_exit(0);
    }


    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
	pthread_exit(0);
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
	pthread_exit(0);
    }



    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    while(1)
    {
	pthread_num++;
	pthread_num = pthread_num > 3 ? 1 : pthread_num;
    	///成功返回非负描述字，出错返回-1
    	int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    	printf("a client has connected--%d\n",conn);
   	printf("client's address is %s\n",(char *)&client_addr);
    	if(conn<0)
    	{
       	    perror("client don't connect");
	    continue;
    	}

 
        int ret = pthread_create(&ptid[pthread_num],NULL,pthread_run,(void *)&conn);
    	printf("thread's pid = %d----%d\n",(int)ptid[pthread_num],(int)pthread_self());
    	if(0 > ret)
    	{
            perror("pthread_create");
    	}
	
	sleep(1);
    }
    close(server_sockfd);
}

/*
func:handle series data
*/
void *pthread_run_series(void *arg)
{


    char *devname = "/dev/ttyUSB0";
    unsigned long baudrate = 9600;
    COM_PORT *comport;

    char buf[buf_size];


    comport = Comport_Init(devname, baudrate, "8N1");
    if(NULL == comport)
    {   
        printf("init serial port failure\n");
        pthread_exit(NULL);                           
    }   

    Comport_open(comport);
    if(comport->fd <0) 
    {   
        printf("open error :%s\n",strerror(errno));
	sleep(200); 
        pthread_exit(NULL);                           
    }   

    printf("OPEN THE DEVICE\n");
    while(1)
    {   
        usleep(100000);
        if(read(comport->fd,buf,buf_size)<0)
        {
	   perror("read comport error");
	}
        printf("read is ok\n");
        printf("read : %s\n",buf);
        write(comport->fd,buf,buf_size);
        printf("write is ok\n");
        usleep(100000);
    }
    close(comport->fd);
    free(comport);
}

int main(int argc,char *argv[])
{
//    void *condition_tmp;


    pthread_t ptid[2];
    int seriespid_num = 0,listenpid_num = 1;
    int ret = pthread_create(&ptid[seriespid_num],NULL,pthread_run_series,0);
    if(0 > ret)
    {
        perror("pthread_create_series");
	return -1;
    }
    printf("series thread's pid = %d----%d\n",(int)ptid[seriespid_num],(int)pthread_self());
    

    ret = pthread_create(&ptid[listenpid_num],NULL,pthread_run_listen,0);
    if(0 > ret)
    {
        perror("pthread_create_listen");
	pthread_join(ptid[seriespid_num],NULL);
	return -1;
    }
    printf("listen thread's pid = %d----%d\n",(int)ptid[listenpid_num],(int)pthread_self());



    ///客户端套接字
//    char Rev_Buffer[BUFFER_SIZE];
//    struct sockaddr_in client_addr;
//    socklen_t length = sizeof(client_addr);


//    while(1)
//    {
//	printf("-------heart beating----%d",heartbeat++);
//    	sleep(10);
//    }

#if 0
	pthread_num++;
	pthread_num = pthread_num > 4 ? 1 : pthread_num;
    	///成功返回非负描述字，出错返回-1
    	int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    	printf("a client has connected--%d\n",conn);
   	printf("client's address is %s\n",(char *)&client_addr);
    	if(conn<0)
    	{
       		perror("connect");
        	exit(1);
    	}

 
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
    int inputchar = 0;
    while((inputchar = getchar()) != 'q');
    pthread_join(ptid[seriespid_num],NULL);
    pthread_join(ptid[listenpid_num],NULL);
    return 0;
}
