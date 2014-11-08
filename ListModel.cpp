/****************************************************************************
**
** Copyright (C) Paul Lemire, Tepee3DTeam and/or its subsidiary(-ies).
** Contact: paul.lemire@epitech.eu
** Contact: tepee3d_2014@labeip.epitech.eu
**
** This file is part of the Tepee3D project
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
****************************************************************************/

#include "ListModel.h"
// DEBUG
#include <QDebug>


/*!
 * \namespace Models
 *
 * \brief The Models namespace contains all classes use for Data Model
 * encapsulation within the application.
 *
 * \inmodule Tepee3D
 *
 *
 */


/*!
 * \class Models::ListItem
 *
 * \code
 * #include <ListItem.h>
 * \endcode
 *
 * \brief The basic element Models::ListModel are composed of.
 *
 * The basic element Models::ListModel are composed of. It gives a representation
 * of the data it contains throught the use of data retrieved by matching role names.
 * Your model items should inherit from this class and implement the various methods to reflect
 * what your item contains.
 *
 * \sa Models::ListModel
 *
 * \inmodule Tepee3D
 */

/*!
 * \fn Models::ListItem::ListItem(QObject *parent)
 *
 * Constructs a new model item with the optionnal \a parent argument/
 */

/*!
 * \fn Models::ListItem::~ListItem()
 *
 * Destroys a ListItem instance.
 */

/*!
 * \fn int Models::ListItem::id() const
 *
 * Returns the id of the item.
 */

/*!
 * \fn QVariant Models::ListItem::data(int role) const
 *
 * Returns the data associated to a given \a role.
 */

/*!
 * \fn bool Models::ListItem::setData(int role, const QVariant& value)
 *
 * If you want your model to be editable, implement this method in your base class,
 * updating the value of the field identified by \a role with \a value;
 */

/*!
 * \fn QHash<int, QByteArray> Models::ListItem::roleNames() const
 *
 * Returns a QHash containing the roleNames and a corresponding role identifier.
 */

/*!
 * \fn void Models::ListItem::triggerItemUpdate()
 *
 * Triggers an update of the data model to reflect the changes of the ListItem.
 */

/*!
 * \fn void Models::ListItem::dataChanged()
 *
 * Triggered when an update of the ListItem is requested.
 */

/*!
 * \class Models::ListModel
 *
 * \code
 * #include <ListModel.h>
 * \endcode
 *
 * \brief The Models::ListModel class is a utility model class
 *
 * The Models::ListModel class provides an easy way to create a C++ model
 * and later expose it to the Qml View.
 *
 * \inmodule Tepee3D
 *
 * \sa Models::SubListedListModel
 */

/*!
 * Instanciates a new ListModel and sets the list row prototype to \a prototype.
 * The \a parent parameter is optional.
 *
 */
Models::ListModel::ListModel(Models::ListItem *prototype, QObject *parent) : QAbstractListModel(parent)
{
    // We need to register that metatype so that the QML engine
    // knows about
    qRegisterMetaType<Models::ListItem*>("ListItem*");

    // So that we can return ListModel * from a Q_INVOKABLE to QML
    // without having the QmlEngine take ownership and destroy our object
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    this->m_prototype = prototype;
    this->m_sortEnabled = false;
}

/*!
 * Destroys a ListModel instance.
 */
Models::ListModel::~ListModel()
{
    delete this->m_prototype;
    this->m_prototype = NULL;
    this->clear();
}

/*!
 * \property Models::ListModel::count
 *
 * This property holds the number of rows of the model.
 */

/*!
 * Returns number of rows in the model.
 * \a index is not used but needed to reimplement the method.
 */
int         Models::ListModel::rowCount(const QModelIndex &) const
{
    return this->m_items.size();
}

/*!
 * Returns a QVariant containing the data associed to \a role for row at \a index.
 */
QVariant    Models::ListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < this->m_items.size())
        return this->m_items.at(index.row())->data(role);
    return QVariant();
}

