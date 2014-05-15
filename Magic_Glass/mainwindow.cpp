/*
	Written by Wai Khoo <wlkhoo@gmail.com>
	MainWindow implementation file.
*/
#include <QtGui>
#include "mainwindow.h"
#include "ImagePanel.h"
#include "label.h"
#include "histo.h"

/*
	Constructor: laying out the main window and set up appropriate widget in an appropriate place.
	Connects all the signals and slots appropriately.
*/
MainWindow::MainWindow()
{
	imagePanel = new ImagePanel;
	rgb = new Label;
	histo = new Histo;

	QWidget *w = new QWidget;
	QGridLayout *layout = new QGridLayout;
	layout -> addWidget (imagePanel, 0, 0);
	layout -> addWidget (rgb, 0, 1);
	layout -> setColumnMinimumWidth (0, 770);
	layout -> setColumnMinimumWidth (1, 30);

	createActions();
	createMenus();
	createToolBars();

	setWindowTitle (tr("Magic Glass"));
	resize (800,600);

	w -> setLayout (layout);
	setCentralWidget (w);

	connect (imagePanel, SIGNAL (labelChanged(int, int, int, int, int)),
			rgb, SLOT (valuesChanged(int, int, int, int, int)));

	connect (imagePanel, SIGNAL (displayHisto (int, int, int)),
			histo, SLOT (showHisto (int, int, int)));

	connect (histo, SIGNAL (histoValue (int, int, int)),
			rgb, SLOT (histoChanged (int, int, int)));

	connect (rgb, SIGNAL (thresLevelChanged(int)), this, SLOT (threshold(int)));

	connect (rgb, SIGNAL (changedRadius(int)), imagePanel, SLOT (setRadius(int)));
}

/*
	Main window open function (to open an image file which format Qt can supports).
	Set appropriate actions to true when a file is opened.
*/
void MainWindow::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

	if (!fileName.isEmpty())
	{
		QImage image (fileName);
		if (image.isNull())
		{
			QMessageBox::information(this, tr("Open"), tr("Cannot open %1.").arg(fileName));
			return;
		}
		imagePanel -> show(QPixmap::fromImage(image)); //pass the image pixmap to imagePanel show()
		histo -> histoCalc (image);				// pass the image to Histo to calculate the histogram.
		scaleFactor = 1.0;

		zoomInAct -> setEnabled (true);
		zoomOutAct -> setEnabled (true);
		histogramAct -> setEnabled (true);
		disMagicGlass();
	}
}

/*
	Zoom in function.  Image can't exceed scale factor 3.
	This function only passes the scale factor to image panel.
	Zooming implementation is in image panel class
*/
void MainWindow::zoomIn()
{
	if (scaleFactor < 3)
	{
		scaleFactor *= 1.25;
		imagePanel -> scaleImage (scaleFactor);
	}
}

/*
	Zoom out function.  Image can't shrink below scale factor 1/3.
	This function only passes the scale factor to image panel.
	Zooming implementation is in image panel class.
*/
void MainWindow::zoomOut()
{
	if (scaleFactor > 0.333)
	{
		scaleFactor *= 0.8;
		imagePanel -> scaleImage (scaleFactor);
	}
}

// Set main window to full screen.
void MainWindow::fullScreen()
{
	this -> setWindowState (this -> windowState() ^ Qt::WindowFullScreen);
}

// Display about which explain what does the program do.
void MainWindow::about()
{
	QMessageBox::about (this, tr("Magic Glass"),
		tr("<p>Magic Glass v.5. </p>"
			"<p>This program performs basic image processing such as display single band, grayscale, threshold, "
			"histogram, and edge detection. </p>"
			"<p>There is also a magic glass feature.  When the feature is turned on, it will only "
			"process the image within the glass, depending on which image processing technique you picked. </p> "
			"<p>Any problem(s), please look at readme file </p>"));
}

// Red channel of an image.
void MainWindow::red()
{
	imagePanel -> redBand();
}

// Green channel of an image
void MainWindow::green()
{
	imagePanel -> greenBand();
}

// Blue channel of an image.
void MainWindow::blue()
{
	imagePanel -> blueBand();
}

