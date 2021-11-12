#pragma once
#define _USE_MATH_DEFINES

#include <math.h>

#include <QDialog.h>
#include "ui_qgsattributetabledialog.h"
class MyAttTableView :
	public QDialog
{
public:
	MyAttTableView(QgsVectorLayer* layer,
		QgsMapCanvas* mapCanvas);
	~MyAttTableView();

private:
	Ui_QgsAttributeTableDialog  ui;
};
