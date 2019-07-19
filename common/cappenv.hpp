#ifndef CAPPENV_HPP
#define CAPPENV_HPP

#include <QDebug>
#include <QDirModel>

#ifdef USE_CUSTOM_DIR_MODEL
class CDirModel : public QDirModel
{
    Q_OBJECT
public:
    explicit CDirModel(QObject *parent = nullptr) :
        QDirModel(parent) {

    }

    QVariant data(const QModelIndex &index,
                  int role=Qt::DisplayRole) const override
    {
        if (role == Qt::DisplayRole && index.column() == 0) {
            QString path = QDir::toNativeSeparators(filePath(index));
            if (path.endsWith(QDir::separator()))
                path.chop(1);
            return path;
        }
        return QDirModel::data(index, role);
    }
};
#endif

class CAppEnv
{
public:
    CAppEnv();
};

#endif // CAPPENV_HPP
