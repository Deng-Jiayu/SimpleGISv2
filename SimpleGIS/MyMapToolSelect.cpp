#include "MyMapToolSelect.h"



MyMapToolSelect::MyMapToolSelect(QgsMapCanvas *canvas) : QgsMapTool(canvas)
{
	mToolName = QStringLiteral("ø’º‰≤È—Ø");
}


MyMapToolSelect::~MyMapToolSelect()
{
}
#include <QgsApplication.h>

void MyMapToolSelect::setSelectionMode(SelectionMode mode)
{
	if (mode == SelectSimple)
		mCursor = QgsApplication::getThemeCursor(QgsApplication::Cursor::Select);
	else
		mCursor = Qt::ArrowCursor;

	mSelectionMode = mode;
}
