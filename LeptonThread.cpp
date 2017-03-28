#include "LeptonThread.h"

#include <QString>
#include <QTextStream>

LeptonThread::LeptonThread()
    : QThread()
    , result(RowPacketBytes*FrameHeight)
    , rawData(FrameWords) { }

LeptonThread::~LeptonThread() { }

const char *LeptonThread::device = "/dev/spidev0.0"; // Change to 0.1 if necessary!
unsigned char LeptonThread::mode = 0, LeptonThread::bits = 8;
unsigned int LeptonThread::speed = 16000000;
unsigned short LeptonThread::delay = 0;
QVector<unsigned char> LeptonThread::tx(LeptonThread::RowPacketBytes, 0);

bool LeptonThread::initLepton() {
    fd = open(device, O_RDWR);
    if (fd < 0)
        qDebug() << "Can't open device";
    else if (-1 == ioctl(fd, SPI_IOC_WR_MODE, &mode))
        qDebug() << "Can't set SPI mode";
    else if (-1 == ioctl(fd, SPI_IOC_RD_MODE, &mode))
        qDebug() << "Can't get SPI mode";
    else if (-1 == ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits))
        qDebug() << "Can't set bits per word";
    else if (-1 == ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits))
        qDebug() << "Can't get bits per word";
    else if (-1 == ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
        qDebug() << "Can't set max speed";
    else if (-1 == ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed))
        qDebug() << "Can't get max speed";
    else
        return true;
    return false;
}

int LeptonThread::getPacket(int iRow, unsigned char *packetData) {
    _tr.rx_buf = (unsigned long) packetData;
    return ioctl(fd, SPI_IOC_MESSAGE(1), &_tr);
}

void LeptonThread::run() {    
    if (!initLepton()) return;

    usleep(250000);

    _tr.tx_buf = (unsigned long) &tx[0];
    _tr.len = RowPacketBytes;
    _tr.delay_usecs = delay;
    _tr.speed_hz = speed;
    _tr.bits_per_word = bits;

    int resets = 0; // Number of times we've reset the 0...59 loop for packets
    int errors = 0; // Number of error-packets received
    while (true) {
        int iRow;
        for (iRow = 0; iRow < FrameHeight; ) {
            unsigned char *packet = &result[iRow*RowPacketBytes];

            if (getPacket(iRow, packet) < 1) {
                qDebug() << "Error transferring SPI packet";
                return;
            }

            int packetNumber;
            if ((packet[0] & 0xf)==0xf)
                packetNumber = -1;
            else
                packetNumber = packet[1];

            if (packetNumber==-1) {
                usleep(1000);
                if (++errors > 300) break;
                continue;
            }

            if (packetNumber != iRow) {
                usleep(1000);
                break;
            }

            ++iRow;
        }

        if (iRow < FrameHeight) {
            if (++resets >= 750) {
                qDebug() << "Packet reset counter hit 750";
                resets = 0;
                usleep(750000);
            }
            continue;
        }

        resets = 0; errors = 0;

        uint16_t minValue = 65535;
        uint16_t maxValue = 0;
        unsigned char *in = &result[0];
        unsigned short *out = &rawData[0];
        for (int iRow = 0; iRow < FrameHeight; ++iRow) {
            in += 4;
            for (int iCol = 0; iCol < FrameWidth; ++iCol) {
                unsigned short value = in[0];
                value <<= 8;
                value |= in[1];
                in += 2;
                if (value > maxValue) maxValue = value;
                if (value < minValue) minValue = value;
                *(out++) = value;
            }
        }

        emit updateImage(&rawData[0], minValue, maxValue);
    }
}
