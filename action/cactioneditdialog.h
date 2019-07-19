#ifndef CACTIONEDITDIALOG_H
#define CACTIONEDITDIALOG_H

#include <QDialog>
#include <memory>

class CAction;

namespace Ui {
class CActionEditDialog;
}

class CActionEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CActionEditDialog(QWidget *parent = nullptr);
    explicit CActionEditDialog(std::shared_ptr<CAction> action, QWidget *parent = nullptr);
    ~CActionEditDialog() final;

    CActionEditDialog(const CActionEditDialog &dlg) = delete;
    CActionEditDialog(CActionEditDialog &&dlg) = delete;
    CActionEditDialog& operator=(const CActionEditDialog &dlg) = delete;
    CActionEditDialog& operator=(CActionEditDialog &&dlg) = delete;

    std::shared_ptr<CAction> action() const;

private slots:
    void on_buttonBox_accepted();

    void on_timeEdit_delayBefore_timeChanged(const QTime &time);

    void on_timeEdit_delayAfter_timeChanged(const QTime &time);

    void on_pushButton_selectProcess_clicked();

private:
    Ui::CActionEditDialog *ui;

    std::shared_ptr<CAction> m_action;
};

#endif // CACTIONEDITDIALOG_H
