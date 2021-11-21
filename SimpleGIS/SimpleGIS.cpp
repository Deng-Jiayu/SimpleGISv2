#include "SimpleGIS.h"

#include <QFileDialog>
#include <QgsVectorLayer.h>
#include <QgsMaptoolZoom.h>

SimpleGIS::SimpleGIS(QWidget* parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	this->show();

	this->m_SimpleAnnDialog = new SimpleAnnDialog;
	connect(m_SimpleAnnDialog, SIGNAL(sendData(QVector<QString>)), this, SLOT(receiveData(QVector<QString>)));

	this->m_GraduatedSymbolDialog = new GraduatedSymbolDialog;
	connect(m_GraduatedSymbolDialog, SIGNAL(sendData(QVector<QString>)), this, SLOT(receiveData2(QVector<QString>)));

	this->m_DiagramDialog = new DiagramDialog;
	connect(m_DiagramDialog, SIGNAL(sendData(QVector<QString>)), this, SLOT(receiveData3(QVector<QString>)));

	mapCanvas = new QgsMapCanvas;

	mapProject = QgsProject::instance();

	setCentralWidget(mapCanvas);

	QMenu* m = new QMenu(QStringLiteral("基础功能"));
	QAction* addVector = m->addAction(QStringLiteral("添加矢量数据"), this, &SimpleGIS::addVectorLayer);
	addVector->setIcon(QIcon(QStringLiteral(":/icons/文件.svg")));
	QAction* addRasterFile = m->addAction(QStringLiteral("添加栅格数据"), this, &SimpleGIS::addRasterFile);
	addRasterFile->setIcon(QIcon(QStringLiteral(":/icons/栅格.svg")));
	QAction* zoomIn = m->addAction(QStringLiteral("放大"), this, &SimpleGIS::ZoomIn);
	zoomIn->setIcon(QIcon(QStringLiteral(":/icons/放大.svg")));
	QAction* zoomOut = m->addAction(QStringLiteral("缩小"), this, &SimpleGIS::ZoomOut);
	zoomOut->setIcon(QIcon(QStringLiteral(":/icons/缩小.svg")));
	QAction* pan = m->addAction(QStringLiteral("移动"), this, &SimpleGIS::Pan);
	pan->setIcon(QIcon(QStringLiteral(":/icons/移动.svg")));
	QAction* fullExtent = m->addAction(QStringLiteral("复位"), this, &SimpleGIS::FullExtent);
	fullExtent->setIcon(QIcon(QStringLiteral(":/icons/复位.svg")));
	QAction* identify = m->addAction(QStringLiteral("识别"), this, &SimpleGIS::Identify);
	identify->setIcon(QIcon(QStringLiteral(":/icons/选择.svg")));
	QAction* zoomToFeature = m->addAction(QStringLiteral("缩放到选定要素"), this, &SimpleGIS::ZoomToFeature);
	zoomToFeature->setIcon(QIcon(QStringLiteral(":/icons/缩放.svg")));
	QAction* openOverView = m->addAction(QStringLiteral("打开鹰眼"), this, &SimpleGIS::OpenOverView);
	openOverView->setIcon(QIcon(QStringLiteral(":/icons/鹰眼.svg")));

	m->addSeparator();

	QAction* createDoc = m->addAction(QStringLiteral("创建文档"), this, &SimpleGIS::CreateDoc);
	createDoc->setIcon(QIcon(QStringLiteral(":/icons/创建文件夹.svg")));
	QAction* saveDoc = m->addAction(QStringLiteral("保存文档"), this, &SimpleGIS::SaveDoc);
	saveDoc->setIcon(QIcon(QStringLiteral(":/icons/保存.svg")));
	QAction* saveAsDoc = m->addAction(QStringLiteral("另存文档"), this, &SimpleGIS::SaveAsDoc);
	saveAsDoc->setIcon(QIcon(QStringLiteral(":/icons/另存为.svg")));
	QAction* openDoc = m->addAction(QStringLiteral("打开文档"), this, &SimpleGIS::OpenDoc);
	openDoc->setIcon(QIcon(QStringLiteral(":/icons/文件夹.svg")));
	QAction* layerManage = m->addAction(QStringLiteral("图层管理"), this, &SimpleGIS::LayerManage);
	layerManage->setIcon(QIcon(QStringLiteral(":/icons/图层管理.svg")));


	ui.menuBar->addMenu(m);


	ui.mainToolBar->addAction(createDoc);
	ui.mainToolBar->addAction(openDoc);
	ui.mainToolBar->addAction(saveDoc);
	ui.mainToolBar->addAction(saveAsDoc);
	ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(addVector);
	ui.mainToolBar->addAction(addRasterFile);
	ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(zoomIn);
	ui.mainToolBar->addAction(zoomOut);
	ui.mainToolBar->addAction(pan);
	ui.mainToolBar->addAction(fullExtent);
	ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(identify);
	ui.mainToolBar->addAction(zoomToFeature);
	ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(openOverView);
	ui.mainToolBar->addAction(layerManage);
	ui.mainToolBar->addSeparator();

	m = new QMenu(QStringLiteral("空间数据管理"));

	QAction* catalog = m->addAction(QStringLiteral("数据目录"), this, &SimpleGIS::Catalog);
	catalog->setIcon(QIcon(QStringLiteral(":/icons/目录.svg")));
	m->addAction(QStringLiteral("图层属性"), this, &SimpleGIS::AttShow);
	m->addAction(QStringLiteral("图层属性QGIS"), this, &SimpleGIS::AttShowQGIS);
	QAction* attShowMy = m->addAction(QStringLiteral("图层属性My"), this, &SimpleGIS::AttShowMy);
	attShowMy->setIcon(QIcon(QStringLiteral(":/icons/属性.svg")));

	m->addSeparator();
	QAction* spaQuery = m->addAction(QStringLiteral("空间查询-点"), this, &SimpleGIS::SpaQuery);
	spaQuery->setIcon(QIcon(QStringLiteral(":/icons/空间查询点.svg")));
	QAction* spaPolygonQuery = m->addAction(QStringLiteral("空间查询-多边形"), this, &SimpleGIS::SpaPolygonQuery);
	spaPolygonQuery->setIcon(QIcon(QStringLiteral(":/icons/空间查询多边形.svg")));
	QAction* spaFreeHandQuery = m->addAction(QStringLiteral("空间查询-手绘"), this, &SimpleGIS::SpaFreeHandQuery);
	spaFreeHandQuery->setIcon(QIcon(QStringLiteral(":/icons/空间查询手绘.svg")));
	QAction* spaSimpleQuery = m->addAction(QStringLiteral("空间查询-矩形"), this, &SimpleGIS::SpaSimpleQuery);
	spaSimpleQuery->setIcon(QIcon(QStringLiteral(":/icons/空间查询矩形.svg")));

	ui.menuBar->addMenu(m);

	ui.mainToolBar->addAction(catalog);
	ui.mainToolBar->addAction(attShowMy);
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addAction(spaPolygonQuery);
	ui.mainToolBar->addAction(spaSimpleQuery);


	m = new QMenu(QStringLiteral("地图制图"));
	m->addAction(QStringLiteral("添加要素"), this, &SimpleGIS::FeatureEdit);
	m->addSeparator();
	m->addAction(QStringLiteral("设置简单注记参数"), this, &SimpleGIS::setSimpleAnnPara);
	m->addAction(QStringLiteral("简单注记"), this, &SimpleGIS::SimpleAnn);
	m->addAction(QStringLiteral("注记配置"), this, &SimpleGIS::AnnSetting);
	m->addSeparator();
	m->addAction(QStringLiteral("单一符号渲染"), this, &SimpleGIS::SingleSymbol);
	m->addAction(QStringLiteral("分类符号渲染"), this, &SimpleGIS::CategorizedSymbol);
	m->addAction(QStringLiteral("设置分级符号渲染参数"), this, &SimpleGIS::setGraduatedSymbolPara);
	m->addAction(QStringLiteral("分级符号渲染"), this, &SimpleGIS::GraduatedSymbol);
	m->addSeparator();
	m->addAction(QStringLiteral("设置图形渲染参数"), this, &SimpleGIS::setDiagramPara);
	m->addAction(QStringLiteral("比例饼图"), this, &SimpleGIS::PieDiagram);
	m->addAction(QStringLiteral("比例文本图"), this, &SimpleGIS::TextDiagram);
	m->addAction(QStringLiteral("比例直方图"), this, &SimpleGIS::HistogramDiagram);
	m->addAction(QStringLiteral("比例累积图"), this, &SimpleGIS::StackBarDiagram);
	ui.menuBar->addMenu(m);


	m = new QMenu(QStringLiteral("GPS"));
	m->addAction(QStringLiteral("加载数据"), this, &SimpleGIS::LoadGPSData);
	m->addAction(QStringLiteral("GPS数据目录"), this, &SimpleGIS::GPSCatalog);
	ui.menuBar->addMenu(m);

	m = new QMenu(QStringLiteral("帮助"));
	m->addAction(QStringLiteral("查看帮助"), this, &SimpleGIS::Help);
	ui.menuBar->addMenu(m);
}

