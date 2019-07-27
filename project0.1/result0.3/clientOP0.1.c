#include<stdio.h>

#include<termios.h>
#include<unistd.h>
#include<fcntl.h>

#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
int main(int argc,char *argv[])
{

      char ip[126];
      char buff[BUFSIZ];
      int sockfd,n;
      int mode;
      char apname[30],appassword[30],staname[30],stapassword[30],pppoename[30],pppoepassword[30],usrname[30],usrpassword[30];

      char ledname[30],ledstatus[30];

      struct sockaddr_in servaddr;
      if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
      printf("create socket error");
      exit(0);
      }

      memset(&servaddr,0,sizeof(servaddr));
      servaddr.sin_family=AF_INET;
      servaddr.sin_port=htons(5002);
  /*    if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0){
      printf("inet_pton error for %s\n",argv[1]);
      exit(0);
      }  */
      strcpy(ip,"192.168.8.212");      

      servaddr.sin_addr.s_addr=inet_addr(ip);
      if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
      printf("connect error.\n");
      exit(0);
      }

	    struct json_object *info_object = NULL;
	    info_object =(struct json_object *)json_object_new_object();
	    if(info_object == NULL){printf("new json object falid.\n");return;}
	    json_object_object_add(info_object,"operation",json_object_new_string("machine"));
	    memset(buff,0,sizeof(buff));
	    strcpy(buff,(char *)json_object_to_json_string(info_object));
//	    printf("%ld",sizeof(buff));
  //        char test[]="homeway test";
	    send(sockfd,buff,sizeof(buff),0);
	    printf("提交成功！\n");
	    json_object_put(info_object);
