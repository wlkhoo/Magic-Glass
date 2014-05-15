/*
	Written by Wai Khoo <wlkhoo@gmail.com>
*/

#include <QApplication>
#include "mainwindow.h"

int main (int argc, char *argv[])
{
	QApplication app (argc, argv);

	MainWindow gui;
	gui.show();
	return app.exec();
}
//Wai Khoo