SimpleGIS::~SimpleGIS()
{
	delete m_tv;
	delete m_tfm;
	delete mapCanvas;
	delete mapProject;
	delete m_treeView;
	delete m_treeModel;
}

void SimpleGIS::addVectorLayer()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Shape File(*.shp)", 0);
	if (!fileName.isNull()) {
		QStringList temp = fileName.split(QDir::separator());
		QString basename = temp.at(temp.size() - 1);
		QgsVectorLayer* vecLayer = new QgsVectorLayer(fileName, basename);
		//QgsVectorLayer* vecLayer = new QgsVectorLayer("file:///G:/exp/20140808_train.txt?type=csv&useHeader=No&maxFields=10000&detectTypes=yes&xField=field_3&yField=field_2&crs=EPSG:4326&spatialIndex=no&subsetIndex=no&watchFile=no", "test","delimitedtext");

		//QList <QgsMapLayer*> myLayerSet;
		//myLayerSet.append(vecLayer);

		mapCanvas->setLayers(mapCanvas->layers() << vecLayer);
		//mapCanvas->setExtent(vecLayer->extent());
		mapCanvas->zoomToFullExtent();
		mapCanvas->enableAntiAliasing(true);
		mapCanvas->setCanvasColor(QColor(255, 255, 255));
		mapCanvas->freeze(false);
		// Set the Map Canvas Layer Set

		mapCanvas->setVisible(true);
		mapCanvas->refresh();

		mapProject->addMapLayers(mapCanvas->layers());
	}
	this->status = 2;
}

#include <QMessageBox>
#include <QgsRasterLayer.h>
void SimpleGIS::addRasterFile()
{
	// 获取文件名称
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Raster File(*.tif)", 0);
	if (filename.isEmpty())
	{
		QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("文件名称为空")); return;
	}
	// 获取栅格图层
	QgsRasterLayer* my_rasterlayer = nullptr;
	QStringList temp = filename.split(QDir::separator());
	QString basename = temp.at(temp.size() - 1);
	QString provideKey = "";
	if (provideKey.isEmpty())
	{
		my_rasterlayer = new QgsRasterLayer(filename, basename);
	}
	else
	{
		my_rasterlayer = new QgsRasterLayer(filename, basename, provideKey);
	}
	if (!my_rasterlayer->isValid())
	{
		QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("栅格图层无效")); return;
	}
	// 显示影像
	//my_rasterlayer->setCrs(QgsCoordinateReferenceSystem("EPSG::32649") );
	mapCanvas->setLayers(mapCanvas->layers() << my_rasterlayer);
	mapCanvas->zoomToFullExtent();
	mapCanvas->refresh();

	mapProject->addMapLayers(mapCanvas->layers());

	this->status = 2;
}

void SimpleGIS::ZoomIn()
{
	QgsMapToolZoom* mpZoomInTool = new QgsMapToolZoom(mapCanvas, false);
	mapCanvas->setMapTool(mpZoomInTool);
}

void SimpleGIS::ZoomOut()
{
	QgsMapToolZoom* mpZoomInTool = new QgsMapToolZoom(mapCanvas, true);
	mapCanvas->setMapTool(mpZoomInTool);
}