//	    close(sockfd);
	    
	    //读取服务器发送的数据
	    while(1){
	    struct json_object *jiebao_json,*jiebao_mode,*jiebao_apname,*jiebao_appassword,*jiebao_staname,*jiebao_stapassword,*jiebao_pppoename,*jiebao_pppoepassword,*jiebao_usrname,*jiebao_usrpassword,*jiebao_ledname,*jiebao_ledstatus=NULL;
	    memset(buff,0,sizeof(buff));
	    n= recv(sockfd,buff,BUFSIZ,0);
	 printf("-------n=%d-----\n",n);

	    //出现异常
	    if(n<0){printf("recv erro.\n");exit(0);}
	    else if(n == 0)   //断开连接
            {
	    	json_object_put(jiebao_json);
	    	json_object_put(jiebao_mode);
	    	json_object_put(jiebao_apname);
	    	json_object_put(jiebao_appassword);
	    	json_object_put(jiebao_staname);
	    	json_object_put(jiebao_stapassword);
	    	json_object_put(jiebao_pppoename);
	    	json_object_put(jiebao_pppoepassword);
	    	json_object_put(jiebao_usrname);
	    	json_object_put(jiebao_usrpassword);
	    
		json_object_put(jiebao_ledname);
	    	json_object_put(jiebao_ledstatus);
	
		printf("已断开连接！\n");
		exit(0);		
	    }
//	    buff[n]='\0';
	    //解包

	    jiebao_json = (struct json_object *)json_tokener_parse(buff);
	    jiebao_mode = (struct json_object *)json_object_object_get(jiebao_json,"mode");
	    mode = json_object_get_int(jiebao_mode);
	    printf("mode:%d\t",mode);    //0为关闭wifi,1为AP模式，2为STA模式

	    jiebao_apname = (struct json_object *)json_object_object_get(jiebao_json,"apname");
	    strcpy(apname,(char *)json_object_get_string(jiebao_apname));
	    printf("apname:%s\t",apname);
	    jiebao_appassword = (struct json_object *)json_object_object_get(jiebao_json,"appassword");
	    strcpy(appassword,(char *)json_object_get_string(jiebao_appassword));
	    printf("appassword:%s\n",appassword);

	    jiebao_staname = (struct json_object *)json_object_object_get(jiebao_json,"staname");
	    strcpy(staname,(char *)json_object_get_string(jiebao_staname));
	    printf("staname:%s\n",staname);
	    jiebao_stapassword = (struct json_object *)json_object_object_get(jiebao_json,"stapassword");
	    strcpy(stapassword,(char *)json_object_get_string(jiebao_stapassword));
	    printf("stapassword:%s\n",stapassword);

	    jiebao_pppoename = (struct json_object *)json_object_object_get(jiebao_json,"pppoename");
	    strcpy(pppoename,(char *)json_object_get_string(jiebao_pppoename));
	    printf("pppoename:%s\n",pppoename);
	    jiebao_pppoepassword = (struct json_object *)json_object_object_get(jiebao_json,"pppoepassword");
	    strcpy(pppoepassword,(char *)json_object_get_string(jiebao_pppoepassword));
	    printf("pppoepassword:%s\n",pppoepassword);

	    jiebao_usrname = (struct json_object *)json_object_object_get(jiebao_json,"usrname");
	    strcpy(usrname,(char *)json_object_get_string(jiebao_usrname));
	    printf("usrname:%s\n",usrname);
	    jiebao_usrpassword = (struct json_object *)json_object_object_get(jiebao_json,"usrpassword");
	    strcpy(usrpassword,(char *)json_object_get_string(jiebao_usrpassword));
	    printf("usrpassword:%s\n",usrpassword);
	    
	    
	    jiebao_ledname = (struct json_object *)json_object_object_get(jiebao_json,"ledname");
	    strcpy(ledname,(char *)json_object_get_string(jiebao_ledname));
	    printf("ledname:%s\n",ledname);
	    jiebao_ledstatus = (struct json_object *)json_object_object_get(jiebao_json,"ledstatus");
	    strcpy(ledstatus,(char *)json_object_get_string(jiebao_ledstatus));
	    printf("ledstatus:%s\n",ledstatus);
	
#if 1 
	   
         /*
	   char ledst[30];
	   memset(ledst,0,30);
	   sprintf(ledst,"echo %s%s > /dev/ttyS0",ledname,ledstatus);
	   system(ledst);
	*/


	   if(strcmp(pppoename,"0")){
	   
	    memset(buff,0,sizeof(buff));
	    sprintf(buff,"/app/pppoe.sh %s %s",pppoename,pppoepassword);
	    system(buff);   
	   }
        



	struct termios opt;
	char buf11[1024];
	int fdled=open( "/dev/ttyS0",O_RDWR|O_NOCTTY|O_NDELAY);//打开串口
	opt.c_cflag|=CLOCAL|CREAD; //激活选项有CLOCAL和CREAD
	cfsetispeed(&opt, B9600);//设置波特率，使用函数cfsetispeed、 cfsetospee
	cfsetospeed(&opt, B9600);
	opt.c_cflag&=~CSIZE; //设置数据位，需使用掩码设置。
	opt.c_cflag|=CS8;
	opt.c_cflag&=~ PARENB;//不使用奇偶校验
	opt.c_cflag &=~CSTOPB;//设置停止位
	opt.c_cc[VTIME]=0;//设置最少字符和等待时间
	opt.c_cc[VMIN]=0;
	tcflush(fdled,TCIOFLUSH);//处理要写入的引用对象
	tcsetattr(fdled,TCSANOW,&opt);//激活配置。
	fcntl(fdled, F_SETFL, 0);//恢复串口的状态为阻塞状态，用于等待串口?	
       
	isatty(STDIN_FILENO);//测试打开的文件描述府是否引用一个终端设备，以进一步确认串
	memset(buf11,0,1024);
        sprintf(buf11,"%s%s",ledname,ledstatus)
	write(fdled,buf11,strlen(buf11));




	   if(mode==0){
	   
	       memset(buff,0,sizeof(buff));
	    sprintf(buff,"/app/nowifi.sh start");
	    system(buff);   
	  //  free(buff);
	   }
	   if(mode==1){
	   
	       memset(buff,0,sizeof(buff));
	    sprintf(buff,"/app/ap.sh %s %s",apname,appassword);
	    system(buff);   
	  //  free(buff);
	   }
	   if(mode==2){
	   
	       memset(buff,0,sizeof(buff));
	    sprintf(buff,"/app/sta.sh %s %s",staname,stapassword);
	    system(buff);   
	  //  free(buff);
	   }

	
#endif

	    	json_object_put(jiebao_json);
	    	json_object_put(jiebao_mode);
	    	json_object_put(jiebao_apname);
	    	json_object_put(jiebao_appassword);
	    	json_object_put(jiebao_staname);
	    	json_object_put(jiebao_stapassword);
	    	json_object_put(jiebao_pppoename);
	    	json_object_put(jiebao_pppoepassword);
	    	json_object_put(jiebao_usrname);
	    	json_object_put(jiebao_usrpassword);
	    
		json_object_put(jiebao_ledname);
	    	json_object_put(jiebao_ledstatus);

      }

}

