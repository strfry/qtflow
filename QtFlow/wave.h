#ifndef WAVE_H
#define WAVE_H

#include <QDockWidget>
#include <QDebug>
#include <QFile>
#include <QTreeView>
#include <QListView>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMimeData>

#include "qtreeviewdragdrop.h"

#include "ui_wave.h"

#include "vcd/vcdtreemodel.h"
#include "vcd/vcdsignaltreemodel.h"
#include "vcd/vcdsignalviewtreemodel.h"
#include "vcd/vcdsignalview.h"

//#include "vcd_parser/location.hh"
#include "vcd_reader/vcd_loader.hpp"

namespace Ui {
class Wave;
}

class Wave : public QDockWidget
{
	Q_OBJECT

public:
	explicit Wave(QWidget *parent = 0);
	~Wave();

	void loadVcd(QString);

public slots:
	void onSelectScope(QModelIndex);
	void onSelectSignal(QModelIndex);

private:
	Ui::Wave *ui;
	VcdTreeModel *tree;
	VcdSignalTreeModel *signalTree;
	VcdSignalView *signalView;
	QTreeViewDragDrop *treeSelectionView;

	vcd::VcdData vcd_data;
};

#endif // WAVE_H
