#include "testbenchtoolbox.h"

TestBenchToolBox::TestBenchToolBox(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::TestBenchToolBox)
{
	ui->setupUi(this);
	setMaximumHeight(ui->dockWidgetContents->height()+20);
}