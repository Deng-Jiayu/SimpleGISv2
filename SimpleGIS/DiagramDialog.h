#pragma once

#include <QWidget>
namespace Ui { class DiagramDialog; };

class DiagramDialog : public QWidget
{
	Q_OBJECT

public:
	DiagramDialog(QWidget* parent = Q_NULLPTR);
	~DiagramDialog();

private:
	Ui::DiagramDialog* ui;

signals:
	void sendData(QVector<QString>);

private slots:
	void btn_accepted();
};
