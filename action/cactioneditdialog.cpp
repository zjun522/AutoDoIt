#include "cactioneditdialog.h"
#include "ui_cactioneditdialog.h"
#include "cgetprocessdialog.h"
#include "cmakefactory.hpp"
#include "cgroupaction.h"
#include "cpoweroffaction.h"
#include "cstartstopprocessaction.h"
#include "cmessageaction.h"
#include "cexitautodoitaction.h"
#include "cstartstoptaskaction.h"
#include <QButtonGroup>

CActionEditDialog::CActionEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CActionEditDialog)
{
    ui->setupUi(this);

    auto *group = new QButtonGroup(this);
    group->addButton(ui->radioButton_startStopProcessAction);
    group->addButton(ui->radioButton_poweroffAction);
    group->addButton(ui->radioButton_messageAction);
    group->addButton(ui->radioButton_exitAction);
    group->addButton(ui->radioButton_startStopAction);

    ui->groupBox_startStopProcess->setVisible(false);
    ui->groupBox_message->setVisible(false);
    ui->groupBox_startStop->setVisible(false);
    ui->groupBox_powerOff->setVisible(false);

    adjustSize();
}

CActionEditDialog::CActionEditDialog(std::shared_ptr<CAction> action, QWidget *parent) :
    CActionEditDialog(parent)
{
    m_action = std::move(action);

    Q_ASSERT(m_action);
    ui->timeEdit_delayBefore->setTime(QTime(0, m_action->getDelayBefore()/60, m_action->getDelayBefore()%60));
    ui->timeEdit_delayAfter->setTime(QTime(0, m_action->getDelayAfter()/60, m_action->getDelayAfter()%60));
    ui->groupBox_delayBefore->setChecked(m_action->getDelayBefore());
    ui->groupBox_delayAfter->setChecked(m_action->getDelayAfter());
    if(std::dynamic_pointer_cast<CGroupAction>(m_action))
    {
        Q_ASSERT(false);
    }
    else if(std::shared_ptr<CPowerOffAction> act = std::dynamic_pointer_cast<CPowerOffAction>(m_action))
    {
        ui->radioButton_poweroffAction->setChecked(true);
        ui->radioButton_powerOff->setChecked(act->getPowerOffMode() == CPowerOffAction::EPOM_PowerOff);
        ui->radioButton_suspend->setChecked(act->getPowerOffMode() == CPowerOffAction::EPOM_Suspend);
        ui->radioButton_hibernate->setChecked(act->getPowerOffMode() == CPowerOffAction::EPOM_Hibernate);
    }
    else if(std::shared_ptr<CMessageAction> act = std::dynamic_pointer_cast<CMessageAction>(m_action))
    {
        ui->radioButton_messageAction->setChecked(true);
        ui->radioButton_message->setChecked(act->getMsgMode() == CMessageAction::ETM_Message);
        ui->radioButton_notify->setChecked(act->getMsgMode() == CMessageAction::ETM_Notify);
        ui->lineEdit_message->setText(act->getMessage());
    }
    else if(std::shared_ptr<CStartStopProcessAction> act = std::dynamic_pointer_cast<CStartStopProcessAction>(m_action))
    {
        ui->radioButton_startStopProcessAction->setChecked(true);
        ui->radioButton_startProcess->setChecked(act->getStart());
        ui->radioButton_stopProcess->setChecked(!act->getStart());
        ui->lineEdit_startStopProcess->setText(act->getProcessName());
    }
    else if(std::shared_ptr<CStartStopTaskAction> act = std::dynamic_pointer_cast<CStartStopTaskAction>(m_action))
    {
        ui->radioButton_startStopAction->setChecked(true);
        ui->radioButton_startTask->setChecked(act->getStart());
        ui->radioButton_stopTask->setChecked(!act->getStart());
    }
    else if(std::shared_ptr<CExitAutoDoItAction> act = std::dynamic_pointer_cast<CExitAutoDoItAction>(m_action))
    {
        ui->radioButton_exitAction->setChecked(true);
    }
}

CActionEditDialog::~CActionEditDialog()
{
    delete ui;
}

void CActionEditDialog::on_groupBox_delayBefore_clicked(bool checked)
{
    if(checked)
        on_timeEdit_delayBefore_timeChanged(ui->timeEdit_delayBefore->time());
    else
        ui->label_delayBefore->clear();
}

