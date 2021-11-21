#include "DiagramDialog.h"
#include "ui_DiagramDialog.h"

DiagramDialog::DiagramDialog(QWidget* parent)
	: QWidget(parent)
{
	ui = new Ui::DiagramDialog();
	ui->setupUi(this);

	connect(ui->btn, SIGNAL(clicked()), this, SLOT(btn_accepted()));
}

DiagramDialog::~DiagramDialog()
{
	delete ui;
}

void DiagramDialog::btn_accepted() {
	QVector<QString> data;

	data.push_back(ui->A->text());
	data.push_back(ui->RA->text());
	data.push_back(ui->GA->text());
	data.push_back(ui->BA->text());

	data.push_back(ui->B->text());
	data.push_back(ui->RB->text());
	data.push_back(ui->GB->text());
	data.push_back(ui->BB->text());

	emit sendData(data);

	this->hide();
}
