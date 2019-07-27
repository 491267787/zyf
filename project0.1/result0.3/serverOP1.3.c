#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#include<sys/stat.h>

#define MAXLINE BUFSIZ


union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};

static int sem_id = 0;

static int set_semvalue();
static void del_semvalue();
static int semaphore_p();
static int semaphore_v();

FILE *fp1 = NULL;
union semun sem_union;
struct sembuf sem_b;

int mode,ret;

int val_key;

char apname[30],appassword[30],staname[30],stapassword[30],pppoename[30],pppoepassword[30],usrname[30],usrpassword[30];

char ledname[30],ledstatus[30];

char sql[128];char *errmsg;
sqlite3 *db;

void *THREAD_FUNCTION(void *arg){
	int fd=*(int *)arg;


//	while(1){
		struct json_object *jiebao_json,*jiebao_operation,*jiebao_mode,*jiebao_apname,*jiebao_appassword,*jiebao_staname,*jiebao_stapassword,*jiebao_pppoename,*jiebao_pppoepassword,*jiebao_usrname,*jiebao_usrpassword,*jiebao_ledname,*jiebao_ledstatus=NULL;
		//    int PID,age,ret;
		//    char sql[128];char *errmsg;
		//    sqlite3 *db;
		sqlite3_open("test4.db",&db);
		char *buff;
		long int n;
		char name[16];
		//    memset(buff,0,MAXLINE);
		buff = (char *)malloc(BUFSIZ);
		bzero(buff,MAXLINE);
		n= recv(fd,buff,MAXLINE,0);     //接受数据
		printf("-----n=%ld\n",n);

		/*出现异常错误 */
		if(n<0){printf("recv erro.\n");pthread_exit(0);}

		//   printf("%ld\n",sizeof(buff));
		//   buff[n-1]='\0';
		//  if(strcmp(buff,"quit")==0)    close(db);
		else if(n==0)        //对方断开连接
		{    json_object_put(jiebao_json);
			json_object_put(jiebao_operation); 
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

			pthread_exit(0);
		}   
//		buff[n]='\0';
		//解包
		jiebao_json = (struct json_object *)json_tokener_parse(buff);
		jiebao_operation = (struct json_object *)json_object_object_get(jiebao_json,"operation");
		char opt[25];
		strcpy(opt,(char *)json_object_get_string(jiebao_operation));


     /*
		// 创建信号量 
		sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
		// 程序第一次被调用，初始化信号量 
		if(!set_semvalue())
		{
			fprintf(stderr, "Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}		    
      */

#if 0
		int a;
		if(a = semget((key_t)1234, 1, 0666 | IPC_CREAT | IPC_EXCL) != -1)
		// 程序第一次被调用，初始化信号量 
		{	
			sem_id = a;
			set_semvalue();
		}
		
		else 
			sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
#endif
		//存储web端的操作数据
		if(!strcmp(opt,"web"))
		{   
			printf("val=%d",val_key);
                        //获取信号量
			if(sem_id = semget((key_t)val_key, 1, 0666 | IPC_CREAT | IPC_EXCL) == -1)
			
			{
			sem_id = semget((key_t)val_key, 1, 0666 | IPC_CREAT);

			jiebao_mode = (struct json_object *)json_object_object_get(jiebao_json,"mode");
			mode = json_object_get_int(jiebao_mode);    //获取mode，0为关闭wifi,1为AP模式，2为STA模式
			printf("mode:%d\t",mode);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name=%d where flag='mode' ;",mode);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}

			jiebao_apname = (struct json_object *)json_object_object_get(jiebao_json,"apname");
			strcpy(apname,(char *)json_object_get_string(jiebao_apname));    //获取apname
			printf("apname:%s\t",apname);

			jiebao_appassword = (struct json_object *)json_object_object_get(jiebao_json,"appassword");
			strcpy(appassword,(char *)json_object_get_string(jiebao_appassword));    //获取appassword
			printf("appassword:%s\n",appassword);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='ap' ;",apname,appassword);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}

			jiebao_staname = (struct json_object *)json_object_object_get(jiebao_json,"staname");
			strcpy(staname,(char *)json_object_get_string(jiebao_staname));    //获取staname
			printf("staname:%s\t",staname);

			jiebao_stapassword = (struct json_object *)json_object_object_get(jiebao_json,"stapassword");
			strcpy(stapassword,(char *)json_object_get_string(jiebao_stapassword));    //获取stapassword
			printf("stapassword:%s\n",stapassword);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='sta' ;",staname,stapassword);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}

			jiebao_pppoename = (struct json_object *)json_object_object_get(jiebao_json,"pppoename");
			strcpy(pppoename,(char *)json_object_get_string(jiebao_pppoename));    //获取pppoename
			printf("pppoename:%s\t",pppoename);

			jiebao_pppoepassword = (struct json_object *)json_object_object_get(jiebao_json,"pppoepassword");
			strcpy(pppoepassword,(char *)json_object_get_string(jiebao_pppoepassword));    //获取pppoepassword
			printf("pppoepassword:%s\n",pppoepassword);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='pppoe' ;",pppoename,pppoepassword);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}

			jiebao_usrname = (struct json_object *)json_object_object_get(jiebao_json,"usrname");
			strcpy(usrname,(char *)json_object_get_string(jiebao_usrname));    //获取usrname
			printf("usrname:%s\t",usrname);

			jiebao_usrpassword = (struct json_object *)json_object_object_get(jiebao_json,"usrpassword");
			strcpy(usrpassword,(char *)json_object_get_string(jiebao_usrpassword));    //获取usrpassword
			printf("usrpassword:%s\n",usrpassword);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='usr' ;",usrname,usrpassword);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}


			jiebao_ledname = (struct json_object *)json_object_object_get(jiebao_json,"ledname");
			strcpy(ledname,(char *)json_object_get_string(jiebao_ledname));    //获取ledname
			printf("ledname:%s\t",ledname);

			jiebao_ledstatus = (struct json_object *)json_object_object_get(jiebao_json,"ledstatus");
			strcpy(ledstatus,(char *)json_object_get_string(jiebao_ledstatus));    //获取ledstatus
			printf("ledstatus:%s\n",ledstatus);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='led' ;",ledname,ledstatus);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}


//			sqlite3_close(db);
		//	free(buff);
			json_object_put(jiebao_json);
			json_object_put(jiebao_operation); 
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
			printf("-------更新数据成功-------\n");      



			semaphore_v();
			}

		else{
		

			jiebao_mode = (struct json_object *)json_object_object_get(jiebao_json,"mode");
			mode = json_object_get_int(jiebao_mode);    //获取mode，1为AP模式，2为STA模式
			printf("mode:%d\t",mode);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name=%d where flag='mode' ;",mode);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}

			jiebao_apname = (struct json_object *)json_object_object_get(jiebao_json,"apname");
			strcpy(apname,(char *)json_object_get_string(jiebao_apname));    //获取apname
			printf("apname:%s\t",apname);

			jiebao_appassword = (struct json_object *)json_object_object_get(jiebao_json,"appassword");
			strcpy(appassword,(char *)json_object_get_string(jiebao_appassword));    //获取appassword
			printf("appassword:%s\n",appassword);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='ap' ;",apname,appassword);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}

			jiebao_staname = (struct json_object *)json_object_object_get(jiebao_json,"staname");
			strcpy(staname,(char *)json_object_get_string(jiebao_staname));    //获取staname
			printf("staname:%s\t",staname);

			jiebao_stapassword = (struct json_object *)json_object_object_get(jiebao_json,"stapassword");
			strcpy(stapassword,(char *)json_object_get_string(jiebao_stapassword));    //获取stapassword
			printf("stapassword:%s\n",stapassword);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='sta' ;",staname,stapassword);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}

			jiebao_pppoename = (struct json_object *)json_object_object_get(jiebao_json,"pppoename");
			strcpy(pppoename,(char *)json_object_get_string(jiebao_pppoename));    //获取pppoename
			printf("pppoename:%s\t",pppoename);

			jiebao_pppoepassword = (struct json_object *)json_object_object_get(jiebao_json,"pppoepassword");
			strcpy(pppoepassword,(char *)json_object_get_string(jiebao_pppoepassword));    //获取pppoepassword
			printf("pppoepassword:%s\n",pppoepassword);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='pppoe' ;",pppoename,pppoepassword);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}

			jiebao_usrname = (struct json_object *)json_object_object_get(jiebao_json,"usrname");
			strcpy(usrname,(char *)json_object_get_string(jiebao_usrname));    //获取usrname
			printf("usrname:%s\t",usrname);

			jiebao_usrpassword = (struct json_object *)json_object_object_get(jiebao_json,"usrpassword");
			strcpy(usrpassword,(char *)json_object_get_string(jiebao_usrpassword));    //获取usrpassword
			printf("usrpassword:%s\n",usrpassword);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='usr' ;",usrname,usrpassword);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}


			jiebao_ledname = (struct json_object *)json_object_object_get(jiebao_json,"ledname");
			strcpy(ledname,(char *)json_object_get_string(jiebao_ledname));    //获取ledname
			printf("ledname:%s\t",ledname);

			jiebao_ledstatus = (struct json_object *)json_object_object_get(jiebao_json,"ledstatus");
			strcpy(ledstatus,(char *)json_object_get_string(jiebao_ledstatus));    //获取ledstatus
			printf("ledstatus:%s\n",ledstatus);
			memset(sql,0,128);
			sprintf(sql,"update config_info set name='%s',password='%s' where flag='usr' ;",ledname,ledstatus);
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
				printf("update:%s\n",errmsg);
				pthread_exit(0);
			}


//			sqlite3_close(db);
		//	free(buff);
			json_object_put(jiebao_json);
			json_object_put(jiebao_operation); 
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
			printf("-------更新数据成功-------\n");      
		}
		} 


