#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

#include <qabstractitemmodel.h>
#include <QgsFeature.h>
struct GPSPoint
{
	long carId;
	float x, y;
	unsigned char flag;
	unsigned char h, m, s;
	double maxV = 0;
	double averageV = 0;
};

struct velocity {
	long carId;
	double maxV = 0;
	double averageV = 0;
};

class TreeItem;
class GPSTreeModel :
	public QAbstractItemModel
{
public:
	GPSTreeModel(const QString& fname, QObject* parent = nullptr);
	~GPSTreeModel();

	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
		const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	//把index节点为根节点的子树转成要素列表
	QgsFeatureList GetFeatures(QModelIndex& index);
	QgsFeatureList GetLineFeatures(QModelIndex& index);

private:
	void setupModelData(const QString& fname);
	//void InsertByTime(TreeItem *parent,const QStringList &columnStrings);

	TreeItem* rootItem;
	QVector<GPSPoint>	mData;
};
