#include "comport.h"

COM_PORT *Comport_Init(const char *dev_name, unsigned long baudrate, const char *settings)
{
    COM_PORT *comport = NULL;
    if(NULL == (comport = (COM_PORT *)malloc(sizeof(COM_PORT))))
    {
        return NULL;
    }
    memset(comport, 0, sizeof(COM_PORT));
    comport->is_connect = 0;
    comport->frag_size = 128;

    strncpy(comport->dev_name, dev_name, DEVNAME_LEN);
    comport->baudrate = baudrate;

    Setting_Comport(comport, settings);

    return comport;
}

void Setting_Comport(COM_PORT *comport, const char *settings)
{
    if(NULL == comport || NULL == settings)
       return;

    switch(settings[0])   /* databit */
    {
      case '7':
        comport->databit = 7;
        break;
      case '8':
        comport->databit = 8;
        break;
      default:
        comport->databit = 8;
        break;
    }


    switch(settings[1])  /*parity check*/
    {
      case 'O':    /*Odd check*/
      case 'o':
        comport->parity = 1;
        break;
      case 'E':    /* Even check */
      case 'e':
        comport->parity = 2;
        break;
      case 'N':   /* None parity check */
      case 'n':
      default:
        comport->parity = 0;
        break;
    }

    switch(settings[2])  /*stop bit*/
    {
      case '1':
        comport->stopbit = 1;
        break;
      case '2':
      default:
        comport->stopbit = 2;
        break;
    }
}

void Comport_close(COM_PORT *comport)
{
    if(0 != comport->fd)
      close(comport->fd);

    comport->is_connect = 0x00;
    comport->fd = -1;

}

int Comport_open(COM_PORT *comport)
{
    int retval = -1;
    struct termios old_cfg, new_cfg;
    long temp;

    if(NULL == comport)
        return -1;

    Comport_close(comport);

    /* not a TTY device */
    if(!strstr(comport->dev_name, "tty"))
    {
        printf("open not tty device \"%s\".\n",comport->dev_name);
        comport->fd = open(comport->dev_name, O_RDWR);
        retval = comport->fd <0 ? -2 : comport->fd;
        goto CleanUp;
    }
    comport->fd = open(comport->dev_name,O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(comport->fd <0)
    {
        retval = -3;
        goto CleanUp;
    }
    printf("open device \"%s\"\n",comport->dev_name);

    if(0 != tcgetattr(comport->fd, &old_cfg))
    {
        retval = -4;  /* failed to get com settings*/
        goto CleanUp;
    }

    memset(&new_cfg, 0, sizeof(new_cfg));

    /*  Configure comport */

    new_cfg.c_cflag &= ~CSIZE; /*character length is 0 */
    new_cfg.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    new_cfg.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    new_cfg.c_oflag &= ~(OPOST);

    /* set the databit */
    switch(comport->databit)
    {
      case 0x07:
        new_cfg.c_cflag |= CS7;
        break;
      case 0x08:
      default:
        new_cfg.c_cflag |= CS8;
        break;
    }

    /* set the parity */
    switch(comport->parity)
    {
      case 0x01:   //Odd check
          new_cfg.c_cflag |= PARENB;
          new_cfg.c_iflag |= (INPCK | ISTRIP);
          new_cfg.c_cflag |= PARODD;
          break;
      case 0x02:  //Even check
          new_cfg.c_cflag |= PARENB;
          new_cfg.c_iflag |= (INPCK | ISTRIP);
          new_cfg.c_cflag &= ~PARODD;
          break;
      case 0x00:
      default:
          new_cfg.c_cflag &= ~PARENB;
          break;
    }

    /*  set stop bit */
    if(0x01 !=comport->stopbit)
        new_cfg.c_cflag != CSTOPB;
    else 
        new_cfg.c_cflag &= ~CSTOPB;

    switch(comport->baudrate)
    {
     case 115200:
        temp = B115200;
        break;
     case 9600:
        temp = B9600;
        break;
     case 4800:
        temp = B4800;
        break;
     case 2400:
        temp = B2400;
        break;
     case 1800:
        temp = B1800;
        break;
     case 1200:
        temp = B1200;
        break;
     default:
        temp = B115200;
        break;
    }
    cfsetispeed(&new_cfg, temp);
    cfsetospeed(&new_cfg, temp);

    /* Set the com port timeout settings  */
    new_cfg.c_cc[VMIN] = 0;
    new_cfg.c_cc[VTIME] = 0;
    tcflush(comport->fd, TCIFLUSH);

    if(0 != tcsetattr(comport->fd, TCSANOW, &new_cfg))
    {
        retval = -5; //failed to set device comport settings
        goto CleanUp;
    }

    printf("Open device \"%s\".\n", comport->dev_name);
    comport->is_connect = 0x01;
    retval = comport->fd;

CleanUp:
    printf("open device \"%s\" %s.\n",comport->dev_name,retval >0 ? "successfully" : "failure");
    return retval;
}
