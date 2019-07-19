#ifndef CNEWTASKWIZARD_H
#define CNEWTASKWIZARD_H

#include <QWizard>
#include <memory>

class CTrigger;
class CCondition;
class CAction;
class QListWidgetItem;

namespace Ui {
class CNewTaskWizard;
}

class CNewTaskWizard : public QWizard
{
    Q_OBJECT

public:
    explicit CNewTaskWizard(QWidget *parent = nullptr);
    explicit CNewTaskWizard(CTrigger *trigger, QWidget *parent = nullptr);
    ~CNewTaskWizard() final;
    Q_DISABLE_COPY_MOVE(CNewTaskWizard)

    CTrigger *trigger() const;

private slots:
    void checkAvaliable();

    void on_CNewTaskWizard_accepted();

    void on_timeEdit_time_timeChanged(const QTime &time);

    void on_radioButton_timer_toggled(bool checked);

    void on_pushButton_explorer_clicked();

    void on_pushButton_addCondition_clicked();

    void on_pushButton_addAction_clicked();

    void on_pushButton_removeAction_clicked();

    void on_pushButton_removeCondition_clicked();

    void on_CNewTaskWizard_currentIdChanged(int id);

    void on_listWidget_condition_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_action_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_selectProcess_clicked();

private:
    Ui::CNewTaskWizard *ui;
    CTrigger *m_trigger = nullptr;

    void parseCondition(const std::shared_ptr<CCondition> &condition);
    void parseAction(const std::shared_ptr<CAction> &action);
};

#endif // CNEWTASKWIZARD_H
