/*
	Written by Wai Khoo <wlkhoo@gmail.com>
	This file create labels which is displayed on the right side of the window.
	Labels included RGB values, its histogram, the xy coordinates, threshold level, and magic class's radius.
*/
#ifndef LABEL_H
#define LABEL_H

#include <QtGui>

class Label : public QWidget
{
	Q_OBJECT

public:
	Label(QWidget *parent = 0);
	void disabledThres();
	void enableMagic(bool ans);

signals:
	void thresLevelChanged (int value);
	void changedRadius (int value);

public slots:
	void valuesChanged (int r, int g, int b, int x, int y);
	void histoChanged (int rh, int gh, int bh);
	void enabledThres();

private slots:
	void thresChanged (int value);
	void sliderChanged(int value);
	void radiusChanged(int value);

private:
	QLabel *red;
	QLabel *redValue;
	QLabel *redFreq;
	QLabel *redFreqValue;

	QLabel *green;
	QLabel *greenValue;
	QLabel *greenFreq;
	QLabel *greenFreqValue;

	QLabel *blue;
	QLabel *blueValue;
	QLabel *blueFreq;
	QLabel *blueFreqValue;

	QLabel *x;
	QLabel *xValue;
	QLabel *y;
	QLabel *yValue;

	QLabel *blank;
	QLabel *thres;
	QSlider *slider;
	QSpinBox *thresNum;

	QLabel *blank2;
	QLabel *radLab;
	QSpinBox *radius;

};
#endif
//Wai Khoo