#if 1	
		//将web端发的命令发送给路由器
		if(!strcmp(opt,"machine"))
		{ 
			char buff1[4096],buff2[4096];
			char ta[30],tb[30],tc[30],td[30],te[30],tf[30],tg[30],th[30],ti[30];
			char tj[30],tk[30];

			val_key=rand();
			printf("val=%d",val_key);
			set_semvalue();
			sem_id = semget((key_t)val_key, 1, 0666 | IPC_CREAT);

			while(1){

			if( (fp1 = fopen("cache.txt","r")) == NULL){
				printf("File cannot be opened\n");
				pthread_exit(0);
			}

			int i,j,index,ret,nrow,ncolumn;
			char **resultSet=NULL;
			memset(buff,0,BUFSIZ);
			memset(sql,0,128);
			sprintf(sql,"select * from config_info");
			ret = sqlite3_get_table(db,sql,&resultSet,&nrow,&ncolumn,0);
			if(ret != SQLITE_OK){
				fprintf(stderr,"select err:%s\n",sqlite3_errmsg(db));
				return;
			}
			index = ncolumn;
			struct json_object *info_object = NULL;
			info_object = (struct json_object *)json_object_new_object();
			if(info_object == NULL){printf("new json object failed.\n");return;}
			for(j=0;j<nrow;j++){
			for(i=0;i<ncolumn;i++){
				printf("字段名：%s >字段值：%s\n",resultSet[i],resultSet[index]);
				if(strcmp(resultSet[index],"mode") == 0)
				{ json_object_object_add(info_object,"mode",json_object_new_int(atoi(resultSet[index+1])));
				strcpy(ta,resultSet[index+1]);
				}

				if(strcmp(resultSet[index],"ap") == 0)
				{json_object_object_add(info_object,"apname",json_object_new_string(resultSet[index+1]));
				 json_object_object_add(info_object,"appassword",json_object_new_string(resultSet[index+2]));
				strcpy(tb,resultSet[index+1]);
				strcpy(tc,resultSet[index+2]);
				}	

				if(strcmp(resultSet[index],"sta") == 0)
				{json_object_object_add(info_object,"staname",json_object_new_string(resultSet[index+1]));
				 json_object_object_add(info_object,"stapassword",json_object_new_string(resultSet[index+2]));
				strcpy(td,resultSet[index+1]);
				strcpy(te,resultSet[index+2]);
				}

				if(strcmp(resultSet[index],"pppoe") == 0)
				{json_object_object_add(info_object,"pppoename",json_object_new_string(resultSet[index+1]));
				 json_object_object_add(info_object,"pppoepassword",json_object_new_string(resultSet[index+2]));
				strcpy(tf,resultSet[index+1]);
				strcpy(tg,resultSet[index+2]);
				}

				if(strcmp(resultSet[index],"usr") == 0)
				{json_object_object_add(info_object,"usrname",json_object_new_string(resultSet[index+1]));
				 json_object_object_add(info_object,"usrpassword",json_object_new_string(resultSet[index+2]));
				strcpy(th,resultSet[index+1]);
				strcpy(ti,resultSet[index+2]);
				}	

				if(strcmp(resultSet[index],"led") == 0)
				{json_object_object_add(info_object,"ledname",json_object_new_string(resultSet[index+1]));
				 json_object_object_add(info_object,"ledstatus",json_object_new_string(resultSet[index+2]));
				strcpy(tj,resultSet[index+1]);
				strcpy(tk,resultSet[index+2]);
				}	

				index++;
			}
			}
			memset(buff1,0,4096);
			memset(buff2,0,4096);
			sprintf(buff1,"%s %s %s %s %s %s %s %s %s %s %s",ta,tb,tc,td,te,tf,tg,th,ti,tj,tk);
			printf("buff1=%s\n",buff1);
			while(fgets(buff2,4096,fp1));
			fclose(fp1);
			printf("buff2=%s\n",buff2);


//			printf("------------1------------\n");
			memset(buff,0,BUFSIZ);
			strcpy(buff,(char *)json_object_to_json_string(info_object));
		        buff[BUFSIZ-1]='\0';
//			printf("------------2----%d--------\n",strlen(buff));

                        //所发数据与上一次发送数据不同时则将发送记录跟新，发送数据;相同时则不发送数据
			if(strcmp(buff1,buff2) != 0){         
			
			if((fp1=fopen("cache.txt","w"))==NULL){
				printf("File cannot be opened\n");
				pthread_exit(0);
			}
			fputs(buff1,fp1);
			fclose(fp1);
			send(fd,buff,BUFSIZ,0);
			printf("提交成功!\n");

			}
			
//			printf("------------3------------\n");
			json_object_put(info_object);

			
	//		sleep(10000);
			semaphore_p();
			printf("homeway is coming!\n");

			}


		}

