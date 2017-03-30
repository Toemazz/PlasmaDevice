// Engineer: Thomas Reaney
// College: National University of Ireland Galway
// Date: 20/02/2017
#ifndef LEPTONTHREAD
#define LEPTONTHREAD

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <QThread>
#include <QDebug>
#include <QVector>


class LeptonThread : public QThread
{
    Q_OBJECT

    QVector<unsigned char> result;
    QVector<unsigned short> rawData;

    static const char *device;
    static unsigned char mode, bits;
    static unsigned int speed;
    static unsigned short delay;
    static QVector<unsigned char> tx;

    int fd;
    struct spi_ioc_transfer _tr;

    bool initLepton();
    int getPacket(int iRow, unsigned char *packetData);

public:
    enum
	{
        FrameWidth = 80,
        FrameHeight = 60,
        RowPacketWords = FrameWidth + 2,
        RowPacketBytes = 2*RowPacketWords,
        FrameWords = FrameWidth*FrameHeight
    };

    LeptonThread();
    ~LeptonThread();

    void run();

signals:
    void updateImage(unsigned short *, int, int);
};

#endif