#include <QgsMapToolPan.h>
void SimpleGIS::Pan()
{
	QgsMapToolPan* mpPan = new QgsMapToolPan(mapCanvas);
	mapCanvas->setMapTool(mpPan);
}

void SimpleGIS::FullExtent()
{
	mapCanvas->zoomToFullExtent();
}

#include <QgsMapToolIdentify.h>
#include <QgsAttributeTableView.h>
#include <QgsGui.h>
#include <QgsEditorWidgetRegistry.h>
#include <QgsAttributeForm.h>
#include <QgsMapMouseEvent.h>
class MyIdentify :public QgsMapToolIdentify
{
public:
	MyIdentify(QgsMapCanvas* canvas, QgsAttributeTableView* tv) :QgsMapToolIdentify(canvas), m_tv(tv) {}
	void canvasReleaseEvent(QgsMapMouseEvent* e)
	{
		QgsPointXY point = mCanvas->getCoordinateTransform()->toMapCoordinates(e->x(), e->y());
		QList<IdentifyResult> result = QgsMapToolIdentify::identify(QgsGeometry::fromPointXY(point), IdentifyMode::TopDownAll, AllLayers);
		//QgsVectorLayer *layer = (QgsVectorLayer*)(mCanvas->currentLayer());

		mCanvas->setSelectionColor(Qt::red);
		QgsVectorLayer* layer = (QgsVectorLayer*)(mCanvas->layers()[0]);
		layer->removeSelection();
		layer->selectByIds(QgsFeatureIds() << result[0].mFeature.id());

		//QgsEditorWidgetRegistry::initEditors(mCanvas);
		QgsGui::editorWidgetRegistry()->initEditors(mCanvas);

		QgsAttributeForm* form = new QgsAttributeForm(layer, result[0].mFeature);
		form->setMode(QgsAttributeEditorContext::SingleEditMode);
		form->show();

		m_tv->scrollToFeature(result[0].mFeature.id());
	}
private:
	QgsAttributeTableView* m_tv;
};

void SimpleGIS::Identify()
{
	MyIdentify* tool = new MyIdentify(mapCanvas, m_tv);
	//tool->m_tv = m_tv;
	//QgsMapToolIdentifyAction *tool = new QgsMapToolIdentifyAction(mapCanvas);

	//tool->setLayer((QgsVectorLayer*)(mapCanvas->layers()[0]));
	//connect(tool, static_cast<void(QgsMapToolIdentifyFeature::*)(QgsFeatureId)>(&QgsMapToolIdentifyFeature::featureIdentified), this, &SimpleGIS::FlashFeature);
	//connect(tool, SIGNAL(featureIdentified(QgsFeatureId)), this, SLOT(FlashFeature(QgsFeatureId)));
	mapCanvas->setMapTool(tool);
}

void SimpleGIS::FlashFeature(QgsFeatureId id)
{

	//mapCanvas->flashFeatureIds((QgsVectorLayer*)(mapCanvas->layers()[0]), QgsFeatureIds()<<id);
}

void SimpleGIS::ZoomToFeature()
{
	mapCanvas->zoomToSelected((QgsVectorLayer*)(mapCanvas->layers()[0]));
}

#include <QgsMapOverviewCanvas.h>
#include <QDockWidget>
void SimpleGIS::OpenOverView()
{
	QgsMapOverviewCanvas* overview = new QgsMapOverviewCanvas(NULL, mapCanvas);
	overview->setLayers(mapCanvas->layers());
	overview->setBackgroundColor(QColor(255, 255, 255));
	overview->enableAntiAliasing(true);
	//overview->setMinimumWidth(100);
	overview->setMinimumHeight(100);
	overview->setVisible(true);
	overview->refresh();

	//overview->show();
	QDockWidget* overView = new QDockWidget(QStringLiteral("鹰眼"));
	overView->setWidget(overview);
	overView->setObjectName(QStringLiteral("鹰眼"));
	overView->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, overView);

}

void SimpleGIS::CreateDoc()
{
	if (this->status == 2) {

		QMessageBox box;
		box.setWindowTitle(QStringLiteral("警告"));
		box.setText(QStringLiteral("当前文档尚未保存，是否保存"));
		QPushButton* yesBtn = box.addButton(QStringLiteral("是(&Y)"), QMessageBox::YesRole);
		QPushButton* noBtn = box.addButton(QStringLiteral("否(&N)"), QMessageBox::NoRole);
		QPushButton* cancleBtn = box.addButton(QStringLiteral("取消"), QMessageBox::RejectRole);
		box.exec();

		if (box.clickedButton() == yesBtn) {
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", QStringLiteral("地图文档(*.qgs)"), 0);
			this->currentFileName = fileName;
			mapProject->write(currentFileName);
		}
		else if (box.clickedButton() == cancleBtn) {
			return;
		}
	}

	mapProject->clear();
	mapProject->addMapLayers(mapCanvas->layers());
	this->status = 1;
}

void SimpleGIS::SaveDoc()
{
	//mapProject = new QgsProject;
	//mapProject->addMapLayer(mapCanvas->currentLayer());

	mapProject->addMapLayers(mapCanvas->layers());
	if (currentFileName == NULL) {
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", QStringLiteral("地图文档(*.qgs)"), 0);
		this->currentFileName = fileName;
		mapProject->write(currentFileName);
	}
	else {
		mapProject->write(currentFileName);
	}
	this->status = 3;
}

void SimpleGIS::SaveAsDoc()
{
	//mapProject = new QgsProject;
	//mapProject->addMapLayer(mapCanvas->currentLayer());

	mapProject->addMapLayers(mapCanvas->layers());
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", QStringLiteral("地图文档(*.qgs)"), 0);
	this->currentFileName = fileName;
	mapProject->write(currentFileName);

	this->status = 3;
}

#include <QgsLayerTree.h>
void SimpleGIS::OpenDoc()
{
	mapProject = QgsProject::instance();
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", QStringLiteral("地图文档(*.qgs)"), 0);
	if (!fileName.isNull()) {

		mapProject->read(fileName);
		//mapCanvas->setProject(mapProject);
		mapCanvas->setLayers(mapProject->layerTreeRoot()->layerOrder());

		mapCanvas->zoomToFullExtent();
		mapCanvas->enableAntiAliasing(true);
		mapCanvas->setCanvasColor(QColor(255, 255, 255));
		mapCanvas->freeze(false);
		mapCanvas->setVisible(true);
		mapCanvas->refresh();

	}
}

