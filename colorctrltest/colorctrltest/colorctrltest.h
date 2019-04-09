#ifndef COLORCTRLTEST_H
#define COLORCTRLTEST_H

#include <QtWidgets/QWidget>
#include "ui_colorctrltest.h"

class colorctrltest : public QWidget
{
	Q_OBJECT

public:
	colorctrltest(QWidget *parent = 0);
	~colorctrltest();

private:
	Ui::colorctrltestClass ui;
};

#endif // COLORCTRLTEST_H
