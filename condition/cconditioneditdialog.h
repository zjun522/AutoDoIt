#ifndef CCONDITIONEDITDIALOG_H
#define CCONDITIONEDITDIALOG_H

#include <QDialog>
#include <memory>

class CCondition;

namespace Ui {
class CConditionEditDialog;
}

class CConditionEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CConditionEditDialog(QWidget *parent = nullptr);
    explicit CConditionEditDialog(std::shared_ptr<CCondition> condition, QWidget *parent = nullptr);
    ~CConditionEditDialog() final;
    Q_DISABLE_COPY_MOVE(CConditionEditDialog)

    std::shared_ptr<CCondition> condition() const;

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_explorer_clicked();

    void on_pushButton_selectProcess_clicked();

private:
    Ui::CConditionEditDialog *ui;

    std::shared_ptr<CCondition> m_condition;
};

#endif // CCONDITIONEDITDIALOG_H
