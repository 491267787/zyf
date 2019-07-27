#ifndef _COMPORT_H
#define _COMPORT_H

#include <stdio.h>    /*standard input and output definition*/
#include <string.h>
#include <stdlib.h>   /*standard function library definition*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <unistd.h>   /*UNIX standard function definition*/
#include <termios.h>  /* PPSIX terminal control definition*/
#include <fcntl.h>    /*file control definition*/
#include <errno.h>    /*error number definition*/
#include <sys/wait.h>

#define DEVNAME_LEN   64
#define buf_size     4096
typedef struct _COM_PORT
{
  unsigned char databit,parity,stopbit,flowctrl,is_connect;
  char dev_name[DEVNAME_LEN];
  int fd;
  int frag_size;
  long baudrate;
} COM_PORT;

COM_PORT  *Comport_Init(const char *dev_name,unsigned long baudrate, const char *settings);
void Setting_Comport(COM_PORT *comport, const char *settings);
void Comport_close(COM_PORT *comport);
int Comport_open(COM_PORT *comport);




#endif

