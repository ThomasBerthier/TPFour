//*********************************************************************************************
//* Programme : TPFour.h								Date : 16/12/2021
//*														Derni�re MAJ : 16/12/2021
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERNARD L�a							Classe : BTSSN2
//*					BERTHIER Thomas 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Permet de d'allumer et d'�teindre le four � une certaine puissance. Permet �galement de visualiser le graphique de temp�rature.
//*
//*********************************************************************************************
#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <qchartview.h>
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
	void heatAtValue();
	void graphRefresh();
	void stopGraph();

private:
    Ui::TPFourClass ui;
	I16 card;
	QTimer * tempTimer;
	QTimer * graphTimer;
	double temp = 25;
	qreal tempo = 5;
	QLineSeries * series;
	QChart * chart;
	QChartView *graph;
	QCategoryAxis * axisY;
	QCategoryAxis * axisX;
	int numberOfGraphValues = 0;
};