void CActionEditDialog::on_timeEdit_delayBefore_timeChanged(const QTime &time)
{
    ui->label_delayBefore->setText(QStringLiteral("延时%1分%2秒后执行").arg(time.minute()).arg(time.second()));
}

void CActionEditDialog::on_groupBox_delayAfter_clicked(bool checked)
{
    if(checked)
        on_timeEdit_delayAfter_timeChanged(ui->timeEdit_delayAfter->time());
    else
        ui->label_delayAfter->clear();
}

void CActionEditDialog::on_timeEdit_delayAfter_timeChanged(const QTime &time)
{
    ui->label_delayAfter->setText(QStringLiteral("执行后延时%1分%2秒").arg(time.minute()).arg(time.second()));
}

void CActionEditDialog::on_buttonBox_accepted()
{
    std::shared_ptr<CAction> action{nullptr};

    ulong delayBefore = 0;
    if(ui->groupBox_delayBefore->isChecked())
        delayBefore = static_cast<ulong>(ui->timeEdit_delayBefore->time().minute() * 60 + ui->timeEdit_delayBefore->time().second());
    ulong delayAfter = 0;
    if(ui->groupBox_delayAfter->isChecked())
        delayAfter = static_cast<ulong>(ui->timeEdit_delayAfter->time().minute() * 60 + ui->timeEdit_delayAfter->time().second());

    if(ui->radioButton_poweroffAction->isChecked())
    {
        CPowerOffAction::EPowerOffMode mode;
        if(ui->radioButton_powerOff->isChecked())
            mode = CPowerOffAction::EPOM_PowerOff;
        else if(ui->radioButton_suspend->isChecked())
            mode = CPowerOffAction::EPOM_Suspend;
        else if(ui->radioButton_hibernate->isChecked())
            mode = CPowerOffAction::EPOM_Hibernate;
        action = CMakeFactory::makeUnique<CPowerOffAction>(mode, delayBefore, delayAfter);
    }
    else if(ui->radioButton_exitAction->isChecked())
    {
        action = CMakeFactory::makeUnique<CExitAutoDoItAction>(delayBefore, delayAfter);
    }
    else if(ui->radioButton_startStopAction->isChecked())
    {
        action = CMakeFactory::makeUnique<CStartStopTaskAction>(ui->radioButton_startTask->isChecked(), delayBefore, delayAfter);
    }
    else if(ui->radioButton_startStopProcessAction->isChecked())
    {
        if(ui->lineEdit_startStopProcess->text().trimmed().isEmpty())
        {
            QPalette plt = ui->lineEdit_startStopProcess->palette();
            plt.setColor(QPalette::PlaceholderText, Qt::red);
            ui->lineEdit_startStopProcess->setPalette(plt);
            return;
        }
        QStringList list = ui->lineEdit_startStopProcess->text().trimmed().split(QStringLiteral(" "));
        const QString &program = list.takeAt(0);
        action = CMakeFactory::makeUnique<CStartStopProcessAction>(program, list, ui->radioButton_startProcess->isChecked(), delayBefore, delayAfter);
    }
    else if(ui->radioButton_messageAction->isChecked())
    {
        if(ui->lineEdit_message->text().trimmed().isEmpty())
        {
            QPalette plt = ui->lineEdit_message->palette();
            plt.setColor(QPalette::PlaceholderText, Qt::red);
            ui->lineEdit_message->setPalette(plt);
            return;
        }
        action = CMakeFactory::makeUnique<CMessageAction>(ui->lineEdit_message->text().trimmed(),
                                                          ui->radioButton_message->isChecked() ? CMessageAction::ETM_Message : CMessageAction::ETM_Notify,
                                                          delayBefore, delayAfter);
    }

    m_action = action;
    this->accept();
}

std::shared_ptr<CAction> CActionEditDialog::action() const
{
    return m_action;
}

void CActionEditDialog::on_pushButton_selectProcess_clicked()
{
    CGetProcessDialog dlg(this);
    if(QDialog::Accepted == dlg.exec())
    {
        ui->lineEdit_startStopProcess->setText(dlg.selectProcessName());
    }
}
