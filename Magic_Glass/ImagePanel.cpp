/*
	Written by Wai Khoo <wlkhoo@gmail.com
	Contributed by Edgardo Molina
	The implementation of ImagePanel.h
*/
#include <QtGui>
#include "ImagePanel.h"
#include "label.h"
#include "histo.h"

//	Constructor: setting up the background for the panel and initializes variables.
ImagePanel::ImagePanel (QWidget* parent, Qt::WFlags f)
  : QWidget(parent, f)
{
	rgb = new Label;
	histo = new Histo;
	setCursor (Qt::CrossCursor);
	QPalette pal;
	pal.setColor(QPalette::Window, QColor(Qt::black));
	pal.setColor(QPalette::WindowText, QColor(Qt::white));
	setPalette(pal);
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	resize(sizeHint());
	reset();
	setMouseTracking (true);
	log = prewitt = sobel = thresAll = thresInd = magGla = aveGS = lumGS = red = green = blue = false;
	thresValue = 0;
	radius = 60;
}

//	Destructor
ImagePanel::~ImagePanel() {

}

//	Reset some variables to original state.
void ImagePanel::reset() {
  _px = 0; _py = 0;
  image = QImage();
  copyIm = QImage();
  magicIm = QImage();
  repaint();
}

/*
	This function get called from the open() of MainWindow
	Convert the pixmap obatained from MainWindow to image, send it to Histo to calculate the histogram,
		and make a deep copy of it for later use.
*/
void ImagePanel::show(QPixmap p)
{
  image = p.toImage();
  copyIm = magicIm = image.copy(QRect());
  repaint();
}


//	The image zooming implementation.
void ImagePanel::scaleImage (double factor)
{
	scaleWidth = factor * (double)image.width();
	scaleHeight = factor * (double)image.height();
	magicIm = copyIm = image.scaled (scaleWidth, scaleHeight);
	if (prewitt)
		prewittM();
	else if (sobel)
		sobelM();
	else if (log)
		LoGM();
	else
		repaint();
}

// Setting red band to true and everything else to false
void ImagePanel::redBand()
{
	red = true;
	log = prewitt = sobel = thresAll = thresInd = aveGS = lumGS = green = blue = false;
}

// Setting green band to true and everything else to false
void ImagePanel::greenBand()
{
	green = true;
	log = prewitt = sobel = thresAll = thresInd = aveGS = lumGS = red = blue = false;
}

// Setting blue band to true and everything else to false
void ImagePanel::blueBand()
{
	blue = true;
	log = prewitt = sobel = thresAll = thresInd = aveGS = lumGS = red = green = false;
}

// Setting average grayscale to true and everything else to false
void ImagePanel::aveGrayScale()
{
	aveGS = true;
	log = prewitt = sobel = thresAll = thresInd = lumGS = red = green = blue = false;
}

// Setting luminance grayscale to true and everything else to false
void ImagePanel::lumGrayScale()
{
	lumGS = true;
	log = prewitt = sobel = thresAll = thresInd = aveGS = red = green = blue = false;
}

// Called from MainWindow.  MainWindow passes threshold value to here.
void ImagePanel::thresChanged (int value)
{
	thresValue = value;
	histo -> lookUpTable (thresValue);
}

// Setting threshold all to true and everything else to false.
void ImagePanel::thresholdAll()
{
	thresAll = true;
	log = prewitt = sobel = thresInd = aveGS = lumGS = red = green = blue = false;
}

// Setting threshold individual to true and everything else to false.
void ImagePanel::thresholdSin()
{
	thresInd = true;
	log = prewitt = sobel = thresAll = aveGS = lumGS = red = green = blue = false;

}

// magGla correspond whether magic glass is enable or not.
// If it is enabled, red band is the default.
// If not, draw the original back on the screen.
void ImagePanel::magic (bool ans)
{
	magGla = ans;
	if (magGla)
	{
		redBand();
		update();
	}
	else
	{
		image = image.scaled (copyIm.width(), copyIm.height());
		copyIm = image.copy(QRect());
		update();
	}
}

// Edge detection.
void ImagePanel::prewittM()
{
	prewitt = true;
	log = sobel = false;
	copyIm = histo -> prewittMask (image, copyIm);
	update();
}

// Edge detection
void ImagePanel::sobelM()
{
	sobel = true;
	log = prewitt = false;
	copyIm = histo -> sobelMask (image, copyIm);
	update();
}

// Edge detection
void ImagePanel::LoGM()
{
	log = true;
	prewitt = sobel = false;
	copyIm = histo -> LoGMask (image, copyIm);
	update();
}

// Obtain radius from Label
void ImagePanel::setRadius (int rad)
{
	radius = rad;
}

//	Paint the "current" image, which is copyIm.  If magic glass is enabled, also paint the glass on top of copyIm.
void ImagePanel::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  painter.setBackgroundMode(Qt::OpaqueMode);
  painter.setBackground(QBrush(Qt::black));
  if (magGla)
  {
	 painter.drawImage(_px,_py,magicIm);
  }
  else
  {
     painter.drawImage(_px,_py,copyIm);
  }
}

//	Stores the current coordinates when mouse pressed.
void ImagePanel::mousePressEvent(QMouseEvent* e) {
  _pressed = true;
  _x = e->x(); _y = e->y();
}

//	Set the _pressed state to false when user releases the mouse key.
void ImagePanel::mouseReleaseEvent(QMouseEvent*) {
  _pressed = false;
}

/*
	When mouse key pressed, move image 2-pixels at a time and then repaint it.
	If key is not pressed, get the RGB values, xy coordinates at current points.
	If magic glass is enabled, pass the current coordinate, radius, and images to histo to create magic glass.
*/
void ImagePanel::mouseMoveEvent(QMouseEvent* e)
{
 	  int x = e->x();
	  int y = e->y();

  if (_pressed)
  {
	  if (x > this -> width() || x < 0 || y > this -> height() || y < 0)
	  	return;

	  int dx = x - _x;
	  int dy = y - _y;

	  if(dx < 0) {
		dx += (x%2);
		x += (x%2);
	  } else {
		dx -= (x%2);
		x -= (x%2);
	  }

	  if(dy < 0) {
		dy += (y%2);
		y += (y%2);
	  } else {
		dy -= (y%2);
		y -= (y%2);
	  }

	  _x = x;
	  _y = y;
	  _px += dx;
	  _py += dy;
  	  update();
  }

	QPixmap pic = QPixmap::grabWidget(this);
 	QImage im = pic.toImage();
  	color = im.pixel (x, y);
	emit labelChanged (qRed(color), qGreen(color), qBlue(color), x, y);
	emit displayHisto (qRed(color), qGreen(color), qBlue(color));

	if (magGla)
	{
		histo -> setState (red, green, blue, aveGS, lumGS, thresAll, thresInd, thresValue);
		magicIm = histo -> magicGlass (image, copyIm, radius, (x - _px), (y - _py));
		repaint();
	}
}
//Wai Khoo
