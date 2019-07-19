#ifndef CPROCESSTABLEMODEL_H
#define CPROCESSTABLEMODEL_H

#include <QAbstractTableModel>

struct SProcessInfo
{
    SProcessInfo(quint64 _pId, QString _pName) :
        pId(_pId), pName(std::move(_pName)) {}
    quint64 pId = 0;
    QString pName;
};

class CProcessTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CProcessTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
private:
    QList<SProcessInfo> m_pList;
};

#endif // CPROCESSTABLEMODEL_H
