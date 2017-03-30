// Engineer: Thomas Reaney
// College: National University of Ireland Galway
// Date: 20/02/2017
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

class QLabel;
class LeptonThread;
class QGridLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum { ImageWidth = 320, ImageHeight = 240 };

    static int snapshotCount;
	static int runCount;

    QLabel *imageLabel;
    LeptonThread *thread;
    QGridLayout *layout;

    unsigned short rawMin, rawMax;
    QVector<unsigned short> rawData;
    QImage rgbImage;

public:
    explicit MainWindow(QWidget *parent = 0);

public slots:
    void saveSnapshot(QString directory);
    void updateImage(unsigned short *, int, int);
};

#endif
