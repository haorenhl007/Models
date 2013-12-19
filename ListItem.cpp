#include "ListModel.h"
#include <QDebug>


Models::ListItem::ListItem(QObject *parent) : QObject(parent)
{
    if (qobject_cast<ListItem *>(parent) != NULL)
        qDebug() << "Parent is a listItem";
}

bool Models::ListItem::setData(int role, const QVariant &value)
{
    Q_UNUSED(role);
    Q_UNUSED(value);
    return false;
}

QHash<QByteArray, int> Models::ListItem::roleTypesFromName()
{
    QHash<int, QByteArray> roles = this->roleNames();
    QHash<QByteArray, int> typesForNameHash;

    foreach (const QByteArray &val, roles.values())
        typesForNameHash[val] = roles.key(val);

    return typesForNameHash;
}

void Models::ListItem::triggerItemUpdate()
{
    emit dataChanged();
}

bool Models::ListItem::operator<(const Models::ListItem &nextElem)
{
    Q_UNUSED(nextElem);
    return false;
}