#include <QgsLayerTreeView.h>
#include <QgsLayerTreeViewDefaultActions.h>
class MyMenuProvider :public QgsLayerTreeViewMenuProvider
{
public:
	MyMenuProvider(QgsLayerTreeView* view, QgsMapCanvas* ctrl) :treeView(view), mapCtrl(ctrl) {}

	virtual QMenu* createContextMenu()
	{
		QMenu* menu = new QMenu;
		QgsLayerTreeViewDefaultActions* actions = treeView->defaultActions();
		QModelIndex idx = treeView->currentIndex();
		if (!idx.isValid())
			menu->addAction(actions->actionAddGroup());
		else
			menu->addAction(actions->actionZoomToLayer(mapCtrl));
		return menu;
	}
private:
	QgsMapCanvas* mapCtrl;
	QgsLayerTreeView* treeView;
};

#include <QgsLayerTreeModel.h>
#include <QgsLayerTreeMapCanvasBridge.h>
#include <QgsCustomLayerOrderWidget.h>
void SimpleGIS::LayerManage()
{
	QgsLayerTreeMapCanvasBridge* bridge = new QgsLayerTreeMapCanvasBridge(mapProject->layerTreeRoot(), mapCanvas);

	QgsCustomLayerOrderWidget* view = new QgsCustomLayerOrderWidget(bridge);

	QDockWidget* overView = new QDockWidget(QStringLiteral("图层管理"));
	overView->setWidget(view);
	overView->setObjectName(QStringLiteral("图层管理"));
	overView->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, overView);
}

#include <QgsBrowserGuiModel.h>
#include <QgsBrowserDockWidget.h>
void SimpleGIS::Catalog()
{
	QgsBrowserGuiModel* model = new QgsBrowserGuiModel;
	QgsBrowserDockWidget* view = new QgsBrowserDockWidget(QStringLiteral("数据目录"), model);
	addDockWidget(Qt::LeftDockWidgetArea, view);
}

void SimpleGIS::AttShow()
{
	QgsVectorLayer* vectorLayer = (QgsVectorLayer*)(mapCanvas->layers()[0]);


	QgsVectorLayerCache* lc = new QgsVectorLayerCache(vectorLayer, vectorLayer->featureCount());
	QgsAttributeTableView* tv = new QgsAttributeTableView();
	QgsAttributeTableModel* tm = new QgsAttributeTableModel(lc);
	tm->loadLayer(); // 一定不要忘记，否则model里面没有图层的属性数据

	QgsAttributeTableFilterModel* tfm = new QgsAttributeTableFilterModel(mapCanvas, tm, tm);
	tfm->setFilterMode(QgsAttributeTableFilterModel::ShowAll);
	tv->setModel(tfm);
	m_tv = tv;
	//tv->show();
	QDockWidget* overView = new QDockWidget(QStringLiteral("属性浏览"));
	overView->setWidget(tv);
	overView->setObjectName(QStringLiteral("属性浏览"));
	overView->setAllowedAreas(Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, overView);
}

#include "MyAttTableView.h"
void SimpleGIS::AttShowQGIS()
{
	QgsGui::editorWidgetRegistry()->initEditors(mapCanvas);

	QgsVectorLayer* vectorLayer = (QgsVectorLayer*)(mapCanvas->layers()[0]);
	MyAttTableView* dlg = new MyAttTableView(vectorLayer, mapCanvas);
	//dlg->show();
	QDockWidget* overView = new QDockWidget(QStringLiteral("属性浏览"));
	overView->setWidget(dlg);
	overView->setObjectName(QStringLiteral("属性浏览"));
	overView->setAllowedAreas(Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, overView);
}

#include <QgsExpressionContextUtils.h>
void MyAttQuery::DoQuery()
{
	QgsExpression filterExpression(lineEdit->text());
	if (filterExpression.hasParserError())
	{
		return;
	}

	QgsExpressionContext context(QgsExpressionContextUtils::globalProjectLayerScopes(mapCanvas->layers()[0]));

	if (!filterExpression.prepare(&context))
	{
		return;
	}

	mFilterModel->setFilterExpression(filterExpression, context);
	mFilterModel->filterFeatures();

	mFilterModel->setFilterMode(QgsAttributeTableFilterModel::ShowFilteredList);
}

#include <QgsExpressionBuilderDialog.h>
class MyExpBuilder :public QgsExpressionBuilderDialog
{
public:
	MyExpBuilder(QgsVectorLayer* layer, QLineEdit* lineEdit) :QgsExpressionBuilderDialog(layer), lineEdit(lineEdit) {}
protected:
	void accept()
	{
		lineEdit->setText(expressionText());
	}

private:
	QLineEdit* lineEdit;
};

void MyAttQuery::BuildQuery()
{
	//MyExpBuilder *dlg = new MyExpBuilder((QgsVectorLayer*)(mapCanvas->layers()[0]), lineEdit);
	//dlg->show();

	QgsExpressionBuilderDialog dlg((QgsVectorLayer*)(mapCanvas->layers()[0]));
	if (dlg.exec() == QDialog::Accepted)
	{
		lineEdit->setText(dlg.expressionText());
	}
}

void SimpleGIS::AttShowMy()
{
	//QWidget *view = new QWidget;
	//Ui::Form *form = new Ui::Form;
	//form->setupUi(view);
	MyAttQuery* form = new MyAttQuery(mapCanvas);

	QgsVectorLayer* vectorLayer = (QgsVectorLayer*)(mapCanvas->layers()[0]);


	QgsVectorLayerCache* lc = new QgsVectorLayerCache(vectorLayer, vectorLayer->featureCount());
	QgsAttributeTableView* tv = form->tableView;//new QgsAttributeTableView();

	QgsAttributeTableModel* tm = new QgsAttributeTableModel(lc);
	tm->loadLayer(); // 一定不要忘记，否则model里面没有图层的属性数据

	QgsAttributeTableFilterModel* tfm = new QgsAttributeTableFilterModel(mapCanvas, tm, tm);
	tfm->setFilterMode(QgsAttributeTableFilterModel::ShowAll);
	//tfm->setFilterMode(QgsAttributeTableFilterModel::ShowSelected);
	tv->setModel(tfm);
	m_tv = tv;
	m_tfm = tfm;
	form->mFilterModel = tfm;
	//tv->show();
	QDockWidget* overView = new QDockWidget(QStringLiteral("属性浏览"));
	overView->setWidget(form);
	overView->setObjectName(QStringLiteral("属性浏览"));
	overView->setAllowedAreas(Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, overView);
}