/*!
 * Sets the data element specified by \a role to \a value
 */

bool    Models::ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= 0 && index.row() < this->m_items.size())
        return this->m_items.at(index.row())->setData(role, value);
    return false;
}

/*!
 * Returns a hash containing the roleNames of the Model.
 */
QHash<int, QByteArray>  Models::ListModel::roleNames() const
{
        return this->m_prototype->roleNames();
}

/*!
 * Returns the ListItem contained at \a row if it has been specified.
 * Otherwise the first ListItem is returned.
 */
Models::ListItem *Models::ListModel::takeRow(int row, const QModelIndex &index)
{
    if (row == -2) // IF ROW HAS NOT BEEN SPECIFIED TAKE FIRST ITEM
        row = 0;
    if (row >= 0 && row < this->m_items.size())
    {
        beginRemoveRows(index, row, row);
        Models::ListItem *item = this->m_items.takeAt(row);
        endRemoveRows();
        emit (countChanged(this->rowCount()));
        return item;
    }
    return NULL;
}

/*!
 * Returns a list of items that are contained in the model from the row \a row
 * to the row \a row + \a count. The items are removed from the model.
 * If \a row is not specified, it starts from the first row.
 * If count is not specified, returns items from \a row to the end of the model.
 * In that case if row is not the first row, nothing will be returned.
 */
QList<Models::ListItem *> Models::ListModel::takeRows(int row, int count, const QModelIndex &index)
{
    QList<Models::ListItem *> items;

    if (row == -2) // IF ROW HAS NOT BEEN SPECIFIED TAKE FIRST ITEM
        row = 0;
    if (count == -1)
        count = this->m_items.size();
    if (row >= 0 && count > 0 && (row + count) <= this->m_items.size())
    {
        beginRemoveRows(index, row, row + count - 1);
        for (int i = 0; i < count; i++)
            items << this->m_items.takeAt(row);
        endRemoveRows();
        emit (countChanged(this->rowCount()));
    }
    return items;
}

/*!
 * Appends a single row \a item to the Model.
 */
void        Models::ListModel::appendRow(Models::ListItem *item)
{
    if (item != NULL)
    {
        this->appendRows(QList<Models::ListItem *>() << item);
        emit (countChanged(this->rowCount()));
    }
}

/*!
 * Appends several rows \a items to the Model.
 */
void        Models::ListModel::appendRows(const QList<Models::ListItem *> &items)
{
    if (items.size() == 0)
        return ;
    // NEEDED TO UPDATE VIEW
    this->beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount() + items.size() - 1);
    foreach(Models::ListItem *item, items)
    {
        QObject::connect(item, SIGNAL(dataChanged()), this, SLOT(updateItem()));
        this->m_items.append(item);
    }
    // NEEDED TO UPDATE VIEW
    this->endInsertRows();
    this->sort();
    emit (countChanged(this->rowCount()));
}

/*!
 * Insert new row described by \a item at position defined by \a row.
 */
void       Models::ListModel::insertRow(int row, Models::ListItem *item)
{
    if (item == NULL)
        return ;
    this->beginInsertRows(QModelIndex(), row, row);
    QObject::connect(item, SIGNAL(dataChanged()), this, SLOT(updateItem()));
    this->m_items.insert(row, item);
    this->endInsertRows();
    this->sort();
    emit (countChanged(this->rowCount()));
}

/*!
 * Removes a single row at position defined by \a row .
 * The \a index argument is optional.
 * Returns true if row was removed, false if row not found or \a row is invalid.
 */
bool        Models::ListModel::removeRow(int row, const QModelIndex &index)
{
    if (row >= 0 && row < this->m_items.size())
    {
        beginRemoveRows(index, row, row);
        Models::ListItem *item = this->m_items.takeAt(row);
        delete item;
        item = NULL;
        endRemoveRows();
        emit (countChanged(this->rowCount()));
        return true;
    }
    return false;
}

