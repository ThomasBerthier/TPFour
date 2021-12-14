#include "TPFour.h"
#include <qdebug.h>

TPFour::TPFour(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	card = Register_Card(PCI_9111DG, 0);
	ui.fourOff->setEnabled(false);
	ui.puissance->setEnabled(false);
	ui.puissance->setValue(0);
	tempTimer = new QTimer(this);
}

void TPFour::fourOn()
{
	card = Register_Card(PCI_9111DG, 0);
	ui.fourOn->setEnabled(false);
	ui.fourOff->setEnabled(true);
	ui.puissance->setEnabled(true); 
	ui.puissance->setValue(0);
	connect(tempTimer, SIGNAL(timeout()), this, SLOT(getTemp()));
	tempTimer->start(3000);

}
TPFour::~TPFour()
{
	writeData(0);
	Release_Card(card);
}
void TPFour::writeData(double value)
{
	AO_VWriteChannel(card, 0, value);
}
void TPFour::fourOff()
{
	ui.fourOn->setEnabled(true);
	ui.fourOff->setEnabled(false);
	ui.puissance->setEnabled(false);
	writeData(0);
	Release_Card(card);
	disconnect(tempTimer, SIGNAL(timeout()), this, SLOT(getTemp()));
}

void TPFour::updateUI()
{
	QString value = QString::number(ui.puissance->value());
	ui.valuePuissance->setText("Value : " + value + "%");

	double result = value.toDouble() / 10.0;
	qDebug() << result;
	writeData(result);
}

void TPFour::getTemp()
{
	qDebug() << "test";
	double tension;
	AI_VReadChannel(card, 0, AD_B_10_V, &tension);

	double temp = tension * 150 / 10;
	qDebug() << tension;
	qDebug() << temp;

	ui.affichage->addItem(QString::number(temp) + "°C");
}