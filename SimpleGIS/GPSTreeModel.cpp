#include "GPSTreeModel.h"
#include "treeitem.h"


GPSTreeModel::GPSTreeModel(const QString& fname, QObject* parent) :QAbstractItemModel(parent)
{
	setupModelData(fname);
}


GPSTreeModel::~GPSTreeModel()
{
	delete rootItem;
}

//! [1]

//! [2]
int GPSTreeModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	return rootItem->columnCount();
}
//! [2]

//! [3]
QVariant GPSTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column());
}
//! [3]

//! [4]
Qt::ItemFlags GPSTreeModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant GPSTreeModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}
//! [5]

//! [6]
QModelIndex GPSTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem* parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}
//! [6]

//! [7]
QModelIndex GPSTreeModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem* parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int GPSTreeModel::rowCount(const QModelIndex& parent) const
{
	TreeItem* parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}
//! [8]
//#include <fstream>
//#include <map>
//using namespace std;
#include <QFile>
bool GPSPointLessThan(const GPSPoint& s1, const GPSPoint& s2)
{
	if (s1.carId != s2.carId)
		return s1.carId < s2.carId;
	if (s1.h != s2.h)
		return s1.h < s2.h;
	if (s1.m != s2.m)
		return s1.m < s2.m;
	return s1.s < s2.s;
}

void CountSubItems(TreeItem* root, long maxi)
{
	if (root->childCount() == 0)return;

	for (int i = 0; i < root->childCount() - 1; i++)
	{
		root->child(i)->appendColumn(root->child(i + 1)->data(1).toInt() - root->child(i)->data(1).toInt());
		CountSubItems(root->child(i), root->child(i + 1)->data(1).toInt());
	}
	root->child(root->childCount() - 1)->appendColumn(maxi - root->child(root->childCount() - 1)->data(1).toInt());
	CountSubItems(root->child(root->childCount() - 1), maxi);
}

int deltaTime(GPSPoint& p1, GPSPoint& p2)
{
	return p2.h * 3600 + p2.m * 60 + p2.s - (p1.h * 3600 + p1.m * 60 + p1.s);
}

QVector<velocity> mVelocity;

void GPSTreeModel::setupModelData(const QString& fname)
{
	rootItem = new TreeItem({ QStringLiteral("分类"),QStringLiteral("开始位置"),QStringLiteral("平均速度"),QStringLiteral("最高速度"), QStringLiteral("要素数") });

	GPSPoint p;
	QFile file(fname);
	file.open(QIODevice::ReadOnly);
	while (!file.atEnd())
	{
		QString line = file.readLine();
		const QStringList columnStrings =
			line.split(QLatin1Char(','), QString::SkipEmptyParts);
		p.carId = columnStrings[0].toLong();
		p.y = columnStrings[1].toFloat();
		p.x = columnStrings[2].toFloat();
		p.flag = columnStrings[3].toInt();
		const QStringList hms =
			columnStrings[4].split(QLatin1Char(':'), QString::SkipEmptyParts);
		p.h = hms[0].right(2).toInt();
		p.m = hms[1].toInt();
		p.s = hms[2].toInt();
		mData.push_back(p);
	}
	file.close();

	qSort(mData.begin(), mData.end(), GPSPointLessThan);

	long  carId;
	// 每一个点的总距离，时间
	double totalDistance = 0;
	int totalTime = 0;
	double maxV = 0;
	velocity v;

	for (QVector<GPSPoint>::iterator it = mData.begin(); it != mData.end(); it++)
	{


		QgsPointXY p(it->x, it->y);
		carId = it->carId;
		if (it->carId == (it + 1)->carId && (it + 1) != mData.end() - 1) {
			QgsPointXY c((it + 1)->x, (it + 1)->y);
			int dt = deltaTime(*it, *(it + 1));

			if (dt != 0) {
				double v = c.distance(p) / dt * 111000;
				if (v > maxV) {
					maxV = v;								// 更新最大速度
				}
				totalDistance += c.distance(p);				// 总路程
				totalTime += dt;							// 总时间
			}

		}
		else											// 遇见新点
		{
			double averageV = totalDistance / totalTime * 111000;
			v.carId = carId;
			v.maxV = maxV;
			v.averageV = averageV;
			mVelocity.push_back(v);							// 将速度加入到数组中

			// 新点总距离，时间重置
			totalDistance = 0;
			totalTime = 0;
			maxV = 0;
		}
	}

	QVector<velocity>::Iterator vIt = mVelocity.begin();
	for (QVector<GPSPoint>::iterator it = mData.begin(); it != mData.end(), vIt != mVelocity.end(); )
	{
		while (vIt->carId == it->carId) {
			it->maxV = vIt->maxV;
			it->averageV = vIt->averageV;
			it++;
		}
		vIt++;
	}


	TreeItem* car = NULL, * Hour = NULL, * Minute = NULL;
	long   HourId, MinuteId, i = 0;
	// 遍历每一个点
	for (QVector<GPSPoint>::iterator it = mData.begin(); it != mData.end(); it++, i++)
	{
		QVector<QVariant> columnData;
		if (car == NULL || carId != it->carId)
		{
			columnData << it->carId << i << (it)->averageV << (it)->maxV; carId = it->carId;
			car = new TreeItem(columnData, rootItem);
			rootItem->appendChild(car);

			columnData.clear();
			HourId = long(it->h);
			columnData << HourId << i << (it)->averageV << (it)->maxV;
			Hour = new TreeItem(columnData, car);
			car->appendChild(Hour);

			columnData.clear();
			MinuteId = long(it->m / 5 * 5);
			columnData << MinuteId << i << (it)->averageV << (it)->maxV;
			Minute = new TreeItem(columnData, Hour);
			Hour->appendChild(Minute);
		}
		else if (HourId != long(it->h))
		{
			HourId = long(it->h);
			columnData << HourId << i << it->averageV << it->maxV;
			Hour = new TreeItem(columnData, car);
			car->appendChild(Hour);

			columnData.clear();
			MinuteId = long(it->m / 5 * 5);
			columnData << MinuteId << i << it->averageV << it->maxV;
			Minute = new TreeItem(columnData, Hour);
			Hour->appendChild(Minute);
		}
		else if (MinuteId != long(it->m / 5 * 5))
		{
			MinuteId = long(it->m / 5 * 5);
			columnData << MinuteId << i << it->averageV << it->maxV;
			Minute = new TreeItem(columnData, Hour);
			Hour->appendChild(Minute);
		}
	}

	CountSubItems(rootItem, mData.size());
}