/*
void deleteCl(int arg){
	int sockfd = arg;
	while(1){
        system("clear");
	printf("请输入将删除信息的PID:");

	scanf("%d",&id);
//	id=12;

	while(getchar()!='\n');
	printf("\n");
	printf("是否提交（y/n）,按b可返回首页,任意健返回：");

	scanf("%c",&choice);
//        choice='y' ;

	if(choice == 'b')break;
	printf("\n");
	switch(choice){
		case 'y':
			{struct json_object *info_object = NULL;
	    		info_object =(struct json_object *)json_object_new_object();
	    		if(info_object == NULL){printf("new json object falid.\n");return;}
	   		json_object_object_add(info_object,"operation",json_object_new_string("d"));
	    		json_object_object_add(info_object,"PID",json_object_new_int(id));
			strcpy(buff,(char *)json_object_to_json_string(info_object));
			send(sockfd,buff,sizeof(buff),0);
			printf("提交成功。\n");
			json_object_put(info_object);
			sleep(2);
			system("clear");}
			break;
		defult :
		       continue;	
			
	}	
}
}


void listCl(int arg){
	int n;
	int sockfd = arg;
        system("clear");
	printf("1.PID号查找个人信息        2.查找全部客户信息\n");
	printf("3.退出\n");
	printf("请输入您的选择：");

	scanf("%d",&n);
//	n=1;

	printf("\n");
        switch(n){
		case 1:
			{
			printf("请输入需要查找的PID号：");

			scanf("%d",&id);
//			id=12;

			struct json_object *info_object = NULL;
	    		info_object =(struct json_object *)json_object_new_object();
	    		if(info_object == NULL){printf("new json object falid.\n");return;}
	   		json_object_object_add(info_object,"operation",json_object_new_string("p"));
	    		json_object_object_add(info_object,"PID",json_object_new_int(id));
			strcpy(buff,(char *)json_object_to_json_string(info_object));
			send(sockfd,buff,sizeof(buff),0);
			printf("提交成功。\n");
			json_object_put(info_object);
			sleep(1);
			system("clear");}
			break;





	}
//	memset(buff,'\n',BUFSIZ);
//        printf("------1---------sizeof(buff):%lu\n",sizeof(buff));

        bzero(buff,BUFSIZ);

        //n=recv(sockfd,buff,BUFSIZ,0);
        n=recv(sockfd,buff,BUFSIZ,0);
	printf("n=%d buff:%s\n",n,buff);
	buff[n]='\0';

//        printf("------2---------\n");
	struct json_object *jiebao_json,*jiebao_operation,*jiebao_PID,*jiebao_age,*jiebao_name=NULL;
	jiebao_json = (struct json_object *)json_tokener_parse(buff);
	jiebao_operation = (struct json_object *)json_object_object_get(jiebao_json,"operation");
	char *opt;
	opt = (char *)json_object_get_string(jiebao_operation);
//        printf("------3---------\n");

	if(!strncmp(opt,"m",1)){
//        	printf("------4---------\n");
		jiebao_PID = (struct json_object *)json_object_object_get(jiebao_json,"PID");
		id  = json_object_get_int(jiebao_PID);
		printf("PID:%d\t",id);
		jiebao_age = (struct json_object *)json_object_object_get(jiebao_json,"age");
		age = json_object_get_int(jiebao_age);
		printf("age:%d\t",age);
		jiebao_name = (struct json_object *)json_object_object_get(jiebao_json,"name");
		strcpy(name,(char *)json_object_get_string(jiebao_name));
		printf("name:%s\n",name);
		printf("\n\n\n按enter键返回：\n");
		while(getchar()!= '\n');
		while(getchar()!= '\n');


	}

	json_object_put(jiebao_json);
	json_object_put(jiebao_operation);
	json_object_put(jiebao_PID);
	json_object_put(jiebao_age);
	json_object_put(jiebao_name);

	
}

*/
