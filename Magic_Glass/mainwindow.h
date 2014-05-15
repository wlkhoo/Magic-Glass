/*
	Written by Wai Khoo <wlkhoo@gmail.com>
	The "control" header file. This file layout the main window and set up other widget (imagepanel, label, etc)
	Most signals have to passed through here in order to tell other class what has changed.
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "ImagePanel.h"
#include "label.h"
#include "histo.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void open();
	void zoomIn();
	void zoomOut();
	void fullScreen();
	void about();
	void red();
	void green();
	void blue();
	void restore();
	void averageGrayScale();
	void luminanceGrayScale();
	void histogram();
	void thresAll();
	void thresInd();
	void threshold(int value);
	void enMagicGlass();
	void disMagicGlass();
	void prewitt();
	void sobel();
	void LoG();

private:
	void createActions();
	void createMenus();
	void createToolBars();

	Histo *histo;
	ImagePanel *imagePanel;
	Label *rgb;
	double scaleFactor;

	QActionGroup *bandGroup;
	QActionGroup *edgeDetectionGroup;

	QAction *redAct;
	QAction *greenAct;
	QAction *blueAct;
	QAction *openAct;
	QAction *exitAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *fullScreenAct;
	QAction *aboutAct;
	QAction *aboutQtAct;
	QAction *histogramAct;
	QAction *aveGrayScaleAct;
	QAction *lumGrayScaleAct;
	QAction *thresAllAct;
	QAction *thresSinAct;
	QAction *enMagGlaAct;
	QAction *disMagGlaAct;
	QAction *prewittAct;
	QAction *sobelAct;
	QAction *logAct;

	QToolBar *viewToolBar;

	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;
	QMenu *bandChannelMenu;
	QMenu *thresholdMenu;
	QMenu *edgeDetMenu;
};
#endif
// Wai Khoo