// Restore back original image and set all channels unchecked and disable threshold feature.
void MainWindow::restore()
{
	redAct -> setChecked (false);
	greenAct -> setChecked (false);
	blueAct -> setChecked (false);
	aveGrayScaleAct -> setChecked (false);
	lumGrayScaleAct -> setChecked (false);
	thresAllAct -> setChecked (false);
	thresSinAct -> setChecked (false);
	rgb -> disabledThres();
}

// Gray scale channel (average) of an image.
void MainWindow::averageGrayScale()
{
	imagePanel -> aveGrayScale();
}

// Gray scale channel (luminance) of an image.
void MainWindow::luminanceGrayScale()
{
	imagePanel ->lumGrayScale();
}

// Allow user to save the histogram, but only in JPG format.
void MainWindow::histogram()
{
	QByteArray fileFormat ("JPG");
	QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
	QString fileName = QFileDialog::getSaveFileName (this, tr("Save As"), initialPath, tr("%1 Files (*.%2);;All Files (*)")
															.arg(QString(fileFormat.toUpper())).arg(QString(fileFormat)));
	histo -> drawHisto (fileName + ".jpg");
}

// Threshold all bands of an image.
void MainWindow::thresAll()
{
	imagePanel -> thresholdAll();
	rgb -> enabledThres();
}

// Threshold individual band of an image.
void MainWindow::thresInd()
{
	imagePanel -> thresholdSin();
	rgb -> enabledThres();
}

// Let ImagePanel know which threshold to perform and what value.
void MainWindow::threshold(int value)
{
	imagePanel -> thresChanged (value);
	if (thresAllAct -> isChecked())
		imagePanel -> thresholdAll();
	else if (thresSinAct -> isChecked())
		imagePanel -> thresholdSin();
}

// When user enable magic glass, previous "off" features are turn on.
void MainWindow::enMagicGlass()
{
	enMagGlaAct -> setEnabled (false);
	prewittAct -> setEnabled (false);
	prewittAct -> setChecked (false);
	sobelAct -> setEnabled (false);
	sobelAct -> setChecked (false);
	logAct -> setEnabled (false);
	logAct -> setChecked (false);
	redAct -> setEnabled (true);
	redAct -> setChecked (true);
	greenAct -> setEnabled (true);
	blueAct -> setEnabled (true);
	aveGrayScaleAct -> setEnabled (true);
	lumGrayScaleAct -> setEnabled (true);
	thresAllAct -> setEnabled (true);
	thresSinAct -> setEnabled (true);
	disMagGlaAct -> setEnabled (true);
	rgb -> enableMagic (true);
	imagePanel -> magic (true);
}

// When user disable magic glass, all features are turn off except zooming, full screen, and generating histogram.
void MainWindow::disMagicGlass()
{
	enMagGlaAct -> setEnabled (true);
	prewittAct -> setEnabled (true);
	sobelAct -> setEnabled (true);
	logAct -> setEnabled (true);
	disMagGlaAct -> setEnabled (false);
	redAct -> setEnabled (false);
	greenAct -> setEnabled (false);
	blueAct -> setEnabled (false);
	aveGrayScaleAct -> setEnabled (false);
	lumGrayScaleAct -> setEnabled (false);
	thresAllAct -> setEnabled (false);
	thresSinAct -> setEnabled (false);
	rgb -> enableMagic (false);
	imagePanel -> magic (false);
	restore();
}

// Prewitt edge detection.
void MainWindow::prewitt()
{
	imagePanel -> prewittM();
}

// Sobel edge detection.
void MainWindow::sobel()
{
	imagePanel -> sobelM();
}

// Laplacian of Gaussian edge detection.
void MainWindow::LoG()
{
	imagePanel -> LoGM();
}

