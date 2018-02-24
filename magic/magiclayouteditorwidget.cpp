#include "magiclayouteditorwidget.h"

MagicLayoutEditorWidget::MagicLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new MagicLayoutEditor(this)),
	view3D(new Magic3D(this))
{
	QAction *button;
	QToolBar *toolbar = new QToolBar(this);

	setType(MagicLayoutEditorWidgetType);

	button = new QAction(QPixmap(":/icon_3d.svg"), "3D view", toolbar);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(show3D()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/zoom_in.svg"), "Zoom in", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(zoomIn()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/zoom_out.svg"), "Zoom out", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(zoomOut()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/drc.svg"), "DRC settings", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(showDRC()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/array_module.svg"), "Cell manager", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(showCellManager()));
	toolbar->addAction(button);

	addToolBar(toolbar);

	setCentralWidget(editArea);

	layoutVisibles = new LayoutVisibles(this);
	layoutVisibles->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, layoutVisibles);
	editArea->setVisibles(layoutVisibles);

	connect(editArea, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));

	addDrawingOperations();
}

void MagicLayoutEditorWidget::onContentChanged()
{
	setStatusChanged(true);
	emit(contentChanged());
}

void MagicLayoutEditorWidget::addDrawingLayerSelection()
{
	QToolBar *toolbar;

	toolbar = new QToolBar(this);
	activeLayer = new QComboBox(toolbar);

	/*foreach(QString layern, project->getPlanes()) {
		foreach(QString vname, project->getTypes(layern)) {
			activeLayer->addItem(project->materialIcon(vname),vname);
		}
	}*/

	connect(activeLayer,SIGNAL(currentTextChanged(QString)),editArea,SLOT(setActiveLayer(QString)));
	connect(layoutVisibles,SIGNAL(setCurrentLayer(QString)),activeLayer,SLOT(setCurrentText(QString)));
	connect(layoutVisibles,SIGNAL(setCurrentLayer(QString)),editArea,SLOT(setActiveLayer(QString)));

	toolbar->addWidget(activeLayer);
	addToolBar(toolbar);
}

void MagicLayoutEditorWidget::addDrawingOperations()
{
	QAction *button;
	QToolBar *toolbar;

	toolbar = new QToolBar(this);

	button = new QAction(QPixmap(":/add_rectangle.svg"), "Add rectangle", toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_RECTANGLE);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	drawingOperations[DRAWING_OPERATION_RECTANGLE] = button;
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/add_dashed_line.svg"), "Select items", toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_SELECT);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	drawingOperations[DRAWING_OPERATION_SELECT] = button;
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/drag.svg"), "Drag rectangles", toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_DRAG);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	drawingOperations[DRAWING_OPERATION_DRAG] = button;
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/add_zone_cutout.svg"), "Add cutout zone", toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_CUT_OUT);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	drawingOperations[DRAWING_OPERATION_CUT_OUT] = button;
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/show_all_layers.svg"), "Select layers", toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_SELECT_LAYER);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	drawingOperations[DRAWING_OPERATION_SELECT_LAYER] = button;
	toolbar->addAction(button);

	addToolBar(Qt::RightToolBarArea,toolbar);
}

void MagicLayoutEditorWidget::drawingOperation()
{
	QObject* obj = sender();
	QAction *action = qobject_cast<QAction *>(obj);
	drawing_operations o = (drawing_operations)action->data().toInt();
	editArea->setDrawingOperation(DRAWING_OPERATION_NONE);

	foreach(drawing_operations k, drawingOperations.keys()) {
		if(k!=o) drawingOperations[k]->setChecked(false);
	}

	if(drawingOperations.contains(o)) {
		if(drawingOperations[o]->isChecked())
			editArea->setDrawingOperation(o);
	}
}

void MagicLayoutEditorWidget::loadFile(QString path)
{
	editArea->setVisibles(layoutVisibles);
	editArea->loadFile(path);
	view3D->loadFile(path);
}

void MagicLayoutEditorWidget::show3D()
{
	view3D->show();
}

QString MagicLayoutEditorWidget::getFilePath()
{
	return editArea->getFilePath();
}

void MagicLayoutEditorWidget::saveFile()
{
	editArea->saveFile();
	setStatusChanged(false);
	emit(contentSaved());
	view3D->loadFile(editArea->getFilePath());
}
