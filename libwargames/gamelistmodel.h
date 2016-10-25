/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-09
 * \file    gamelistmodel.h
 * \brief   Subclasses QAbstractTableModel for Game objects.
 */

#ifndef GAMELISTMODEL_H
#define GAMELISTMODEL_H

#include <QAbstractTableModel>
#include "game.h"

/*!
 * \brief The GameListModel class subclasses QAbstractTableModel to provide a
 *        list of Games in a QAbstractItemView.
 */
class GameListModel : public QAbstractTableModel
{
public:
    GameListModel(QObject *parent = 0);
    GameListModel(GameList list, QObject *parent = 0);

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
//    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
//    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;

    void setList(const GameList& list);
    Game game(const QModelIndex& index) const;

private:
    GameList m_List;
};

#endif // GAMELISTMODEL_H