#include "qgsmaptoolselect.h"
void SimpleGIS::SpaQuery()
{
	QgsMapToolSelect* tool = new QgsMapToolSelect(mapCanvas);
	mapCanvas->setCurrentLayer(mapCanvas->layers()[0]);
	mapCanvas->setMapTool(tool);

	if (m_tfm)
		m_tfm->setFilterMode(QgsAttributeTableFilterModel::ShowSelected);
}

void SimpleGIS::SpaPolygonQuery()
{
	QgsMapToolSelect* tool = new QgsMapToolSelect(mapCanvas);
	tool->setSelectionMode(QgsMapToolSelectionHandler::SelectPolygon);
	mapCanvas->setCurrentLayer(mapCanvas->layers()[0]);
	mapCanvas->setMapTool(tool);

	if (m_tfm)
		m_tfm->setFilterMode(QgsAttributeTableFilterModel::ShowSelected);
}

void SimpleGIS::SpaFreeHandQuery()
{
	QgsMapToolSelect* tool = new QgsMapToolSelect(mapCanvas);
	tool->setSelectionMode(QgsMapToolSelectionHandler::SelectFreehand);
	mapCanvas->setCurrentLayer(mapCanvas->layers()[0]);
	mapCanvas->setMapTool(tool);

	if (m_tfm)
		m_tfm->setFilterMode(QgsAttributeTableFilterModel::ShowSelected);
}

void SimpleGIS::SpaSimpleQuery()
{
	QgsMapToolSelect* tool = new QgsMapToolSelect(mapCanvas);
	tool->setSelectionMode(QgsMapToolSelectionHandler::SelectSimple);
	mapCanvas->setCurrentLayer(mapCanvas->layers()[0]);
	mapCanvas->setMapTool(tool);

	if (m_tfm)
		m_tfm->setFilterMode(QgsAttributeTableFilterModel::ShowSelected);
}

void SimpleGIS::LoadGPSData()
{
	QgsVectorLayer* vecLayer = new QgsVectorLayer("file:///C:/code/VS2019/SimpleGIS/SimpleGIS/GPS50W.txt?type=csv&useHeader=No&maxFields=10000&detectTypes=yes&xField=field_3&yField=field_2&crs=EPSG:4326&spatialIndex=no&subsetIndex=no&watchFile=no", "test", "delimitedtext");

	//QList <QgsMapLayer*> myLayerSet;
	//myLayerSet.append(vecLayer);
	mapCanvas->setLayers(mapCanvas->layers() << vecLayer);
	//mapCanvas->setExtent(vecLayer->extent());
	mapCanvas->zoomToFullExtent();
	mapCanvas->enableAntiAliasing(true);
	mapCanvas->setCanvasColor(QColor(255, 255, 255));
	mapCanvas->freeze(false);
	// Set the Map Canvas Layer Set

	mapCanvas->setVisible(true);
	mapCanvas->refresh();
}

void SimpleGIS::GPSCatalog()
{
	//ifstream f("g:/exp/")
	//QFile file(":/menu/default.txt");
	//file.open(QIODevice::ReadOnly);
	//TreeModel *model=new TreeModel(file.readAll());
	//file.close();
	GPSTreeModel* model = new GPSTreeModel(":/GPS50W.txt");

	QTreeView* view = new QTreeView;
	view->setModel(model);
	//view-setWindowTitle(QObject::tr("Simple Tree Model"));
	//view.show();
	m_treeView = view;
	m_treeModel = model;

	view->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(view, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(TreeViewMenu(const QPoint&)));


	QDockWidget* overView = new QDockWidget(QStringLiteral("GPS数据目录"));
	overView->setWidget(view);
	overView->setObjectName(QStringLiteral("GPS数据目录"));
	overView->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, overView);
}

void SimpleGIS::TreeViewMenu(const QPoint& p)
{
	QModelIndex index = m_treeView->currentIndex();
	if (index.isValid())
	{
		m_index = index;
		QMenu* menu = new QMenu(m_treeView);
		menu->addAction(QStringLiteral("调阅"), this, &SimpleGIS::RequestGPS);
		menu->addAction(QStringLiteral("速度可视化"), this, &SimpleGIS::ShowVelocity);
		menu->exec(m_treeView->mapToGlobal(p));
	}
}

void SimpleGIS::RequestGPS()
{
	QString layerProperties = "point?";    // 几何类型
	layerProperties.append(QString("crs=EPSG:4326&"));    // 参照坐标系
	layerProperties.append(QString("field=id:integer&field=velocity:double(10,6)&field=ptime:string(20)&"));    // 添加字段
	layerProperties.append(QString("field=maxVelocity:double(10,6)&field=averageVelocity:double(10,6)&field=x:double(20)&field=y:double(20)&"));    // 添加字段
	layerProperties.append(QString("index=yes&"));        // 创建索引
	layerProperties.append(QString("memoryid=%1").arg(QUuid::createUuid().toString()));

	QgsVectorLayer* newLayer = new QgsVectorLayer(layerProperties, QString("临时点层"), QString("memory"));

	//if (!newLayer->isValid())
	//{
	//	delete newLayer;
	//	return;
	//}

	QgsVectorDataProvider* dateProvider = newLayer->dataProvider();

	// 开始编辑
	newLayer->startEditing();

	// 添加要素
	dateProvider->addFeatures(m_treeModel->GetFeatures(m_index));

	// 保存
	newLayer->commitChanges();

	// 更新范围
	newLayer->updateExtents();

	delete mapCanvas->layers()[0];
	mapCanvas->setLayers(QList<QgsMapLayer*>() << newLayer);

	newLayer->triggerRepaint();
	mapCanvas->zoomToFullExtent();
}

