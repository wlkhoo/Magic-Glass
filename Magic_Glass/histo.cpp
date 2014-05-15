/*
	Written by Wai Khoo <wlkhoo@gmail.com
	The implementation of histo.h.
*/
#include <QtGui>
#include "histo.h"

//	Constructor: initializes variables and setting all histogram variables to zero.
Histo::Histo(QWidget *parent, Qt::WFlags f)
	: QWidget (parent, f)
{
	red = green = blue = aveGS = lumGS = thresAll = thresInd = false;
	thresValue = 0;
	redHisto = new int [256];
	greenHisto = new int [256];
	blueHisto = new int [256];

	for (int i = 0; i < 256; i++)
	{
		redHisto [i] = 0;
		greenHisto [i] = 0;
		blueHisto [i] = 0;
	}

	lookUpTable (0);
}

/*
	This function get called from open() of MainWindow.
	This function calculate the histogram of an image, which passed from open()
*/
void Histo::histoCalc (QImage image)
{
	for (int j = 0; j < image.height(); j++)
	{
		for (int k = 0; k < image.width(); k++)
		{
			color = image.pixel (k, j);

			redHisto [qRed(color)]++;
			greenHisto [qGreen(color)]++;
			blueHisto [qBlue(color)]++;
		}
	}
}

// Generate a 3-bands histogram and save it a file that the user specified.
// File format has been preset to JPG format.
void Histo::drawHisto(const QString &fileName)
{
	QImage histogram (256, 256, QImage::Format_RGB32);

	for (int h = 0; h < histogram.height(); h++)
	{
		for (int w = 0; w < histogram.width(); w++)
		{
			histogram.setPixel (w, h, qRgb (255, 255, 255));
		}
	}

	maxRed = redHisto[0];
	for (int r = 1; r < 256; r++)
	{
		if (maxRed < redHisto [r])
			maxRed = redHisto [r];
	}

	maxGreen = greenHisto[0];
	for (int g = 1; g < 256; g++)
	{
		if (maxGreen < greenHisto[g])
			maxGreen = greenHisto[g];
	}

	maxBlue = blueHisto[0];
	for (int b = 1; b < 256; b++)
	{
		if (maxBlue < blueHisto[b])
			maxBlue = blueHisto[b];
	}

	max = maxRed;
	if (max < maxGreen)
		max = maxGreen;
	if (max < maxBlue)
		max = maxBlue;
	if (max < maxRed)
		max = maxRed;

	scale = max / 255.0;

	for (int c = 0; c < 256; c++)
	{
		rPosition = redHisto[c] / scale;
		rPosition = histogram.height() - rPosition - 1;
		histogram.setPixel (c, rPosition, qRgb (255, 0, 0));

		gPosition = greenHisto[c] / scale;
		gPosition = histogram.height() - gPosition - 1;
		histogram.setPixel (c, gPosition, qRgb (0, 255, 0));

		bPosition = blueHisto[c] / scale;
		bPosition = histogram.height() - bPosition - 1;
		histogram.setPixel (c, bPosition, qRgb (0, 0, 255));
	}

	histogram.save (fileName, "jpg");
}

//	Show the histogram at indexes r, g, b, which is really the RGB values.
void Histo::showHisto (int r, int g, int b)
{
	emit histoValue (redHisto [r], greenHisto [g], blueHisto [b]);
}

// A look-up table for thresholding.  0 ~ thresLevel is 0... thresLevel ~ 255 is 255.
void Histo::lookUpTable (int thresLevel)
{
	lut = new int [256];
	int i;

	for (i = 0; i < thresLevel; i++)
		lut [i] = 0;
	for (		  ; i < 255; i++)
		lut [i] = 255;
}

