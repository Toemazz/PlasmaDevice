// Engineer: Thomas Reaney
// College: National University of Ireland Galway
// Date: 20/02/2017
#include "mainwindow.h"
#include "LeptonThread.h"
#include <QLabel>
#include <QGridLayout>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QFile>
#include <QDir>

// Used for renaming files and folders
int MainWindow::snapshotCount = 0;
int MainWindow::runCount = 0;
// Used for applying a fixed colour map to images
const int frameMax = 9984;
const int frameMin = 8448;
const int OriginalRange = frameMax - frameMin;
const int intervalSize = OriginalRange / 256;
// Used for deciding when the images needed to be saved
int oldMaxValue = 0;
int saveThreshold = 8800;
bool coolingFlag = true;
// Colour Map: Jet
const int colormap[] = {1, 3, 74, 0, 3, 74, 0, 3, 75, 0, 3, 75, 0, 3, 76, 0, 3, 76, 0, 3, 77, 0, 3, 79, 0, 3, 82, 0, 5, 85, 0, 7, 88, 0, 10, 91, 0, 14, 94, 0, 19, 98, 0, 22, 100, 0, 25, 103, 0, 28, 106, 0, 32, 109, 0, 35, 112, 0, 38, 116, 0, 40, 119, 0, 42, 123, 0, 45, 128, 0, 49, 133, 0, 50, 134, 0, 51, 136, 0, 52, 137, 0, 53, 139, 0, 54, 142, 0, 55, 144, 0, 56, 145, 0, 58, 149, 0, 61, 154, 0, 63, 156, 0, 65, 159, 0, 66, 161, 0, 68, 164, 0, 69, 167, 0, 71, 170, 0, 73, 174, 0, 75, 179, 0, 76, 181, 0, 78, 184, 0, 79, 187, 0, 80, 188, 0, 81, 190, 0, 84, 194, 0, 87, 198, 0, 88, 200, 0, 90, 203, 0, 92, 205, 0, 94, 207, 0, 94, 208, 0, 95, 209, 0, 96, 210, 0, 97, 211, 0, 99, 214, 0, 102, 217, 0, 103, 218, 0, 104, 219, 0, 105, 220, 0, 107, 221, 0, 109, 223, 0, 111, 223, 0, 113, 223, 0, 115, 222, 0, 117, 221, 0, 118, 220, 1, 120, 219, 1, 122, 217, 2, 124, 216, 2, 126, 214, 3, 129, 212, 3, 131, 207, 4, 132, 205, 4, 133, 202, 4, 134, 197, 5, 136, 192, 6, 138, 185, 7, 141, 178, 8, 142, 172, 10, 144, 166, 10, 144, 162, 11, 145, 158, 12, 146, 153, 13, 147, 149, 15, 149, 140, 17, 151, 132, 22, 153, 120, 25, 154, 115, 28, 156, 109, 34, 158, 101, 40, 160, 94, 45, 162, 86, 51, 164, 79, 59, 167, 69, 67, 171, 60, 72, 173, 54, 78, 175, 48, 83, 177, 43, 89, 179, 39, 93, 181, 35, 98, 183, 31, 105, 185, 26, 109, 187, 23, 113, 188, 21, 118, 189, 19, 123, 191, 17, 128, 193, 14, 134, 195, 12, 138, 196, 10, 142, 197, 8, 146, 198, 6, 151, 200, 5, 155, 201, 4, 160, 203, 3, 164, 204, 2, 169, 205, 2, 173, 206, 1, 175, 207, 1, 178, 207, 1, 184, 208, 0, 190, 210, 0, 193, 211, 0, 196, 212, 0, 199, 212, 0, 202, 213, 1, 207, 214, 2, 212, 215, 3, 215, 214, 3, 218, 214, 3, 220, 213, 3, 222, 213, 4, 224, 212, 4, 225, 212, 5, 226, 212, 5, 229, 211, 5, 232, 211, 6, 232, 211, 6, 233, 211, 6, 234, 210, 6, 235, 210, 7, 236, 209, 7, 237, 208, 8, 239, 206, 8, 241, 204, 9, 242, 203, 9, 244, 202, 10, 244, 201, 10, 245, 200, 10, 245, 199, 11, 246, 198, 11, 247, 197, 12, 248, 194, 13, 249, 191, 14, 250, 189, 14, 251, 187, 15, 251, 185, 16, 252, 183, 17, 252, 178, 18, 253, 174, 19, 253, 171, 19, 254, 168, 20, 254, 165, 21, 254, 164, 21, 255, 163, 22, 255, 161, 22, 255, 159, 23, 255, 157, 23, 255, 155, 24, 255, 149, 25, 255, 143, 27, 255, 139, 28, 255, 135, 30, 255, 131, 31, 255, 127, 32, 255, 118, 34, 255, 110, 36, 255, 104, 37, 255, 101, 38, 255, 99, 39, 255, 93, 40, 255, 88, 42, 254, 82, 43, 254, 77, 45, 254, 69, 47, 254, 62, 49, 253, 57, 50, 253, 53, 52, 252, 49, 53, 252, 45, 55, 251, 39, 57, 251, 33, 59, 251, 32, 60, 251, 31, 60, 251, 30, 61, 251, 29, 61, 251, 28, 62, 250, 27, 63, 250, 27, 65, 249, 26, 66, 249, 26, 68, 248, 25, 70, 248, 24, 73, 247, 24, 75, 247, 25, 77, 247, 25, 79, 247, 26, 81, 247, 32, 83, 247, 35, 85, 247, 38, 86, 247, 42, 88, 247, 46, 90, 247, 50, 92, 248, 55, 94, 248, 59, 96, 248, 64, 98, 248, 72, 101, 249, 81, 104, 249, 87, 106, 250, 93, 108, 250, 95, 109, 250, 98, 110, 250, 100, 111, 251, 101, 112, 251, 102, 113, 251, 109, 117, 252, 116, 121, 252, 121, 123, 253, 126, 126, 253, 130, 128, 254, 135, 131, 254, 139, 133, 254, 144, 136, 254, 151, 140, 255, 158, 144, 255, 163, 146, 255, 168, 149, 255, 173, 152, 255, 176, 153, 255, 178, 155, 255, 184, 160, 255, 191, 165, 255, 195, 168, 255, 199, 172, 255, 203, 175, 255, 207, 179, 255, 211, 182, 255, 216, 185, 255, 218, 190, 255, 220, 196, 255, 222, 200, 255, 225, 202, 255, 227, 204, 255, 230, 206, 255, 233, 208};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , rawData(LeptonThread::FrameWords)
    , rgbImage(LeptonThread::FrameWidth, LeptonThread::FrameHeight, QImage::Format_RGB888)
{
	// Set up main widget
    QWidget *mainWidget = new QWidget();
    setCentralWidget(mainWidget);
	
	// Set up widget layout
    layout = new QGridLayout();
    mainWidget -> setLayout(layout);
	
	// Set up image label
    imageLabel = new QLabel();
    layout -> addWidget(imageLabel, 0, 0, Qt::AlignCenter);

	// Set up placeholder for the live image stream
    QPixmap filler(ImageWidth, ImageHeight); filler.fill(Qt::red);
    imageLabel -> setPixmap(filler);

	// Set up new LeptonThread to get the raw data from the camera
    thread = new LeptonThread();
    connect(thread, SIGNAL(updateImage(unsigned short *,int,int)), this, SLOT(updateImage(unsigned short *, int,int)));
	
	// Start the LeptonThread
    thread -> start();
}