#include <QMessageBox>
QgsFeatureList GPSTreeModel::GetFeatures(QModelIndex& index)
{
	QgsFeatureList ret;

	TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
	int from = childItem->data(1).toInt();
	int num = childItem->data(4).toInt();

	// 创建点
	QgsFeature MyFeature;
	MyFeature.initAttributes(7);

	QgsPointXY p(mData[from].x, mData[from].y);
	for (int i = 1; i < num; i++)
	{
		// 添加点
		int idx = i + from;
		QgsPointXY c(mData[idx].x, mData[idx].y);
		int dt = deltaTime(mData[idx - 1], mData[idx]);
		if (dt == 0) continue;
		MyFeature.setGeometry(QgsGeometry::fromPointXY(c));
		MyFeature.setAttribute(0, QVariant(i));
		// 瞬时速度
		double v = c.distance(p) / dt * 111000;		// 将经纬度计算所得速度转换为m/s
		MyFeature.setAttribute(1, QVariant(v));
		MyFeature.setAttribute(2, QVariant(QString("%1:%2:%3").arg(mData[idx].h, 2, 10, QLatin1Char('0')).arg(mData[idx].m, 2, 10, QLatin1Char('0')).arg(mData[idx].s, 2, 10, QLatin1Char('0'))));
		MyFeature.setAttribute(3, QVariant(mData[idx].maxV));
		MyFeature.setAttribute(4, QVariant(mData[idx].averageV));
		MyFeature.setAttribute(5, QVariant(mData[idx].x));
		MyFeature.setAttribute(6, QVariant(mData[idx].y));
		p = c;
		ret << MyFeature;
	}
	return ret;
}

QgsFeatureList GPSTreeModel::GetLineFeatures(QModelIndex& index)
{
	QgsFeatureList ret;

	TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
	int from = childItem->data(1).toInt();
	int num = childItem->data(4).toInt();

	// 创建点
	QgsFeature MyFeature;
	MyFeature.initAttributes(7);
	QgsPointXY p(mData[from].x, mData[from].y);
	for (int i = 1; i < num; i++)
	{
		// 添加点
		int idx = i + from;

		QgsPointXY c(mData[idx].x, mData[idx].y);
		int dt = deltaTime(mData[idx - 1], mData[idx]);
		if (dt == 0) continue;
		MyFeature.setGeometry(QgsGeometry::fromPolylineXY(QgsPolylineXY() << p << c));
		MyFeature.setAttribute(0, QVariant(i));
		double v = c.distance(p) / dt * 111000;
		MyFeature.setAttribute(1, QVariant(v));
		MyFeature.setAttribute(2, QVariant(QString("%1:%2:%3").arg(mData[idx].h, 2, 10, QLatin1Char('0')).arg(mData[idx].m, 2, 10, QLatin1Char('0')).arg(mData[idx].s, 2, 10, QLatin1Char('0'))));
		MyFeature.setAttribute(3, QVariant(mData[idx].maxV));
		MyFeature.setAttribute(4, QVariant(mData[idx].averageV));
		MyFeature.setAttribute(5, QVariant(mData[idx].x));
		MyFeature.setAttribute(6, QVariant(mData[idx].y));
		p = c;
		ret << MyFeature;
	}
	return ret;
}
