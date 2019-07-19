#include <QSize>
#include <QFont>
#include <QBrush>
#include <QDebug>
#include <QIcon>
#include "clistmodel.h"
#include "trigger/ctrigger.h"
#include "action/caction.h"

CListModel::CListModel(QList<CTask *> *list, QObject *parent)
    : QAbstractListModel(parent),
      m_list(list)
{
}

//QVariant CListModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    if(role != Qt::DisplayRole)
//    {
//        return QVariant();
//    }

//    if(orientation == Qt::Horizontal)
//    {
//        return u8"任务名称";
//    }
//    else
//    {
//        return section + 1;
//    }
//    return QVariant();
//}

int CListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_list->count();
}

QVariant CListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::SizeHintRole)
    {
        return QSize(0, 30);
    }
    if(role == Qt::FontRole)
    {
        return QFont(QStringLiteral("宋体"), 10);
    }
    if (role == Qt::ForegroundRole)
    {
        CTask *task = m_list->at(index.row());
        if(task->getIsWatch()) {
            return QBrush(Qt::black);
        }
        return QBrush(Qt::darkGray);
        
    }
    if(role == Qt::ToolTipRole)
    {
        CTask *task = m_list->at(index.row());
        return task->getTipMsg();
    }
    if(role == Qt::DecorationRole)
    {
        CTask *task = m_list->at(index.row());
        return QIcon(task->getAction()->getIcon());
    }
    if (role == Qt::DisplayRole)
    {
        return m_list->at(index.row())->getTaskName();
    }
    if(role == Qt::BackgroundColorRole )
    {
        CTask *task = m_list->at(index.row());
        if(task->getDone() && !task->getLastActionSuccess()) {
            return QColor(0xFF, 0x80, 0x80);
        }
        return QColor(Qt::white);
    }
    return QVariant();
}

bool CListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i = 0; i < count; ++i)
    {
        delete m_list->takeAt(row);
    }
    emit dataChanged(index(row), index(row + count - 1));
    endRemoveRows();
    return true;
}

void CListModel::beginResetModel()
{
    QAbstractListModel::beginResetModel();
}

void CListModel::endResetModel()
{
    QAbstractListModel::endResetModel();
}
