#ifndef GDTDATA_H
#define GDTDATA_H

#include <QString>
#include <QDebug>

#include "gdsfile.h"
#include "gdscell.h"

class GDTData
{
public:
	GDTData(QString fileName); // constructor to open (create if WRITE) stream file (calls opstrm())
	bool containsCell(QString name);

private:
	void goThroughFile();
	void buildDataStructure();

	GDSFile *m_file;
	QString m_fileName;

	QVector<GDSCell*> m_cells;
	GDSCell* m_recentCell;
};

#endif // GDTDATA_H