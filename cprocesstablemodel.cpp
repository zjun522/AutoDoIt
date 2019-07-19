#include "cprocesstablemodel.h"
#include <QFontMetrics>
#include "common/csyswin.h"

CProcessTableModel::CProcessTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    const QMap<quint64, QString> &processList = CSysWin::getInstance()->GetAllProcess();
    auto it = processList.begin();
    while (it != processList.end())
    {
        m_pList.append(SProcessInfo(it.key(), it.value()));
        it++;
    }
}

QVariant CProcessTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        if(section == 0)
            return u8"进程ID";
        if(section == 1) {
            return u8"进程名称";
        }
    }
    else
    {
        return section + 1;
    }
    return QVariant();
}

int CProcessTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_pList.count();
}

int CProcessTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant CProcessTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if(index.column() == 0) {
            return m_pList.at(index.row()).pId;
        }
        return m_pList.at(index.row()).pName;
    }
    return QVariant();
}

auto sortByIdLessThan = [](SProcessInfo a, SProcessInfo b)
{
    return (a.pId < b.pId);
};

auto sortByIdGreaterThan = [](SProcessInfo a, SProcessInfo b)
{
    return (a.pId > b.pId);
};

auto sortByNameLessThan = [](SProcessInfo a, SProcessInfo b)
{
    return (a.pName.toLower() < b.pName.toLower());
};

auto sortByNameGreaterThan = [](SProcessInfo a, SProcessInfo b)
{
    return (a.pName.toLower() > b.pName.toLower());
};

void CProcessTableModel::sort(int column, Qt::SortOrder order)
{
    beginResetModel();
    if(column == 0) {
        if(order == Qt::AscendingOrder)
            std::stable_sort(m_pList.begin(), m_pList.end(), sortByIdLessThan);
        else
            std::stable_sort(m_pList.begin(), m_pList.end(), sortByIdGreaterThan);
    }
    else {
        if(order == Qt::AscendingOrder)
            std::stable_sort(m_pList.begin(), m_pList.end(), sortByNameLessThan);
        else
            std::stable_sort(m_pList.begin(), m_pList.end(), sortByNameGreaterThan);
    }
    endResetModel();
}
