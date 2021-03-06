#ifndef VCDVcdTreeItem_H
#define VCDVcdTreeItem_H

#include <QList>
#include <QVariant>

class VcdTreeItem
{
public:
	explicit VcdTreeItem(const QList<QVariant> &data, VcdTreeItem *parentItem = 0);
	~VcdTreeItem();

	VcdTreeItem * appendChild(QString key);

	VcdTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	VcdTreeItem *parentItem();

private:
	QList<VcdTreeItem*> m_childItems;
	QMap<QString,VcdTreeItem*> m_childItemMap;
	QList<QVariant> m_itemData;
	VcdTreeItem *m_parentItem;
};

#endif // VCDVcdTreeItem_H