// Method: Used to update the image on the QPixmap and decide when to save images/data
void MainWindow::updateImage(unsigned short *data, int minValue, int maxValue)
{
    // Record the raw data and min/max values
    memcpy(&rawData[0], data, 2*LeptonThread::FrameWords);
    rawMin = minValue;
	rawMax = maxValue;

    // Map the rawData to RGB values in the rgbImage using the colour map
    for (int y=0; y<LeptonThread::FrameHeight; ++y)
	{
        for (int x=0; x<LeptonThread::FrameWidth; ++x)
		{
			int index = 0;
			// Get the rawValue of the pixel
            int rawValue = rawData[LeptonThread::FrameWidth*y + x];
			// Normalize the value based on the defined frameMin
			int normValue = rawValue - frameMin;
			
			// If rawValue is greater than frameMax, set the index to 255
			if (rawValue > frameMax) {
				index = 255;
			// If rawValue is less than frameMin, set the index to 0
			} else if (rawValue < frameMin) {
				index = 0;
			// Otherwise, map the index to the colour map using the normVal and intervalSize
			} else {
                index = normVal / intervalSize;
			}
			// Set the RGB value of the pixel
			rgbImage.setPixel(x, y, qRgb(colormap[3*index], colormap[3*index+1], colormap[3*index+2]));
        }
    }
	
	// Update the pixmap
    QPixmap pixmap = QPixmap::fromImage(rgbImage).scaled(ImageWidth, ImageHeight, Qt::KeepAspectRatio);
    QPainter painter(&pixmap);
    imageLabel -> setPixmap(pixmap);
	
	QString directory;

    // If maxValue in the image is greater than the saveThreshold
    if (maxValue >= saveThreshold) {
		// If the maxValue is greater than the oldMaxValue (i.e. heating up)
		if (maxValue >= oldMaxValue) {
			// Update directory name based off runCount and saveSnapshot
			QString directory = QString("/home/pi/Raw/Run%1/").arg(runCount);
			saveSnapshot(directory);
			coolingFlag = true;
		// If it is cooling down, set it to false and increment runCount
		} else if (coolingFlag == true) {
			coolingFlag = false;
			++runCount;
		}
		oldMaxValue = maxValue;
	}
}


// Method: Used to save the rawData as a .bin file and the image as a .jpg file
void MainWindow::saveSnapshot(QString directory)
{
	// Increment snapshotCount and make new QDir
    ++snapshotCount;
    QDir dir;
    dir.mkpath(directory);

	// Write rawData to .bin file with 1 word for minValue, 1 word for maxValue and 80x60 words of rawData
    QFile rawFile(directory + QString("raw%1.bin").arg(snapshotCount));
    rawFile.open(QIODevice::Truncate | QIODevice::ReadWrite);
    QDataStream rawOut(&rawFile);
    rawOut << rawMin << rawMax;
    rawOut.writeRawData((char *) &rawData[0], 2*rawData.size());
    rawFile.close();

    // Save image as a best quality .jpg file
    rgbImage.save(directory + QString("rgb%1.jpg").arg(snapshotCount), "JPG", 100);
}
