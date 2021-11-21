#pragma once

#include <QDialog>
namespace Ui { class SimpleAnnDialog; };

class SimpleAnnDialog : public QDialog
{
	Q_OBJECT

public:
	SimpleAnnDialog(QWidget* parent = Q_NULLPTR);
	~SimpleAnnDialog();

private:
	Ui::SimpleAnnDialog* ui;

signals:
	void sendData(QVector<QString>);

private slots:
	void btn_accepted();
};
