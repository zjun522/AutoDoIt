#ifndef CGETPROCESSDIALOG_H
#define CGETPROCESSDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class CGetProcessDialog;
}

class CGetProcessDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CGetProcessDialog(QWidget *parent = nullptr);
    ~CGetProcessDialog() final;
    Q_DISABLE_COPY_MOVE(CGetProcessDialog)

    QString selectProcessName() const;

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::CGetProcessDialog *ui;
    QString m_selectProcessName;
};

#endif // CGETPROCESSDIALOG_H
