//*********************************************************************************************
//* Programme : main.cpp								Date : 16/12/2021
//*														Derni�re MAJ : 16/12/2021
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERNARD L�a							Classe : BTSSN2
//*					BERTHIER Thomas 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Permet d'appeler la classe TPFour qui g�re le prograqmme
//*
//*********************************************************************************************
#include "TPFour.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TPFour w;
    w.show();
    return a.exec();
}