#include <QgsGraduatedSymbolRenderer.h>
#include <QgsApplication.h>
#include <QgsClassificationMethodRegistry.h>
void SimpleGIS::ShowVelocity()
{
	QString layerProperties = "linestring?";    // 几何类型
	layerProperties.append(QString("crs=EPSG:4326&"));    // 参照坐标系
	layerProperties.append(QString("field=id:integer&field=velocity:double(10,4)&field=ptime:string(20)&"));    // 添加字段
	layerProperties.append(QString("field=maxVelocity:double(10,6)&field=averageVelocity:double(10,6)&field=x:double(20)&field=y:double(20)&"));    // 添加字段
	layerProperties.append(QString("index=yes&"));        // 创建索引
	layerProperties.append(QString("memoryid=%1").arg(QUuid::createUuid().toString()));

	QgsVectorLayer* newLayer = new QgsVectorLayer(layerProperties, QString("临时点层"), QString("memory"));

	//if (!newLayer->isValid())
	//{
	//	delete newLayer;
	//	return;
	//}

	QgsVectorDataProvider* dateProvider = newLayer->dataProvider();

	// 开始编辑
	newLayer->startEditing();

	// 添加要素
	dateProvider->addFeatures(m_treeModel->GetLineFeatures(m_index));

	// 保存
	newLayer->commitChanges();

	// 更新范围
	newLayer->updateExtents();

	delete mapCanvas->layers()[0];
	mapCanvas->setLayers(QList<QgsMapLayer*>() << newLayer);

	//分段渲染
	QgsGraduatedSymbolRenderer* symRenderer = new QgsGraduatedSymbolRenderer("velocity");

	QgsStringMap props;
	props["style"] = "solid";
	symRenderer->setSourceSymbol(QgsLineSymbol::createSimple(props));
	symRenderer->setSourceColorRamp(new QgsGradientColorRamp(QColor(255, 0, 0), QColor(0, 255, 0)));
	symRenderer->setClassificationMethod(QgsApplication::classificationMethodRegistry()->method("EqualInterval"));
	symRenderer->updateClasses(newLayer, 5);
	newLayer->setRenderer(symRenderer);

	newLayer->triggerRepaint();
	mapCanvas->zoomToFullExtent();
}

#include <QgsMapToolDigitizeFeature.h>
#include <QgsAdvancedDigitizingDockWidget.h>
void SimpleGIS::FeatureEdit()
{
	QgsAdvancedDigitizingDockWidget* cadDockWidget = new QgsAdvancedDigitizingDockWidget(mapCanvas);
	QgsMapToolDigitizeFeature* tool = new QgsMapToolDigitizeFeature(mapCanvas, cadDockWidget, QgsMapToolCapture::CapturePolygon);

	QgsMapLayer* layer = mapCanvas->layers()[0];
	if (!layer->isEditable())
		((QgsVectorLayer*)layer)->startEditing();
	connect(tool, &QgsMapToolDigitizeFeature::digitizingCompleted, this, &SimpleGIS::AddFeature);
	tool->setLayer(layer);
	mapCanvas->setMapTool(tool);

	addDockWidget(Qt::LeftDockWidgetArea, cadDockWidget);
}

void SimpleGIS::AddFeature(const QgsFeature& f)
{
	QgsVectorLayer* layer = (QgsVectorLayer*)(mapCanvas->layers()[0]);
	layer->beginEditCommand("Add feature");
	layer->addFeature(QgsFeature(f));
	layer->triggerRepaint();
	layer->endEditCommand();
	//layer->commitChanges(false);
}

void SimpleGIS::setSimpleAnnPara()
{
	m_SimpleAnnDialog->show();
}

#include <qgspallabeling.h>
#include <qgsvectorlayerlabeling.h>
void SimpleGIS::SimpleAnn()
{
	QgsPalLayerSettings layerSettings;
	layerSettings.drawLabels = true;
	layerSettings.fieldName = m_SimpleAnnPara.isEmpty() ? "NAME" : m_SimpleAnnPara[0];
	layerSettings.isExpression = false;
	layerSettings.placement = QgsPalLayerSettings::OverPoint;
	layerSettings.yOffset = 2.50;

	QgsTextBufferSettings buffersettings;
	buffersettings.setEnabled(true);
	buffersettings.setSize(1);
	if (m_SimpleAnnPara.isEmpty()) {
		buffersettings.setColor(QColor(100, 100, 0));
	}
	else {
		buffersettings.setColor(QColor(m_SimpleAnnPara[1].toInt(), m_SimpleAnnPara[2].toInt(), m_SimpleAnnPara[3].toInt()));
	}

	QgsTextFormat format;
	QFont font(m_SimpleAnnPara.isEmpty() ? "SimSun" : m_SimpleAnnPara[4], 5, 5, false);
	font.setUnderline(false);
	format.setFont(font);
	format.setBuffer(buffersettings);
	layerSettings.setFormat(format);

	QgsVectorLayerSimpleLabeling* labeling = new QgsVectorLayerSimpleLabeling(layerSettings);
	QgsVectorLayer* layer = (QgsVectorLayer*)(mapCanvas->layers()[0]);
	layer->setLabeling(labeling);
	layer->setLabelsEnabled(true);

	mapCanvas->refresh();
}

#include <qgslabelinggui.h>
void SimpleGIS::AnnSetting()
{
	QgsPalLayerSettings layerSettings;
	layerSettings.drawLabels = true;
	layerSettings.fieldName = "NAME";
	layerSettings.isExpression = false;
	layerSettings.placement = QgsPalLayerSettings::OverPoint;
	layerSettings.yOffset = 2.50;

	QgsTextBufferSettings buffersettings;
	buffersettings.setEnabled(true);
	buffersettings.setSize(1);
	buffersettings.setColor(QColor(100, 100, 0));

	QgsTextFormat format;
	QFont font("SimSun", 5, 5, false);
	font.setUnderline(false);
	format.setFont(font);
	format.setBuffer(buffersettings);
	layerSettings.setFormat(format);

	QgsVectorLayer* layer = (QgsVectorLayer*)(mapCanvas->layers()[0]);

	QgsLabelSettingsDialog dlg(QgsPalLayerSettings(), layer, mapCanvas, this, QgsWkbTypes::PolygonGeometry);
	int ret = dlg.exec();
	if (ret == QDialog::Accepted)
	{
		QgsVectorLayerSimpleLabeling* labeling = new QgsVectorLayerSimpleLabeling(dlg.settings());

		layer->setLabeling(labeling);
		layer->setLabelsEnabled(true);

		mapCanvas->refresh();
	}
}