#endif

		/*      //删除用户

			if(!strncmp(opt,"d",1))
			{   
			jiebao_PID = (struct json_object *)json_object_object_get(jiebao_json,"PID");
			int pid;
			pid = json_object_get_int(jiebao_PID);    //获取PID
			memset(sql,0,128);
			sprintf(sql,"delete from cli_info where PID=pid");
			ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);        //添加到数据库
			if(SQLITE_OK != ret){
			printf("insert:%s\n",errmsg);
			pthread_exit(0);
			}
			sqlite3_close(db);
			free(buff);
			json_object_put(jiebao_json);
			json_object_put(jiebao_operation); 
			json_object_put(jiebao_PID);
			json_object_put(jiebao_age);
			json_object_put(jiebao_name);
			printf("-------删除成功-------\n");      
			}


		//查询推送个人信息
		if(!strncmp(opt,"p",1))
		{   
		jiebao_PID = (struct json_object *)json_object_object_get(jiebao_json,"PID");
		int pid,nRow,nColumn,i,j,index;
		char **dbResult;
		char spid[BUFSIZ];
		pid = json_object_get_int(jiebao_PID);    //获取PID
		//     strcpy(spid,(char *)pid);
		sprintf(spid,"%d",pid);       
		memset(sql,0,128);
		sprintf(sql,"select * from cli_info");
		ret=sqlite3_get_table(db,sql,&dbResult,&nRow,&nColumn,&errmsg);        //添加到数据库
		if(SQLITE_OK == ret){        //查询成功

		index = nColumn;  //数据的起始点,dbRusult数据中的前n个为字段名
		printf("查到%d条记录\n",nRow);
		for(i=0;i<nRow;i++){
		printf("第%d条记录\n",i+1);

		for(j=0;j<nColumn;j++){
		printf("字段名：%s>字段值：%s\n",dbResult[j],dbResult[index]);
#if 1	
if(!(strcmp(dbResult[j], "PID") || strcmp(dbResult[index], spid))){ //找到了对象
bzero(buff,BUFSIZ);
int age;
age = atoi(dbResult[index+1]);
struct json_object *P_info_object = NULL;
P_info_object = (struct json_object *)json_object_new_object();
if(P_info_object == NULL){
printf("new json object failed.\n");
return;}
json_object_object_add(P_info_object,"operation",json_object_new_string("m"));
json_object_object_add(P_info_object,"PID",json_object_new_int(pid));
json_object_object_add(P_info_object,"age",json_object_new_int(age));
json_object_object_add(P_info_object,"name",json_object_new_string(dbResult[index+2]));
strcpy(buff,(char *)json_object_to_json_string(P_info_object));

n=strlen(buff);
printf("---------strlen(buff)=%ld\n",n);

send(fd,buff, n, 0);
printf("已发送所需信息.\n");
json_object_put(P_info_object);
}

#endif			
index++;
}
printf("\n");

}

}
sqlite3_free_table(dbResult);

sqlite3_close(db);
free(buff);
json_object_put(jiebao_json);
json_object_put(jiebao_operation); 
json_object_put(jiebao_PID);
json_object_put(jiebao_age);
json_object_put(jiebao_name);
printf("-------推送个人信息成功-------\n");      
}


*/
//           }               

}

