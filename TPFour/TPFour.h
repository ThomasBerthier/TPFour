#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TPFour.h"
#include <Windows.h>
#include "Dask64.h"
#include "qtimer.h"


class TPFour : public QMainWindow
{
    Q_OBJECT

public:
    TPFour(QWidget *parent = Q_NULLPTR);
	~TPFour();
	void writeData(double value);

public slots:
	void fourOn();
	void fourOff();
	void updateUI();
	void getTemp();

private:
    Ui::TPFourClass ui;
	I16 card;
	QTimer * tempTimer;
};
