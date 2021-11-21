#include "SimpleAnnDialog.h"
#include "ui_SimpleAnnDialog.h"

SimpleAnnDialog::SimpleAnnDialog(QWidget* parent)
	: QDialog(parent)
{
	ui = new Ui::SimpleAnnDialog();
	ui->setupUi(this);

	connect(ui->btn, SIGNAL(clicked()), this, SLOT(btn_accepted()));
}

SimpleAnnDialog::~SimpleAnnDialog()
{
	delete ui;
}

void SimpleAnnDialog::btn_accepted() {
	QVector<QString> data;

	data.push_back(ui->attribute->text());
	data.push_back(ui->R->text());
	data.push_back(ui->G->text());
	data.push_back(ui->B->text());
	data.push_back(ui->font->text());

	emit sendData(data);

	this->hide();
}