#include <QgsMarkerSymbolLayer.h>
#include <QgsSingleSymbolRenderer.h>
void SimpleGIS::SingleSymbol()
{
	QgsVectorLayer* vectorLayer = (QgsVectorLayer*)(mapCanvas->layers()[0]);
	// 创建 svgMarkerSymbolLayer
	QgsSvgMarkerSymbolLayer* svgMarker = new QgsSvgMarkerSymbolLayer(":/icons/github.svg");

	QgsSymbolLayerList symList;
	symList.append(svgMarker);

	QgsMarkerSymbol* markSym = new QgsMarkerSymbol(symList);

	QgsSingleSymbolRenderer* symRenderer = new QgsSingleSymbolRenderer(markSym);

	//svgMarker->setSize(10);

	vectorLayer->setRenderer(symRenderer);
	vectorLayer->triggerRepaint();
}

#include <QgsCategorizedSymbolRenderer.h>
void SimpleGIS::CategorizedSymbol()
{
	QgsVectorLayer* vectorLayer = (QgsVectorLayer*)(mapCanvas->layers()[0]);

	//QgsSimpleMarkerSymbolLayer
	QgsStringMap props;
	props["name"] = "star"; props["color"] = "red"; props["size"] = "4.0";
	QgsMarkerSymbol* symbolStar = QgsMarkerSymbol::createSimple(props);
	props["name"] = "circle"; props["color"] = "gray"; props["size"] = "2.0";
	QgsMarkerSymbol* symbolProv = QgsMarkerSymbol::createSimple(props);
	props["name"] = "pentagon"; props["color"] = "blue"; props["size"] = "2.0";
	QgsMarkerSymbol* symbolSpec = QgsMarkerSymbol::createSimple(props);

	QgsCategorizedSymbolRenderer* symRenderer = new QgsCategorizedSymbolRenderer("ADCLASS");
	symRenderer->addCategory(QgsRendererCategory(1, symbolStar, QStringLiteral("首都")));
	symRenderer->addCategory(QgsRendererCategory(2, symbolProv, QStringLiteral("省会")));
	symRenderer->addCategory(QgsRendererCategory(9, symbolSpec, QStringLiteral("特别行政区")));
	vectorLayer->setRenderer(symRenderer);
	vectorLayer->triggerRepaint();
}

void SimpleGIS::setGraduatedSymbolPara()
{
	m_GraduatedSymbolDialog->show();
}

#include <QgsGraduatedSymbolRenderer.h>
#include <QgsColorRamp.h>
#include <QgsClassificationMethodRegistry.h>
void SimpleGIS::GraduatedSymbol()
{
	QgsVectorLayer* vectorLayer = (QgsVectorLayer*)(mapCanvas->layers()[0]);

	QgsGraduatedSymbolRenderer* symRenderer = new QgsGraduatedSymbolRenderer(m_GraduatedSymbolPara.isEmpty() ? "POPU" : m_GraduatedSymbolPara[0]);

	QgsStringMap props;
	props["style"] = "solid";
	symRenderer->setSourceSymbol(QgsFillSymbol::createSimple(props));
	if (m_GraduatedSymbolPara.isEmpty()) {
		symRenderer->setSourceColorRamp(new QgsGradientColorRamp(QColor(255, 255, 255), QColor(255, 0, 0)));
	}
	else {
		symRenderer->setSourceColorRamp(new QgsGradientColorRamp(QColor(255, 255, 255), QColor(m_GraduatedSymbolPara[1].toInt(), m_GraduatedSymbolPara[2].toInt(), m_GraduatedSymbolPara[3].toInt())));
	}
	symRenderer->setClassificationMethod(QgsApplication::classificationMethodRegistry()->method("Pretty"));
	symRenderer->updateClasses(vectorLayer, 5);
	vectorLayer->setRenderer(symRenderer);
	vectorLayer->triggerRepaint();
}

#include <QgsDiagramRenderer.h>
#include <QgsPieDiagram.h>
#include <QgsTextDiagram.h>
#include <QgsHistogramDiagram.h>
#include <QgsStackedBarDiagram.h>
void SimpleGIS::Diagram(QgsDiagram* diagram, bool isSingle)
{
	QgsVectorLayer* vectorLayer = (QgsVectorLayer*)(mapCanvas->layers()[0]);

	QgsDiagramSettings ds;
	QColor col1;
	QColor col2;
	if (m_DiagramPara.isEmpty()) {
		col1 = Qt::green;
		col2 = Qt::red;
	}
	else {
		col1 = QColor(m_DiagramPara[1].toInt(), m_DiagramPara[2].toInt(), m_DiagramPara[3].toInt());
		col2 = QColor(m_DiagramPara[5].toInt(), m_DiagramPara[6].toInt(), m_DiagramPara[7].toInt());
	}
	col1.setAlphaF(0.5);
	col2.setAlphaF(0.5);
	ds.categoryColors = QList<QColor>() << col1 << col2;
	if (m_DiagramPara.isEmpty()) {
		ds.categoryAttributes = QList<QString>() << "Pop_Rural" << "Pop_Urban";
	}
	else {
		ds.categoryAttributes = QList<QString>() << m_DiagramPara[0] << m_DiagramPara[4];
	}
	ds.minimumSize = 0;
	ds.penColor = Qt::gray;
	ds.penWidth = .5;
	ds.scaleByArea = true;
	ds.sizeType = QgsUnitTypes::RenderMillimeters;
	ds.size = QSizeF(5, 5);
	ds.backgroundColor = QColor(255, 255, 255);

	QgsDiagramRenderer* renderer;
	if (isSingle)
	{
		QgsSingleCategoryDiagramRenderer* renderer0 = new QgsSingleCategoryDiagramRenderer();
		renderer0->setDiagramSettings(ds);
		renderer = renderer0;
	}
	else
	{
		QgsLinearlyInterpolatedDiagramRenderer* renderer0 = new QgsLinearlyInterpolatedDiagramRenderer();
		renderer0->setClassificationField("POPU");
		renderer0->setLowerValue(0.0);
		renderer0->setLowerSize(QSizeF(0.0, 0.0));
		renderer0->setUpperValue(10000);
		renderer0->setUpperSize(QSizeF(10, 10));
		renderer0->setDiagramSettings(ds);
		renderer = renderer0;
	}

	renderer->setDiagram(diagram);

	QgsDiagramLayerSettings dls = QgsDiagramLayerSettings();
	dls.setPlacement(QgsDiagramLayerSettings::OverPoint);
	dls.setShowAllDiagrams(true);

	vectorLayer->setDiagramRenderer(renderer);
	vectorLayer->setDiagramLayerSettings(dls);
	vectorLayer->triggerRepaint();
}

