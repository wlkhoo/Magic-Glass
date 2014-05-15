/*
	Written by Wai Khoo <wlkhoo@gmail.com>
	The implementation of Label.h
*/
#include <QtGui>
#include "label.h"

//	Constructor: setting up the background for the labels area and the labels itself.
Label::Label(QWidget *parent)
	: QWidget (parent)
{
	setPalette(QPalette(QColor(255, 255, 255)));
	setAutoFillBackground(true);

	red = new QLabel (tr("Red ="), this);
	redValue = new QLabel (this);
	redFreq = new QLabel (tr("Red \n Frequency ="), this);
	redFreqValue = new QLabel (this);

	green = new QLabel (tr("Green ="), this);
	greenValue = new QLabel (this);
	greenFreq = new QLabel (tr("Green \n Frequency ="), this);
	greenFreqValue = new QLabel (this);

	blue = new QLabel (tr("Blue ="), this);
	blueValue = new QLabel (this);
	blueFreq = new QLabel (tr("Blue \n Frequency ="), this);
	blueFreqValue = new QLabel (this);

	x = new QLabel (tr("X ="), this);
	xValue = new QLabel (this);
	y = new QLabel (tr("Y ="), this);
	yValue = new QLabel (this);

	blank = new QLabel (this);

	thres = new QLabel (tr("Threshold \nLevel = "), this);

	thresNum = new QSpinBox;
	thresNum -> setRange (0, 255);
	thresNum -> setValue (0);
	thresNum -> setDisabled (true);

	slider = new QSlider (Qt::Horizontal, this);
	slider -> setTickPosition(QSlider::TicksBelow);
	slider -> setRange (0, 255);
	slider -> setValue (0);
	slider -> setTickInterval (16);
	slider -> setDisabled(true);

	blank2 = new QLabel (this);

	radLab = new QLabel (tr("Radius of \nMagic Glass = "), this);

	radius = new QSpinBox;
	radius -> setRange (60, 100);
	radius -> setValue (60);
	radius -> setDisabled (true);

	redValue -> setFrameShape (QFrame::StyledPanel);
	redFreqValue -> setFrameShape (QFrame::StyledPanel);
	greenValue -> setFrameShape (QFrame::StyledPanel);
	greenFreqValue -> setFrameShape (QFrame::StyledPanel);
	blueValue -> setFrameShape (QFrame::StyledPanel);
	blueFreqValue -> setFrameShape (QFrame::StyledPanel);
	xValue -> setFrameShape (QFrame::StyledPanel);
	yValue -> setFrameShape (QFrame::StyledPanel);
	blank -> setFrameShape (QFrame::HLine);
	blank2 -> setFrameShape (QFrame::HLine);

	QGridLayout *layout = new QGridLayout;
	layout -> addWidget (red, 0, 0);
	layout -> addWidget (redValue, 0, 1);
	layout -> addWidget (redFreq, 0, 2);
	layout -> addWidget (redFreqValue, 0, 3);

	layout -> addWidget (green, 1, 0);
	layout -> addWidget (greenValue, 1, 1);
	layout -> addWidget (greenFreq, 1, 2);
	layout -> addWidget (greenFreqValue, 1, 3);

	layout -> addWidget (blue, 2, 0);
	layout -> addWidget (blueValue, 2, 1);
	layout -> addWidget (blueFreq, 2, 2);
	layout -> addWidget (blueFreqValue, 2, 3);

	layout -> addWidget (x, 3, 0);
	layout -> addWidget (xValue, 3, 1);
	layout -> addWidget (y, 3, 2);
	layout -> addWidget (yValue, 3, 3);

	layout -> addWidget (blank, 4, 0, 1, 4);
	layout -> addWidget (thres, 5, 0);
	layout -> addWidget (thresNum, 5, 2);
	layout -> addWidget (slider, 6, 0, 1, 4);

	layout -> addWidget (blank2, 7, 0, 1, 4);
	layout -> addWidget (radLab, 8, 0);
	layout -> addWidget (radius, 8, 2);

	layout -> setColumnMinimumWidth (3, 45);

	connect (thresNum, SIGNAL (valueChanged(int)), this, SLOT (thresChanged(int)));
	connect (slider, SIGNAL (valueChanged(int)), this, SLOT (sliderChanged(int)));
	connect (radius, SIGNAL (valueChanged(int)), this, SLOT (radiusChanged(int)));


	setLayout (layout);
}

// Disable threshold level features when the threshold options are not checked or not in use.
void Label::disabledThres()
{
	thresNum -> setEnabled (false);
	slider -> setEnabled (false);
}

// When the user enable magic glass, radius slider is enabled.
void Label::enableMagic (bool ans)
{
	radius -> setEnabled (ans);
}


//	A fuction, which get called by outside the class, that updates the RGB values and the xy coordinates.
void Label::valuesChanged (int r, int g, int b, int x, int y)
{
	redValue -> setNum (r);
	greenValue -> setNum (g);
	blueValue -> setNum (b);

	xValue -> setNum (x);
	yValue -> setNum (y);
}


//	A fuction, which get called by outside the class, that updates the RGB histogram values.
void Label::histoChanged (int rh, int gh, int bh)
{
	redFreqValue -> setNum (rh);
	greenFreqValue -> setNum (gh);
	blueFreqValue -> setNum (bh);
}

// Enable threshold level features when the threshold options are checked or in use.
void Label::enabledThres()
{
	thresNum -> setEnabled (true);
	slider -> setEnabled (true);
}

// Emit signal to update the slider
void Label::thresChanged (int value)
{
	slider -> setValue (value);
}

// Emit signal to update the spinbox and to global (catch by MainWindow).
void Label::sliderChanged(int value)
{
	thresNum -> setValue (value);
	emit thresLevelChanged (value);
}

// Emit signal about magic glass's radius change.
void Label::radiusChanged (int value)
{
	emit changedRadius (value);
}
//Wai Khoo
