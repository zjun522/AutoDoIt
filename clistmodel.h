#ifndef CLISTMODEL_H
#define CLISTMODEL_H

#include <QAbstractListModel>

class CAction;
class CTrigger;
using CTask = CTrigger;

class CListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CListModel(QList<CTask *> *list, QObject *parent = nullptr);

    // Header:
//    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void beginResetModel();
    void endResetModel();

private:
    QList<CTask *> *m_list = nullptr;
};

#endif // CLISTMODEL_H
