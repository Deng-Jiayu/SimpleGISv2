#include "GraduatedSymbolDialog.h"
#include "ui_GraduatedSymbolDialog.h"

GraduatedSymbolDialog::GraduatedSymbolDialog(QWidget* parent)
	: QDialog(parent)
{
	ui = new Ui::GraduatedSymbolDialog();
	ui->setupUi(this);

	connect(ui->btn, SIGNAL(clicked()), this, SLOT(btn_accepted()));
}

GraduatedSymbolDialog::~GraduatedSymbolDialog()
{
	delete ui;
}

void GraduatedSymbolDialog::btn_accepted() {
	QVector<QString> data;

	data.push_back(ui->attribute->text());
	data.push_back(ui->R->text());
	data.push_back(ui->G->text());
	data.push_back(ui->B->text());

	emit sendData(data);

	this->hide();
}
