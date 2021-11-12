#include "SimpleGIS.h"
#include <QtWidgets/QApplication>
#include <QgsApplication.h>

int main(int argc, char* argv[])
{
	QgsApplication a(argc, argv, true);//C:/Program Files/QGIS 3.16/apps/qgis-ltr/plugins
	//QgsApplication::init("C:/Program Files/QGIS 3.16");
	//QgsApplication::setPluginPath("C:/Program Files/QGIS 3.16/apps/qgis-ltr/python/plugins");
	QgsApplication::initQgis();

	SimpleGIS w;
	w.show();
	return a.exec();
}