/*
	Magic Glass function.  Obtain current coordinates from ImagePanel and only process the image within the ellipse's radius.
	Using enum to decide which channel(red, green, blue, grayscales, or threshold) to process.
*/
QImage Histo::magicGlass (QImage orig, QImage copy, int rad, int x, int y)
{
	QImage newPic (copy.width(), copy.height(), QImage::Format_RGB32);
	orig = orig.scaled (copy.width(), copy.height());

	newPic = orig.copy(QRect());

	int result = 0;
	rad *= rad;

	for (int j = 0; j < orig.height(); j++)
	{
		for (int i = 0; i < orig.width(); i++)
		{
			result = (i-x) * (i-x) + (j-y) * (j-y);
			if (result <= rad)
			{
				color = orig.pixel (i, j);
				switch (chan)
				{
					case 0:
					{
						colorValue = qRed(color);
						newPic.setPixel (i, j, qRgb (colorValue, colorValue, colorValue));
					}
					break;
					case 1:
					{
						colorValue = qGreen(color);
						newPic.setPixel (i, j, qRgb (colorValue, colorValue, colorValue));
					}
					break;
					case 2:
					{
						colorValue = qBlue(color);
						newPic.setPixel (i, j, qRgb (colorValue, colorValue, colorValue));
					}
					break;
					case 3:
					{
						colorValue = (qRed(color) + qGreen(color) + qBlue(color)) / 3;
						newPic.setPixel (i, j, qRgb (colorValue, colorValue, colorValue));
					}
					break;
					case 4:
					{
						colorValue = 0.3 * qRed(color) + 0.59 * qGreen(color) + 0.11 * qBlue(color);
						newPic.setPixel (i, j, qRgb (colorValue, colorValue, colorValue));
					}
					break;
					case 5:
					{
						colorValue = 0.3 * qRed(color) + 0.59 * qGreen(color) + 0.11 * qBlue(color);
						colorValue = lut[colorValue];
						newPic.setPixel (i, j, qRgb (colorValue, colorValue, colorValue));
					}
					break;
					case 6:
					{
						newPic.setPixel (i, j, qRgb (lut[qRed(color)], lut[qGreen(color)], lut[qBlue(color)]));
					}
				}
			}
		}
	}

	return newPic;
}

// Set the appropriate state so the Magic Glass function can decide which channel to process.
void Histo::setState (bool r, bool g, bool b, bool ags, bool lgs, bool all, bool individual, int value)
{
	red = r;
	green = g;
	blue = b;
	aveGS = ags;
	lumGS = lgs;
	thresAll = all;
	thresInd = individual;
	thresValue = value;

	if (red)
		chan = Red;
	else if (green)
		chan = Green;
	else if (blue)
		chan = Blue;
	else if (aveGS)
		chan = Ave;
	else if (lumGS)
		chan = Lum;
	else if (thresAll)
		chan = All;
	else if (thresInd)
		chan = Ind;
}

// Prewitt edge detection implementation using Prewitt equations.
// First turn the image into gray image and then perform detection.
QImage Histo::prewittMask (QImage orig, QImage copy)
{
	QImage newPic (copy.width(), copy.height(), QImage::Format_RGB32);
	orig = orig.scaled (copy.width(), copy.height());

	orig = grayIm (orig);

	int gx, gy, grad;

	gx = gy = grad = 0;

	for (int y = 0; y < orig.height(); y++)
	{
		for (int x = 0; x < orig.width(); x++)
		{
			if (x == 0 || x == (orig.width() - 1))
				grad = 0;
			else if (y == 0 || y == (orig.height() - 1))
				grad = 0;
			else
			{
				gx = (qBlue(orig.pixel(x-1,y+1)) + qBlue(orig.pixel(x,y+1)) + qBlue(orig.pixel(x+1,y+1))) - (qBlue(orig.pixel(x-1,y-1)) + qBlue(orig.pixel(x,y-1)) + qBlue(orig.pixel(x+1,y-1)));
				gy = (qBlue(orig.pixel(x+1,y-1)) + qBlue(orig.pixel(x+1,y)) + qBlue(orig.pixel(x+1,y+1))) - (qBlue(orig.pixel(x-1,y-1)) + qBlue(orig.pixel(x-1,y)) + qBlue(orig.pixel(x-1,y+1)));

				if (gx < 0)
					gx = -gx;
				else if (gy < 0)
					gy = -gy;

				grad = gx + gy;
			}
			newPic.setPixel (x, y, qRgb (grad, grad, grad));
		}
	}

	return newPic;
}

