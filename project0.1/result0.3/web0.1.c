#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
char buff[BUFSIZ];
int main(int argc,char *argv[])
{
      int mode;
      char apname[30],appassword[30],staname[30],stapassword[30],pppoename[30],pppoepassword[30],usrname[30],usrpassword[30];

      char ledname[30],ledstatus[30];

      char operation[25];
      int sockfd,n;
      strcpy(operation,"web");
      struct sockaddr_in servaddr;
      
    //  printf("%d\n",argv[1]);


      mode=atoi(argv[1]);
      strcpy(apname,argv[2]);
      strcpy(appassword,argv[3]);
      strcpy(staname,argv[4]);
      strcpy(stapassword,argv[5]);
      strcpy(pppoename,argv[6]);
      strcpy(pppoepassword,argv[7]);
      strcpy(usrname,argv[8]);
      strcpy(usrpassword,argv[9]);

      strcpy(ledname,argv[10]);
      strcpy(ledstatus,argv[11]);
      


      if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
      printf("create socket error");
      exit(0);
      }

      memset(&servaddr,0,sizeof(servaddr));
      servaddr.sin_family=AF_INET;
      servaddr.sin_port=htons(5002);
  /*    if(inet_pton(AF_INET,argvv[1],&servaddr.sin_addr)<=0){
      printf("inet_pton error for %s\n",argvv[1]);
      exit(0);
      }  */
      char p[256];
      strcpy(p,"192.168.8.212");

      servaddr.sin_addr.s_addr=inet_addr(p);
      if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
      printf("connect error.\n");
      exit(0);
      }


	    struct json_object *info_object = NULL;
	    info_object =(struct json_object *)json_object_new_object();
	    if(info_object == NULL){printf("new json object falid.\n");return;}
	    json_object_object_add(info_object,"operation",json_object_new_string(operation));
	    json_object_object_add(info_object,"mode",json_object_new_int(mode));
	    json_object_object_add(info_object,"apname",json_object_new_string(apname));
	    json_object_object_add(info_object,"appassword",json_object_new_string(appassword));
	    json_object_object_add(info_object,"staname",json_object_new_string(staname));
	    json_object_object_add(info_object,"stapassword",json_object_new_string(stapassword));
	    json_object_object_add(info_object,"pppoename",json_object_new_string(pppoename));
	    json_object_object_add(info_object,"pppoepassword",json_object_new_string(pppoepassword));
	    json_object_object_add(info_object,"usrname",json_object_new_string(usrname));
	    json_object_object_add(info_object,"usrpassword",json_object_new_string(usrpassword));


	    json_object_object_add(info_object,"ledname",json_object_new_string(ledname));
	    json_object_object_add(info_object,"ledstatus",json_object_new_string(ledstatus));
	    

	    strcpy(buff,(char *)json_object_to_json_string(info_object));
	    send(sockfd,buff,sizeof(buff),0);
	    printf("提交成功！\n");
	    json_object_put(info_object);
      
}