void MainWindow::createActions()
{
	redAct = new QAction (tr("Red"), this);
	redAct -> setShortcut(tr("Ctrl+R"));
	redAct -> setEnabled (false);
	redAct -> setCheckable (true);
	connect (redAct, SIGNAL(triggered()), this, SLOT(red()));

	greenAct = new QAction (tr("Green"), this);
	greenAct -> setShortcut (tr("Ctrl+G"));
	greenAct -> setEnabled (false);
	greenAct -> setCheckable (true);
	connect (greenAct, SIGNAL(triggered()), this, SLOT(green()));

	blueAct = new QAction (tr("Blue"), this);
	blueAct -> setShortcut (tr("Ctrl+B"));
	blueAct -> setEnabled (false);
	blueAct -> setCheckable (true);
	connect (blueAct, SIGNAL(triggered()), this, SLOT(blue()));

	aveGrayScaleAct = new QAction (tr("Average Gray Scale"), this);
	aveGrayScaleAct -> setShortcut (tr("Ctrl+V"));
	aveGrayScaleAct -> setEnabled (false);
	aveGrayScaleAct -> setCheckable (true);
	connect (aveGrayScaleAct, SIGNAL (triggered()), this, SLOT (averageGrayScale()));

	lumGrayScaleAct = new QAction (tr("Luminance Gray Scale"), this);
	lumGrayScaleAct -> setShortcut (tr("Ctrl+L"));
	lumGrayScaleAct -> setEnabled (false);
	lumGrayScaleAct -> setCheckable (true);
	connect (lumGrayScaleAct, SIGNAL (triggered()), this, SLOT (luminanceGrayScale()));

	histogramAct = new QAction (tr("Generate a histogram"), this);
	histogramAct -> setShortcut (tr("Ctrl+H"));
	histogramAct -> setEnabled (false);
	connect (histogramAct, SIGNAL (triggered()), this, SLOT (histogram()));

	thresAllAct = new QAction (tr("All Bands"), this);
	thresAllAct -> setShortcut (tr("Ctrl+T"));
	thresAllAct -> setEnabled (false);
	thresAllAct -> setCheckable (true);
	connect (thresAllAct, SIGNAL (triggered()), this, SLOT (thresAll()));

	thresSinAct = new QAction (tr("Individual Band"), this);
	thresSinAct -> setShortcut (tr("Ctrl+S"));
	thresSinAct -> setEnabled (false);
	thresSinAct -> setCheckable (true);
	connect (thresSinAct, SIGNAL (triggered()), this, SLOT (thresInd()));

	enMagGlaAct = new QAction (tr("Enable Magic Glass"), this);
	enMagGlaAct -> setShortcut (tr("Ctrl+M"));
	enMagGlaAct -> setEnabled (false);
	connect (enMagGlaAct, SIGNAL (triggered()), this, SLOT (enMagicGlass()));

	disMagGlaAct = new QAction (tr("Disable Magic Glas"), this);
	disMagGlaAct -> setShortcut (tr("Ctrl+D"));
	disMagGlaAct -> setEnabled (false);
	connect (disMagGlaAct, SIGNAL (triggered()), this, SLOT (disMagicGlass()));

	prewittAct = new QAction (tr("Prewitt Mask"), this);
	prewittAct -> setEnabled (false);
	prewittAct -> setCheckable (true);
	connect (prewittAct, SIGNAL (triggered()), this, SLOT (prewitt()));

	sobelAct = new QAction (tr("Sobel Mask"), this);
	sobelAct -> setEnabled (false);
	sobelAct -> setCheckable (true);
	connect (sobelAct, SIGNAL (triggered()), this, SLOT (sobel()));

	logAct = new QAction (tr("Laplacian of Gaussian"), this);
	logAct -> setEnabled (false);
	logAct -> setCheckable (true);
	connect (logAct, SIGNAL (triggered()), this, SLOT (LoG()));

	openAct = new QAction (tr("&Open"), this);
	openAct -> setShortcut (tr("Ctrl+O"));
	connect (openAct, SIGNAL(triggered()), this, SLOT (open()));

	exitAct = new QAction (tr("&Exit"), this);
	exitAct -> setShortcut (tr("Ctrl+Q"));
	connect (exitAct, SIGNAL(triggered()), this, SLOT (close()));

	zoomInAct = new QAction (tr("Zoom &In (25%)"), this);
	zoomInAct -> setShortcut (tr("Ctrl++"));
	zoomInAct -> setEnabled (false);
	connect (zoomInAct, SIGNAL(triggered()), this, SLOT (zoomIn()));

	zoomOutAct = new QAction (tr("Zoom &Out (25%)"), this);
	zoomOutAct -> setShortcut (tr("Ctrl+-"));
	zoomOutAct -> setEnabled (false);
	connect (zoomOutAct, SIGNAL (triggered()), this, SLOT (zoomOut()));

	fullScreenAct = new QAction (tr("&Full Screen"), this);
	fullScreenAct -> setCheckable (true);
	fullScreenAct -> setShortcut (tr("Ctrl+F"));
	connect (fullScreenAct, SIGNAL(triggered()), this, SLOT(fullScreen()));

	aboutAct = new QAction (tr("&About"), this);
	connect (aboutAct, SIGNAL (triggered()), this, SLOT (about()));

	aboutQtAct = new QAction (tr("About &Qt"), this);
	connect (aboutQtAct, SIGNAL (triggered()), qApp, SLOT (aboutQt()));

	bandGroup = new QActionGroup (this);
	bandGroup -> addAction (redAct);
	bandGroup -> addAction (greenAct);
	bandGroup -> addAction (blueAct);
	bandGroup -> addAction (aveGrayScaleAct);
	bandGroup -> addAction (lumGrayScaleAct);
	bandGroup -> addAction (thresAllAct);
	bandGroup -> addAction (thresSinAct);
	bandGroup -> setExclusive (true);
	bandGroup -> setVisible (true);

	edgeDetectionGroup = new QActionGroup (this);
	edgeDetectionGroup -> addAction (prewittAct);
	edgeDetectionGroup -> addAction (sobelAct);
	edgeDetectionGroup -> addAction (logAct);
	edgeDetectionGroup -> setExclusive (true);
	edgeDetectionGroup -> setVisible (true);
}