// Sobel edge detection implementation using Sobel equations.
// First turn the image into gray image and then perform detection.
QImage Histo::sobelMask (QImage orig, QImage copy)
{
	QImage newPic (copy.width(), copy.height(), QImage::Format_RGB32);
	orig = orig.scaled (copy.width(), copy.height());

	orig = grayIm (orig);

	int gx, gy, grad;

	gx = gy = grad = 0;

	for (int y = 0; y < orig.height(); y++)
	{
		for (int x = 0; x < orig.width(); x++)
		{
			if (x == 0 || x == (orig.width() - 1))
				grad = 0;
			else if (y == 0 || y == (orig.height() - 1))
				grad = 0;
			else
			{
				gx = (qBlue(orig.pixel(x-1,y+1)) + 2*qBlue(orig.pixel(x,y+1)) + qBlue(orig.pixel(x+1,y+1))) - (qBlue(orig.pixel(x-1,y-1)) + 2*qBlue(orig.pixel(x,y-1)) + qBlue(orig.pixel(x+1,y-1)));
				gy = (qBlue(orig.pixel(x+1,y-1)) + 2*qBlue(orig.pixel(x+1,y)) + qBlue(orig.pixel(x+1,y+1))) - (qBlue(orig.pixel(x-1,y-1)) + 2*qBlue(orig.pixel(x-1,y)) + qBlue(orig.pixel(x-1,y+1)));

				if (gx < 0)
					gx = -gx;
				else if (gy < 0)
					gy = -gy;

				grad = gx + gy;
			}
			newPic.setPixel (x, y, qRgb (grad, grad, grad));
		}
	}

	return newPic;
}

// LoG edge detection implementation.
// First turn the image into gray image and then perform detection.
QImage Histo::LoGMask (QImage orig, QImage copy)
{
	QImage newPic (copy.width(), copy.height(), QImage::Format_RGB32);
	orig = orig.scaled (copy.width(), copy.height());

	orig = grayIm (orig);

	int log = 0;

	for (int y = 0; y < orig.height(); y++)
	{
		for (int x = 0; x < orig.width(); x++)
		{
			if (x == 0 || x== 1 || x == (orig.width() - 1) || x == (orig.width() - 2))
				log = 0;
			else if (y == 0 || y == 1 || y == (orig.height() - 1) || y == (orig.height() - 2))
				log = 0;
			else
			{
				log = 16*qBlue(orig.pixel(x,y)) - (qBlue(orig.pixel(x,y-2))+qBlue(orig.pixel(x-1,y-1))+2*qBlue(orig.pixel(x,y-1))+qBlue(orig.pixel(x+1,y-1))+qBlue(orig.pixel(x-2,y))+2*qBlue(orig.pixel(x-1,y))+2*qBlue(orig.pixel(x+1,y))+qBlue(orig.pixel(x+2,y))+qBlue(orig.pixel(x-1,y+1))+2*qBlue(orig.pixel(x,y+1))+qBlue(orig.pixel(x+1,y+1))+qBlue(orig.pixel(x,y+2)));
				if (log > 255)
					log = 255;
				else if (log < 0)
					log = 0;
			}
			newPic.setPixel (x, y, qRgb (log, log, log));
		}
	}

	return newPic;
}

// Luminance gray scale function.  Implemented for edge detection.
QImage Histo::grayIm (QImage im)
{
	QImage newPic (im.width(), im.height(), QImage::Format_RGB32);

	for (int y = 0; y < im.height(); y++)
	{
		for (int x = 0; x < im.width(); x++)
		{
			color = im.pixel (x, y);
			colorValue = 0.3 * qRed(color) + 0.59 * qGreen(color) + 0.11 * qBlue(color);
			newPic.setPixel (x, y, qRgb (colorValue, colorValue, colorValue));
		}
	}

	return newPic;
}
//Wai Khoo