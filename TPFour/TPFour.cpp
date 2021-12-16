//*********************************************************************************************
//* Programme : TPFour.cpp								Date : 16/12/2021
//*														Dernière MAJ : 16/12/2021
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERNARD Léa							Classe : BTSSN2
//*					BERTHIER Thomas 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Permet de d'allumer et d'éteindre le four à une certaine puissance. Permet également de visualiser le graphique de température.
//*
//*********************************************************************************************
#include "TPFour.h"
#include <qdebug.h>

TPFour::TPFour(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	ui.fourOff->setEnabled(false);
	ui.puissance->setEnabled(false);
	ui.puissance->setValue(0);
	//Création des timers
	tempTimer = new QTimer(this);
	graphTimer = new QTimer(this);
}

//Méthode qui permet de demarrer le four
void TPFour::fourOn()
{
	//Connexion à la carte
	card = Register_Card(PCI_9111DG, 0);
	ui.fourOn->setEnabled(false);
	ui.fourOff->setEnabled(true);
	ui.puissance->setEnabled(true); 
	ui.puissance->setValue(0);
	//Connecte le slot pour récuperer la temperature et demarre le timer
	connect(tempTimer, SIGNAL(timeout()), this, SLOT(getTemp()));
	tempTimer->start(3000);

}
TPFour::~TPFour()
{
	writeData(0);
	Release_Card(card);
}
//Envoie la tension passé en paramètres à la carte 
void TPFour::writeData(double value)
{
	AO_VWriteChannel(card, 0, value);
}
//Methode qui permet d'éteindre le four
void TPFour::fourOff()
{
	ui.fourOn->setEnabled(true);
	ui.fourOff->setEnabled(false);
	ui.puissance->setEnabled(false);
	writeData(0);
	Release_Card(card);
	disconnect(tempTimer, SIGNAL(timeout()), this, SLOT(getTemp()));
	tempTimer->stop();
}
//Permet de récuperer la puissance choisi avec la slidebar et envoie cette valeur a la carte
void TPFour::updateUI()
{
	QString value = QString::number(ui.puissance->value());
	ui.valuePuissance->setText("Value : " + value + "%");

	//Passe le pourcentage en volt
	double result = value.toDouble() / 10.0;
	writeData(result);
}
//Permet de récuperer la temperature
void TPFour::getTemp()
{
	//Recupere la tension
	double tension;
	AI_VReadChannel(card, 0, AD_B_10_V, &tension);
	//transforme la tension en temperature
	temp = (51 * tension) - 73;

	ui.affichage->addItem(QString::number(temp) + " Degres Celsius");
	ui.valuesToWrite->setText("Valeur entre 25 et 65 degres celsius");
	ui.affichage->scrollToBottom();
}
//Méthode qui permet d'initialiser le graphique
void TPFour::heatAtValue()
{
	//Arrete toute action et recupere la temperature
	fourOff();
	card = Register_Card(PCI_9111DG, 0);
	getTemp();
	//recupere la valeur ecrite et la transforme en tension avant de l'envoyer a la carte si la valeur est superieur a 25
	int value = ui.valueToHeat->text().toInt();
	if(value >= 25) {
		if (value > 65)
			value = 65;
		float valueFloat = (value + 73.0) / 51.0;
		writeData(valueFloat);
		//Connecte les slots des timers
		connect(tempTimer, SIGNAL(timeout()), this, SLOT(getTemp()));
		tempTimer->start(3000);
		connect(graphTimer, SIGNAL(timeout()), this, SLOT(graphRefresh()));
		graphTimer->start(3000);
		//Créer un ensemble de points(QLineSeries) et un graphique(QChart)
		chart = new QChart();
		series = new QLineSeries(chart);
		//Parametrage du graphiques
		chart->legend()->hide();
		chart->setTitleBrush(QBrush(Qt::black));
		chart->setTitle("Graphique de Temperature en fonction du temps");
		//Création des axes du graphiques
		axisX = new QCategoryAxis();
		axisX->setRange(0, 600);
		axisX->setTickCount(30);
		axisX->append("0", 0);
		axisX->append("30", 30);
		axisX->append("60", 60);
		axisX->append("90", 90);
		axisX->append("120", 120);
		axisX->append("150", 150);
		axisX->append("180", 180);
		axisX->append("210", 210);
		axisX->append("240", 240);
		axisX->append("270", 270);
		axisX->append("300", 300);
		axisX->append("330", 330);
		axisX->append("360", 360);
		axisX->append("390", 390);
		axisX->append("420", 420);
		axisX->append("450", 450);
		axisX->append("480", 480);
		axisX->append("510", 510);
		axisX->append("540", 540);
		axisX->append("570", 570);
		axisX->append("600", 600);

		axisY = new QCategoryAxis();
		axisY->setRange(25, 65);
		axisY->setTickCount(10);
		axisY->append("25", 25);
		axisY->append("35", 35);
		axisY->append("45", 45);
		axisY->append("55", 55);
		axisY->append("65", 65);
		//Ajoute un point a la QLineSeries
		series->append(numberOfGraphValues, temp);
		// variable qui sert a placer les points en abscisses
		numberOfGraphValues += 3;
		//Ajoute l'ensemble de points et les axes au graphiques
		chart->addSeries(series);
		chart->setAxisX(axisX, series);
		chart->setAxisY(axisY, series);
		
		//Créer l'affichage du graphique et ses parametrages
		graph = new QChartView(chart);
		graph->setRenderHint(QPainter::Antialiasing);
		//Stocke l'affichage du graphique dans une boite pour eviter de l'ouvrir dans une nouvelle page
		ui.gridLayout->addWidget(graph, 0, 0);
	}
	else {

		ui.valueToHeat->setText("Valeur non valide");
	}
	
}
//Permet de mettre a jour le graphique dynamiquement
void TPFour::graphRefresh()
{
	//Verifie si le graphique est pas complet
	if(numberOfGraphValues == 603){
		//deconnecte les slots et stop les timer, emets a zero la variable qui sert a placer les points en abscisses et arrete de faire chauffer le four
		disconnect(graphTimer, SIGNAL(timeout()), this, SLOT(graphRefresh()));
		disconnect(tempTimer, SIGNAL(timeout()), this, SLOT(getTemp()));
		tempTimer->stop();
		numberOfGraphValues = 0;
		writeData(0);
		return;
	}
	//Créer un poi_nt et l'ajoute a la QLineSeries
	QPointF pt(numberOfGraphValues, temp);
	((QLineSeries*)chart->series().first())->append(pt);
		numberOfGraphValues+=3;
		//Met a jour le graph et l'affichage du graph
		chart->update();
		graph->update();
		//Supprime l'ancien ensemble de points et remets le nouveau car l'ensemble de points ne se mets pas a jour tout seul
		chart->removeSeries(series);
		chart->addSeries(series);
		//Remets les axes qui ont été supprimés en supprimant l'ensemble de points
		chart->setAxisX(axisX, series);
		chart->setAxisY(axisY, series);
}
//Arrete la construction du graphique
void TPFour::stopGraph()
{
	//deconnecte les slots, remets la variables de l'axe des abscisses a zero et arrete de faire chauffer le four 
	disconnect(tempTimer, SIGNAL(timeout()), this, SLOT(getTemp()));
	tempTimer->stop();
	disconnect(graphTimer, SIGNAL(timeout()), this, SLOT(graphRefresh()));
	numberOfGraphValues = 0;
	writeData(0);
	Release_Card(card);
}
