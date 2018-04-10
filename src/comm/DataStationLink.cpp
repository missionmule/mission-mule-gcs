#include "DataStationLink.h"

DataStationLink::DataStationLink(char *portName)
{
    this->connected = false;
    this->Xbee = open( portName, O_RDWR| O_NOCTTY );

    memset (&tty, 0, sizeof tty);

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    cfsetospeed (&tty, (speed_t)B9600);
    cfsetispeed (&tty, (speed_t)B9600);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( Xbee, TCIFLUSH );
}
DataStationLink::~DataStationLink()
{
    if (this->connected){
        this->connected = false;
        //from window version, handle was cleared here.
    }
}
int DataStationLink::readDataStationLink(char *buffer, unsigned int buf_size)
{
    int n = 0,
    spot = 0;
    char buf = '\0';

    /* Whole response*/
    char response[1024];
    memset(response, '\0', sizeof response);

    do {
        n = read( Xbee, &buf, 1 );
        sprintf( &response[spot], "%c", buf );//if formatting is the issue, go here first
        spot += n;
    } while( buf != '\r' && n > 0);

}

bool DataStationLink::writeDataStationLink(char *buffer, unsigned int buf_size)
{
    unsigned char cmd[] = "INIT \r";
    int n_written = 0,
        spot = 0;

    do {
        n_written = write( Xbee, &cmd[spot], 1 );
        spot += n_written;
    } while (cmd[spot-1] != '\r' && n_written > 0);
}

bool DataStationLink::isConnected()
{
    return this->connected;
}
