#include "genericlayouteditorwidget.h"
#include "tech_data_wrapper.hpp"

GenericLayoutEditorWidget::GenericLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent)
{
}

void GenericLayoutEditorWidget::onContentChanged()
{
	setStatusChanged(true);
	emit(contentChanged());
}

void GenericLayoutEditorWidget::loadFile(QString path)
{
	((GenericLayoutEditor*)centralWidget())->loadFile(path);
}

void GenericLayoutEditorWidget::loadLEF(QString path)
{
	((GenericLayoutEditor*)centralWidget())->loadLEF(path);
}

void GenericLayoutEditorWidget::loadGDS(QString path)
{
	((GenericLayoutEditor*)centralWidget())->loadGDS(path);
}

void GenericLayoutEditorWidget::setTechnologyData(TechDataWrapper* toml)
{
	if(toml) {
		if(m_layoutVisibles) {
			m_layoutVisibles->setTechData(toml);
		}
		if(m_activeLayer) {
			m_activeLayer->addItem("all layers");
			foreach(QString layern, toml->getLayers()) {
				m_activeLayer->addItem(toml->getLayerIcon(layern),layern);
			}
		}
		if(m_lambdaInfo) {
			m_lambdaInfo->setText(QString(QChar(0xbb, 0x03))+"="+QString::number(toml->getLambdaValue())+toml->getLambdaUnit());
		}
		if(centralWidget()) {
			((GenericLayoutEditor*)centralWidget())->setTechData(toml);
		}
	}
}

void GenericLayoutEditorWidget::setLambdaUnit(QString s)
{
	if(centralWidget()) {
		((GenericLayoutEditor*)centralWidget())->setLambdaUnit(s);	
	}
}

void GenericLayoutEditorWidget::setLambdaValue(qreal i)
{
	if(centralWidget()) {
		((GenericLayoutEditor*)centralWidget())->setLambdaValue(i);
	}	
}

void GenericLayoutEditorWidget::setUpCentralWidget(GenericLayoutEditor *area)
{
	setCentralWidget(area);
	QAction *button;

	button = new QAction(QPixmap(":/icon_3d.svg"), "3D view", m_toolbar);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(show3D()));
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/zoom_in.svg"), "Zoom in", m_toolbar);
	connect(button, SIGNAL(triggered(bool)), ((GenericLayoutEditor*)centralWidget()), SLOT(zoomIn()));
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/zoom_out.svg"), "Zoom out", m_toolbar);
	connect(button, SIGNAL(triggered(bool)), ((GenericLayoutEditor*)centralWidget()), SLOT(zoomOut()));
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/drc.svg"), "DRC settings", m_toolbar);
	connect(button, SIGNAL(triggered(bool)), ((GenericLayoutEditor*)centralWidget()), SLOT(showDRC()));
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/array_module.svg"), "Cell manager", m_toolbar);
	connect(button, SIGNAL(triggered(bool)), ((GenericLayoutEditor*)centralWidget()), SLOT(showCellManager()));
	m_toolbar->addAction(button);

	m_layoutVisibles = new LayoutVisibles(this);
	m_layoutVisibles->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, m_layoutVisibles);
	connect(((GenericLayoutEditor*)centralWidget()),SIGNAL(registerLayer(QString)),m_layoutVisibles,SLOT(onRegisterLayer(QString)));

	connect(((GenericLayoutEditor*)centralWidget()), SIGNAL(contentChanged()), this, SLOT(onContentChanged()));

	button = new QAction(QPixmap(":/add_rectangle.svg"), "Add rectangle", m_toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_RECTANGLE);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	m_drawingOperations[DRAWING_OPERATION_RECTANGLE] = button;
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/add_dashed_line.svg"), "Select items", m_toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_SELECT);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	m_drawingOperations[DRAWING_OPERATION_SELECT] = button;
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/drag.svg"), "Drag rectangles", m_toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_DRAG);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	m_drawingOperations[DRAWING_OPERATION_DRAG] = button;
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/add_zone_cutout.svg"), "Add cutout zone", m_toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_CUT_OUT);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	m_drawingOperations[DRAWING_OPERATION_CUT_OUT] = button;
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/show_all_layers.svg"), "Select layers", m_toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_SELECT_LAYER);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	m_drawingOperations[DRAWING_OPERATION_SELECT_LAYER] = button;
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/add_dimension.svg"), "Add dimension", m_toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_DIMENSION);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	m_drawingOperations[DRAWING_OPERATION_DIMENSION] = button;
	m_toolbar->addAction(button);

	button = new QAction(QPixmap(":/label.svg"), "Add label", m_toolbar);
	button->setCheckable(true);
	button->setData(DRAWING_OPERATION_LABEL);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	m_drawingOperations[DRAWING_OPERATION_LABEL] = button;
	m_toolbar->addAction(button);

	m_activeLayer = new QComboBox(m_toolbar);

	connect(m_activeLayer,SIGNAL(currentTextChanged(QString)),((GenericLayoutEditor*)centralWidget()),SLOT(setActiveLayer(QString)));
	connect(m_layoutVisibles,SIGNAL(setCurrentLayer(QString)),m_activeLayer,SLOT(setCurrentText(QString)));
	connect(m_layoutVisibles,SIGNAL(setCurrentLayer(QString)),((GenericLayoutEditor*)centralWidget()),SLOT(setActiveLayer(QString)));
	connect(m_layoutVisibles,SIGNAL(enabledTypesChanged(QStringList)),((GenericLayoutEditor*)centralWidget()),SLOT(onVisibleLayersChanged(QStringList)));
	m_toolbar->addWidget(m_activeLayer);

	// lambda info
	m_lambdaInfo = new QLabel("none");
	m_toolbar->addWidget(m_lambdaInfo);
}

void GenericLayoutEditorWidget::drawingOperation()
{
	QObject* obj = sender();
	QAction *action = qobject_cast<QAction *>(obj);
	drawing_operations o = (drawing_operations)action->data().toInt();
	((GenericLayoutEditor*)centralWidget())->setDrawingOperation(DRAWING_OPERATION_NONE);

	foreach(drawing_operations k, m_drawingOperations.keys()) {
		if(k==o) continue;
		m_drawingOperations[k]->setChecked(false);
	}

	if(m_drawingOperations.contains(o)) {
		if(m_drawingOperations[o]->isChecked()) {
			qDebug() << __FUNCTION__ << " setting drawing operation " << o;
			((GenericLayoutEditor*)centralWidget())->setDrawingOperation(o);
		}
	}
}

QAction* GenericLayoutEditorWidget::addToolbarAction(QPixmap pm, QString str)
{
	QAction *button;
	button = new QAction(pm, str, m_toolbar);
	m_toolbar->addAction(button);
	return button;
}

QString GenericLayoutEditorWidget::getFilePath()
{
	return ((GenericLayoutEditor*)centralWidget())->getFilePath();
}

void GenericLayoutEditorWidget::saveFile()
{
	((GenericLayoutEditor*)centralWidget())->saveFile();
	setStatusChanged(false);
	emit(contentSaved());
}
