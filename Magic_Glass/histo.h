/*
	Written by Wai Khoo <wlkhoo@gmail.com
	This file calculate the histogram of each colors (RGB).
	Generate single band, gray scale, and threshold images.
	Perform edge detection (Prewitt, Sobel, and LoG).
*/
#ifndef HISTO_H
#define HISTO_H

#include <QtGui>

class Histo : public QWidget
{
	Q_OBJECT

public:
	enum Channel {Red, Green, Blue, Ave, Lum, All, Ind};
	Histo(QWidget *parent = 0, Qt::WFlags f = 0);
	void histoCalc(QImage image);
	void drawHisto(const QString &fileName);
	QImage thresholdLevel (QImage originalPic, QImage copyPic, int thresLevel, bool all, bool individual);
	void lookUpTable (int thresLevel);
	QImage magicGlass (QImage orig, QImage copy, int rad, int x, int y);
	void setState (bool r, bool g, bool b, bool ags, bool lgs, bool all, bool individual, int value);
	QImage prewittMask (QImage orig, QImage copy);
	QImage sobelMask (QImage orig, QImage copy);
	QImage LoGMask (QImage orig, QImage copy);
	QImage grayIm (QImage im);

signals:
	void histoValue (int r, int g, int b);

public slots:
	void showHisto(int r, int g, int b);

private:

	Channel chan;

	int *redHisto;
	int *greenHisto;
	int *blueHisto;
	int *lut;
	int colorValue;
	int max;
	int maxRed;
	int maxGreen;
	int maxBlue;
	int rPosition;
	int gPosition;
	int bPosition;
	int thresValue;
	double scale;

	bool red;
	bool green;
	bool blue;
	bool aveGS;
	bool lumGS;
	bool thresAll;
	bool thresInd;

	QRgb color;
};
#endif
//Wai Khoo
