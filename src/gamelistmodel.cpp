#include "gamelistmodel.h"

GameListModel::GameListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

GameListModel::GameListModel(GameList list, QObject *parent) :
    QAbstractTableModel(parent)
{
    m_List = list;
}

int GameListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant GameListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // Check for index row outside of bounds
    if (index.row() >= m_List.length() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return m_List[index.row()].name();
        else if (index.column() == 1)
            return m_List[index.row()].maxUsers();
        else if (index.column() == 2)
            return m_List[index.row()].isPublic();
    }

    return QVariant();
}

QVariant GameListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        switch (section)
        {
        case 0:
            return tr("Name");
        case 1:
            return tr("Max Users");
        case 2:
            return tr("Public");
        default:
            return QAbstractTableModel::headerData(section, orientation, role);
        }

    return QAbstractTableModel::headerData(section, orientation, role);
}

int GameListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_List.length();
}

void GameListModel::setList(const GameList &list)
{
    beginResetModel();
    m_List = list;
    endResetModel();
}

Game GameListModel::game(const QModelIndex& index) const
{
    if (!index.isValid())
        return Game();

    // Check for index row outside of bounds
    if (index.row() >= m_List.length() || index.row() < 0)
        return Game();

    return m_List[index.row()];
}