void SimpleGIS::setDiagramPara()
{
	m_DiagramDialog->show();
}

void SimpleGIS::PieDiagram()
{
	QgsVectorLayer* vectorLayer = (QgsVectorLayer*)(mapCanvas->layers()[0]);

	QgsDiagramSettings ds;
	QColor col1;
	QColor col2;
	if (m_DiagramPara.isEmpty()) {
		col1 = Qt::green;
		col2 = Qt::red;
	}
	else {
		col1 = QColor(m_DiagramPara[1].toInt(), m_DiagramPara[2].toInt(), m_DiagramPara[3].toInt());
		col2 = QColor(m_DiagramPara[5].toInt(), m_DiagramPara[6].toInt(), m_DiagramPara[7].toInt());
	}
	col1.setAlphaF(0.5);
	col2.setAlphaF(0.5);
	ds.categoryColors = QList<QColor>() << col1 << col2;
	if (m_DiagramPara.isEmpty()) {
		ds.categoryAttributes = QList<QString>() << "Pop_Rural" << "Pop_Urban";
	}
	else {
		ds.categoryAttributes = QList<QString>() << m_DiagramPara[0] << m_DiagramPara[4];
	}
	ds.minimumSize = 0;
	ds.penColor = Qt::gray;
	ds.penWidth = .5;
	ds.scaleByArea = true;
	ds.sizeType = QgsUnitTypes::RenderMillimeters;
	ds.size = QSizeF(5, 5);
	ds.backgroundColor = QColor(255, 255, 255);

	QgsSingleCategoryDiagramRenderer* renderer = new QgsSingleCategoryDiagramRenderer();

	/*QgsLinearlyInterpolatedDiagramRenderer *renderer = new QgsLinearlyInterpolatedDiagramRenderer();
	renderer->setClassificationField("POPU");
	renderer->setLowerValue(0.0);
	renderer->setLowerSize(QSizeF(0.0, 0.0));
	renderer->setUpperValue(10000);
	renderer->setUpperSize(QSizeF(10, 10));*/
	renderer->setDiagram(new QgsPieDiagram());
	//renderer->setDiagram(new QgsTextDiagram());
	//renderer->setDiagram(new QgsHistogramDiagram());
	//renderer->setDiagram(new QgsStackedBarDiagram());
	renderer->setDiagramSettings(ds);

	QgsDiagramLayerSettings dls = QgsDiagramLayerSettings();
	dls.setPlacement(QgsDiagramLayerSettings::OverPoint);
	dls.setShowAllDiagrams(true);

	vectorLayer->setDiagramRenderer(renderer);
	vectorLayer->setDiagramLayerSettings(dls);
	vectorLayer->triggerRepaint();
}

void SimpleGIS::TextDiagram()
{
	Diagram(new QgsTextDiagram(), true);
}

void SimpleGIS::HistogramDiagram()
{
	Diagram(new QgsHistogramDiagram(), true);
}

void SimpleGIS::StackBarDiagram()
{
	Diagram(new QgsStackedBarDiagram(), true);
}

void SimpleGIS::PieDiagram2()
{
	Diagram(new QgsPieDiagram(), false);
}

void SimpleGIS::TextDiagram2()
{
	Diagram(new QgsTextDiagram(), false);
}

void SimpleGIS::HistogramDiagram2()
{
	Diagram(new QgsHistogramDiagram(), false);
}

void SimpleGIS::StackBarDiagram2()
{
	Diagram(new QgsStackedBarDiagram(), false);
}

#include <QDesktopServices.h>
void SimpleGIS::Help()
{
	QDesktopServices::openUrl(QUrl("https://www.osgeo.cn/qgisdoc/docs/#"));
}

void SimpleGIS::receiveData(QVector<QString> data) {
	this->m_SimpleAnnPara.clear();
	this->m_SimpleAnnPara.push_back(data[0]);
	this->m_SimpleAnnPara.push_back(data[1]);
	this->m_SimpleAnnPara.push_back(data[2]);
	this->m_SimpleAnnPara.push_back(data[3]);
	this->m_SimpleAnnPara.push_back(data[4]);
}

void SimpleGIS::receiveData2(QVector<QString> data)
{
	this->m_GraduatedSymbolPara.clear();
	this->m_GraduatedSymbolPara.push_back(data[0]);
	this->m_GraduatedSymbolPara.push_back(data[1]);
	this->m_GraduatedSymbolPara.push_back(data[2]);
	this->m_GraduatedSymbolPara.push_back(data[3]);
}

void SimpleGIS::receiveData3(QVector<QString> data)
{
	this->m_DiagramPara.clear();
	this->m_DiagramPara.push_back(data[0]);
	this->m_DiagramPara.push_back(data[1]);
	this->m_DiagramPara.push_back(data[2]);
	this->m_DiagramPara.push_back(data[3]);
	this->m_DiagramPara.push_back(data[4]);
	this->m_DiagramPara.push_back(data[5]);
	this->m_DiagramPara.push_back(data[6]);
	this->m_DiagramPara.push_back(data[7]);
}