int main(int argc,char **argv[]){

	int sockfd,connfd,ret;
	struct sockaddr_in servaddr;

	//      char sql[128];
	//      sqlite3 *db;
	sqlite3_open("test4.db",&db);
	memset(sql,0,128);
	/*新建一张表*/
	sprintf(sql,"create table if not exists config_info(flag char,name char,password char)");
	ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(SQLITE_OK != ret){
		printf("create:%s\n",errmsg);
		exit(0);
	}
	sqlite3_close(db);
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
		printf("create socket error.");
		exit(0);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5002);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	/*快速地址复用 */
	int on=1;  
	if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
	{  
		perror("setsockopt failed");  
		exit(EXIT_FAILURE);
	} 

	/*绑定 */
	if( bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
		perror("bind");
		exit(0);
	}
	//    int flags=fcntl(sockfd,F_GETFL,0);
	//    fcntl(sockfd, F_SETFL, flags&~O_NONBLOCK); //设为阻塞
	/*监听 */
	if(listen(sockfd,5)<0) {
		printf("listen socket error.");
		exit(0);
	}

	printf("------waiting for client's request-----\n");
	while(1){
		if((connfd= accept(sockfd,(struct sockaddr*)NULL,NULL))<0){    //等待客户端连接
			printf("accept socket error.\n");
			continue;
		}
		printf("connect success!\n");
		//创建分离线程
		pthread_t tid;
		tid = rand();
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		ret=pthread_create(&tid,&attr,THREAD_FUNCTION,&connfd);
		if(ret < 0){
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
		//  sleep(2);

		//  close(connfd);
	}
	close(sockfd);

}



static int set_semvalue()
{
	/* 用于初始化信号量，在使用信号量前必须这样做 */
//	union semun sem_union;

	sem_union.val = 0;
	if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
	{
		return 0;
	}
	return 1;
}

static void del_semvalue()
{
	/* 删除信号量 */
//	union semun sem_union;

	if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
	{
		fprintf(stderr, "Failed to delete semaphore\n");
	}
}

static int semaphore_p()
{
	/* 对信号量做减1操作，即等待P（sv）*/
//	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;//P()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return 0;
	}
	return 1;
}

static int semaphore_v()
{
	/* 这是一个释放操作，它使信号量变为可用，即发送信号V（sv）*/
//	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;//V()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semaphore_v failed\n");
		return 0;
	}
	return 1;
}


