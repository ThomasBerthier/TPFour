//*********************************************************************************************
//* Programme : main.cpp								Date : 16/12/2021
//*														Dernière MAJ : 16/12/2021
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERNARD Léa							Classe : BTSSN2
//*					BERTHIER Thomas 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Permet d'appeler la classe TPFour qui gère le prograqmme
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