/*!
 * Removes several rows starting at position defined by \a row until either \a count
 * or the model's last row is reached. The \a index argument is optional.
 * Returns true if the rows were removed, false if \a row is invalid.
 */
bool        Models::ListModel::removeRows(int row, int count, const QModelIndex &index)
{
    if (row >= 0 && count > 0 && (row + count) <= this->m_items.size())
    {
        beginRemoveRows(index, row, row + count - 1);
        for (int i = 0; i < count; i++)
        {
            Models::ListItem *item = this->m_items.takeAt(row);
            delete item;
            item = NULL;
        }
        endRemoveRows();
        emit (countChanged(this->rowCount()));
        return true;
    }
    return false;
}

/*!
 * Clears the whole model removing all rows.
 */
void        Models::ListModel::clear()
{
    if (this->m_items.size() == 0)
        return ;
    this->removeRows(0, this->m_items.size());
    emit (countChanged(this->rowCount()));
}

bool Models::ListModel::sortingEnabled() const
{
    return this->m_sortEnabled;
}

void Models::ListModel::setSorting(bool value)
{
    if (value == this->sortingEnabled())
        return;
    this->m_sortEnabled = value;
    emit sortingChanged(value);
    this->sort();
}

/*!
 * Returns the ListItem otype associated to the given model.
 */

Models::ListItem *Models::ListModel::getPrototype() const
{
    return this->m_prototype;
}

/*!
 * Sorts the elements of the models
 */
void Models::ListModel::sort()
{
    if (this->m_sortEnabled)
    {
        qSort(this->m_items.begin(), this->m_items.end(), compareFunc);
        foreach (Models::ListItem *item, this->m_items)
        {
            QModelIndex index = this->indexFromItem(item);
            if (index.isValid())
                emit dataChanged(index, index);
        }
    }
}

/*!
 * Returns the index of the row in the model containing \a item.
 */
QModelIndex     Models::ListModel::indexFromItem(Models::ListItem *item) const
{
    if (item != NULL)
    {
        for (int i = 0; i < this->m_items.size(); i++)
            if (this->m_items.at(i) == item)
                return index(i);
    }
    return QModelIndex();
}

/*!
 * Returns the item whose id matches \a itemId.
 */
Models::ListItem *  Models::ListModel::find(int itemId) const
{
    foreach(Models::ListItem *item, this->m_items)
        if (item->id() == itemId)
            return item;
    return NULL;
}
/*!
 * Returns row index at which \a item can be found in the model.
 */
int         Models::ListModel::getRowFromItem(ListItem *item) const
{
    if (item != NULL)
        for (int i = 0; i < this->m_items.size(); i++)
            if (this->m_items.at(i) == item)
                return i;
    return -1;
}

/*!
 * Returns model as a QList.
 */
QList<Models::ListItem *>   Models::ListModel::toList() const
{
    return this->m_items;
}

/*!
 * Slot triggered when a row item needs to be updated to reflect data changes.
 */
void        Models::ListModel::updateItem()
{
    Models::ListItem *item = static_cast<Models::ListItem *>(sender());
    QModelIndex index = this->indexFromItem(item);
    if (index.isValid())
        emit dataChanged(index, index);
}

/*!
 * Returns the row item at \a index in the model.
 */
Models::ListItem   *Models::ListModel::get(int index) const
{
    if (index >= this->m_items.size() || index < 0)
        return Q_NULLPTR;
    return this->m_items.at(index);
}

/*!
 * Returns the index for item identified by \a id  in the model.
 */
int         Models::ListModel::rowIndexFromId(int id)
{
    Models::ListItem* item = find(id);

    if (item)
        return indexFromItem(item).row();
    return -1;
}

bool compareFunc(void *a, void *b)
{
    return *static_cast<Models::ListItem *>(a) < *static_cast<Models::ListItem *>(b);
}
