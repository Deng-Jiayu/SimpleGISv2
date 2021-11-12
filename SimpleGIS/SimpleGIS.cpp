#include "SimpleGIS.h"

#include <QFileDialog>
#include <QgsVectorLayer.h>
#include <QgsMaptoolZoom.h>

SimpleGIS::SimpleGIS(QWidget* parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

	mapCanvas = new QgsMapCanvas;
	setCentralWidget(mapCanvas);

	QMenu* m = new QMenu(QStringLiteral("基础功能"));
	QAction* addVector = m->addAction(QStringLiteral("添加矢量数据"), this, &SimpleGIS::addVectorLayer);
	addVector->setIcon(QIcon(QStringLiteral(":/icons/文件.svg")));
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

	QAction* createDoc = m->addAction(QStringLiteral("保存文档"), this, &SimpleGIS::CreateDoc);
	createDoc->setIcon(QIcon(QStringLiteral(":/icons/保存.svg")));
	QAction* openDoc = m->addAction(QStringLiteral("打开文档"), this, &SimpleGIS::OpenDoc);
	openDoc->setIcon(QIcon(QStringLiteral(":/icons/文件夹.svg")));
	QAction* layerManage = m->addAction(QStringLiteral("图层管理"), this, &SimpleGIS::LayerManage);
	layerManage->setIcon(QIcon(QStringLiteral(":/icons/图层管理.svg")));


	ui.menuBar->addMenu(m);

	ui.mainToolBar->addAction(openDoc);
	ui.mainToolBar->addAction(createDoc);
	ui.mainToolBar->addAction(addVector);
	ui.mainToolBar->addAction(zoomIn);
	ui.mainToolBar->addAction(zoomOut);
	ui.mainToolBar->addAction(pan);
	ui.mainToolBar->addAction(fullExtent);
	ui.mainToolBar->addAction(identify);
	ui.mainToolBar->addAction(zoomToFeature);
	ui.mainToolBar->addAction(openOverView);
	ui.mainToolBar->addAction(layerManage);


	m = new QMenu(QStringLiteral("空间数据管理"));

	QAction* catalog = m->addAction(QStringLiteral("数据目录"), this, &SimpleGIS::Catalog);
	catalog->setIcon(QIcon(QStringLiteral(":/icons/目录.svg")));
	m->addAction(QStringLiteral("图层属性"), this, &SimpleGIS::AttShow);
	m->addAction(QStringLiteral("图层属性QGIS"), this, &SimpleGIS::AttShowQGIS);
	QAction* attShowMy = m->addAction(QStringLiteral("图层属性My"), this, &SimpleGIS::AttShowMy);
	attShowMy->setIcon(QIcon(QStringLiteral(":/icons/属性.svg")));

	ui.menuBar->addMenu(m);

	ui.mainToolBar->addAction(catalog);
	ui.mainToolBar->addAction(attShowMy);


	m = new QMenu(QStringLiteral("GPS"));
	m->addAction(QStringLiteral("加载数据"), this, &SimpleGIS::LoadGPSData);
	m->addAction(QStringLiteral("GPS数据目录"), this, &SimpleGIS::GPSCatalog);

	ui.menuBar->addMenu(m);

	m = new QMenu(QStringLiteral("帮助"));
	m->addAction(QStringLiteral("查看帮助"), this, &SimpleGIS::Help);

	ui.menuBar->addMenu(m);

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
	}
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
	//mapProject = new QgsProject;
	mapProject = QgsProject::instance();
	//mapProject->addMapLayer(mapCanvas->currentLayer());
	mapProject->clear();

	mapProject->addMapLayers(mapCanvas->layers());
	mapProject->write("test.qgs");

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
	QgsLayerTreeMapCanvasBridge* bridge = new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), mapCanvas);

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

#include <QDesktopServices.h>
void SimpleGIS::Help()
{
	QDesktopServices::openUrl(QUrl("https://www.osgeo.cn/qgisdoc/docs/#"));
}
