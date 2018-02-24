#ifndef MAGICLAYOUTEDITOR_H
#define MAGICLAYOUTEDITOR_H

#include "magic/magicdata.h"

#include "qlayout/layoutvisibles.h"
#include "qlayout/qlayoutscene.h"

#include <QGridLayout>
#include <QAbstractScrollArea>
#include <QFileInfo>
#include <QResource>
#include <QDebug>
#include <QTemporaryDir>
#include <QAction>
#include <QComboBox>
#include <QGraphicsView>

namespace magic {
	class MagicData;
}

class MagicLayoutEditor : public QGraphicsView
{
	Q_OBJECT
public:
	explicit MagicLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void setVisibles(LayoutVisibles *v);
	void saveFile();

	QString getFilePath();

	void setDrawingOperation(drawing_operations o);

	bool changes();

signals:
	void contentSaved();

public slots:
	void setActiveLayer(QString);

	void zoomIn();
	void zoomOut();
	void showDRC();
	void showCellManager();

private:
	void setRecentVisible(QString s);

	/* magic output functions */
	void saveFileWriteHeader(QTextStream &outputStream);
	void saveFileWriteRects(QTextStream &outputStream);
	void saveFileWriteMacros(QTextStream &outputStream);
	/* end of magic output functions */

	void addRectangles();
	void addMacroInstances();

	QString filePath;
	QLayoutScene *editScene;
	magic::MagicData *magicdata;

	QRectF sceneRect;
	LayoutVisibles *visibles;
	QComboBox *activeLayerSelection;

	qreal m_scale;
};

#endif // MAGICLAYOUTEDITOR_H
