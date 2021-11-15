#pragma once
#include <qgsmaptool.h>
#include <QgsGeometry.h>
#include <QgsRubberBand.h>

class MyMapToolSelect :
	public QgsMapTool
{
	Q_OBJECT

public:
	MyMapToolSelect(QgsMapCanvas *canvas);
	~MyMapToolSelect();

	//! Select features to identify by:
	enum SelectionMode
	{
		//! SelectSimple - single click or drawing a rectangle, default option
		SelectSimple,
		//! SelectPolygon - drawing a polygon or right-click on existing polygon feature
		SelectPolygon,
		//! SelectFreehand - free hand selection
		SelectFreehand,
		//! SelectRadius - a circle selection
		SelectRadius
	};

	void setSelectionMode(SelectionMode mode);

	//! Returns most recently selected geometry (may be a point or a polygon)
	QgsGeometry selectedGeometry() const;

	//! Sets the current selection mode
	SelectionMode selectionMode() const;
	//! Returns the current selection mode
	void setSelectionMode(SelectionMode mode);

	//! Deactivates handler (when map tool gets deactivated)
	void deactivate();

	//! Handles mouse move event from map tool
	void canvasMoveEvent(QgsMapMouseEvent *e);
	//! Handles mouse press event from map tool
	void canvasPressEvent(QgsMapMouseEvent *e);
	//! Handles mouse release event from map tool
	void canvasReleaseEvent(QgsMapMouseEvent *e);
	//! Handles escape press event - returns true if the even has been processed
	void keyReleaseEvent(QKeyEvent *e);

	void setSelectedGeometry(const QgsGeometry &geometry, Qt::KeyboardModifiers modifiers = Qt::NoModifier);

signals:
	//! emitted when a new geometry has been picked (selectedGeometry())
	void geometryChanged(Qt::KeyboardModifiers modifiers = Qt::NoModifier);

	private slots:
	//! update the rubber band from the input widget
	void updateRadiusRubberband(double radius);

	/**
	* triggered when the user input widget has a new value
	* either programmatically (from the mouse event) or entered by the user
	*/
	void radiusValueEntered(double radius, Qt::KeyboardModifiers modifiers);

	//! cancel selecting (between two click events)
	void cancel();

private:

	void selectFeaturesMoveEvent(QgsMapMouseEvent *e);
	void selectFeaturesReleaseEvent(QgsMapMouseEvent *e);
	void selectFeaturesPressEvent(QgsMapMouseEvent *e);

	void selectPolygonMoveEvent(QgsMapMouseEvent *e);
	void selectPolygonPressEvent(QgsMapMouseEvent *e);

	void selectFreehandMoveEvent(QgsMapMouseEvent *e);
	void selectFreehandReleaseEvent(QgsMapMouseEvent *e);

	void selectRadiusMoveEvent(QgsMapMouseEvent *e);
	void selectRadiusReleaseEvent(QgsMapMouseEvent *e);

	void initRubberBand();

	QgsPointXY toMapCoordinates(QPoint point);

	void createDistanceWidget();
	void deleteDistanceWidget();

	void updateRadiusFromEdge(QgsPointXY &radiusEdge);

private:

	QgsMapCanvas *mCanvas = nullptr;

	//! the rubberband for selection visualization
	std::unique_ptr<QgsRubberBand> mSelectionRubberBand;

	//! Stores the most recent geometry (in map coordinates)
	QgsGeometry mSelectionGeometry;

	//! Whether a geometry is being picked right now
	bool mSelectionActive = false;

	SelectionMode mSelectionMode = SelectSimple;

	//! Initial point (in screen coordinates) when using simple selection
	QPoint mInitDragPos;

	//! Center point for the radius when using radius selection
	QgsPointXY mRadiusCenter;

	QColor mFillColor = QColor(254, 178, 76, 63);
	QColor mStrokeColor = QColor(254, 58, 29, 100);
};

