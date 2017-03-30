// Engineer: Thomas Reaney
// College: National University of Ireland Galway
// Date: 20/02/2017
#include <QApplication>
#include "mainwindow.h"

// Method: Used to run the plasma device application
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
	
    MainWindow *mainWindow = new MainWindow();
    mainWindow -> show();

	return app.exec();
}

