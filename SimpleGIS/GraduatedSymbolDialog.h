#pragma once

#include <QDialog>
namespace Ui { class GraduatedSymbolDialog; };

class GraduatedSymbolDialog : public QDialog
{
	Q_OBJECT

public:
	GraduatedSymbolDialog(QWidget *parent = Q_NULLPTR);
	~GraduatedSymbolDialog();

private:
	Ui::GraduatedSymbolDialog *ui;

signals:
	void sendData(QVector<QString>);

private slots:
	void btn_accepted();
};
