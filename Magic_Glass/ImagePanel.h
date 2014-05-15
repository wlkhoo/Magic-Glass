/*
	Written by Wai Khoo <wlkhoo@gmail.com>
	Contributed by Edgardo Molina.
	This file is responsible for creating an image panel, which holds an image.
	This class has total control over the mouse. As mouse hovers over an image,
			it will display its RGB values, xy coordinates, and the histograms of the current image.
	This class handle all images related action with help of other classes when needed.
			(threshold, single band, gray scale, and edge detection)
*/
#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <QtGui>
#include "label.h"
#include "histo.h"

class ImagePanel : public QWidget
{
	Q_OBJECT
 public:
  ImagePanel(QWidget* parent=0, Qt::WFlags f=0);
  ~ImagePanel();

  void reset();
  void show(QPixmap);
  void scaleImage (double factor);
  void redBand();
  void greenBand();
  void blueBand();
  void aveGrayScale();
  void lumGrayScale();
  void thresChanged (int value);
  void thresholdAll();
  void thresholdSin();
  void magic (bool ans);
  void prewittM();
  void sobelM();
  void LoGM();

public slots:
  void setRadius (int rad);

signals:
	void labelChanged (int r, int g, int b, int x, int y);
	void displayHisto (int rf, int gf, int bf);

protected:
  void paintEvent(QPaintEvent*);
  void mousePressEvent(QMouseEvent* e);
  void mouseReleaseEvent(QMouseEvent* e);
  void mouseMoveEvent(QMouseEvent* e);

 private:
  Label *rgb;
  Histo *histo;

  QImage image;
  QImage copyIm;
  QImage magicIm;

  QRgb color;
  int _px;
  int _py;
  int _x;
  int _y;
  int radius;
  int thresValue;

  double scaleWidth;
  double scaleHeight;

  bool _pressed;
  bool red;
  bool green;
  bool blue;
  bool aveGS;
  bool lumGS;
  bool magGla;
  bool thresAll;
  bool thresInd;
  bool prewitt;
  bool sobel;
  bool log;
};

#endif
//Wai Khoo
