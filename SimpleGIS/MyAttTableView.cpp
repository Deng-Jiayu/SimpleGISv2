#include "MyAttTableView.h"

MyAttTableView::MyAttTableView(QgsVectorLayer* layer,
	QgsMapCanvas* mapCanvas)
{
	ui.setupUi(this);
	ui.mMainView->init(layer, mapCanvas);
	ui.mMainViewButtonGroup->setId(ui.mTableViewButton, QgsDualView::AttributeTable);
	ui.mMainViewButtonGroup->setId(ui.mAttributeViewButton, QgsDualView::AttributeEditor);
}


MyAttTableView::~MyAttTableView()
{
}