void MainWindow::createMenus()
{
	bandChannelMenu = new QMenu (tr("&Channel"), this);
	bandChannelMenu -> addAction (redAct);
	bandChannelMenu -> addAction (greenAct);
	bandChannelMenu -> addAction (blueAct);
	bandChannelMenu -> addSeparator();
	bandChannelMenu -> addAction (aveGrayScaleAct);
	bandChannelMenu -> addAction (lumGrayScaleAct);

	thresholdMenu = new QMenu (tr("&Threshold"), this);
	thresholdMenu -> addAction (thresAllAct);
	thresholdMenu -> addAction (thresSinAct);

	edgeDetMenu = new QMenu (tr("Edge Detection"), this);
	edgeDetMenu -> addAction (prewittAct);
	edgeDetMenu -> addAction (sobelAct);
	edgeDetMenu -> addAction (logAct);

	fileMenu = new QMenu (tr("&File"), this);
	fileMenu -> addAction (openAct);
	fileMenu -> addAction (histogramAct);
	fileMenu -> addAction (enMagGlaAct);
	fileMenu -> addAction (disMagGlaAct);
	fileMenu -> addSeparator();
	fileMenu -> addAction (exitAct);

	viewMenu = new QMenu (tr("&View"), this);
	viewMenu -> addAction (fullScreenAct);
	viewMenu -> addSeparator();
	viewMenu -> addAction (zoomInAct);
	viewMenu -> addAction (zoomOutAct);
	viewMenu -> addSeparator();
	viewMenu -> addMenu (bandChannelMenu);
	viewMenu -> addMenu (thresholdMenu);
	viewMenu -> addSeparator();
	viewMenu -> addMenu (edgeDetMenu);

	helpMenu = new QMenu (tr("&Help"), this);
	helpMenu -> addAction (aboutAct);
	helpMenu -> addAction (aboutQtAct);

	menuBar() -> addMenu (fileMenu);
	menuBar() -> addMenu (viewMenu);
	menuBar() -> addMenu (helpMenu);
}

void MainWindow::createToolBars()
{
	viewToolBar = addToolBar (tr("View"));
	viewToolBar -> addAction (zoomInAct);
	viewToolBar -> addAction (zoomOutAct);
	viewToolBar -> addAction (fullScreenAct);
}
//Wai Khoo
