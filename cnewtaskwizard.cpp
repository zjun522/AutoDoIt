#include <QListWidget>
#include <QButtonGroup>
#include <QFileDialog>
#include <QCompleter>
#include "cnewtaskwizard.h"
#include "ui_cnewtaskwizard.h"
#include "common/cappenv.hpp"
#include "clistwidgetitem.h"
#include "cgetprocessdialog.h"
#include "cmakefactory.hpp"
#include "condition/cconditioneditdialog.h"
#include "action/cactioneditdialog.h"

CNewTaskWizard::CNewTaskWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CNewTaskWizard)
{
    ui->setupUi(this);
    this->setWindowTitle(tr(u8"创建任务"));

    auto *list = new QListWidget;
//    list->setSelectionMode(QAbstractItemView::NoSelection);
//    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    list->setFrameShape(QFrame::NoFrame);
    for(auto i = 0; i < this->pageIds().count(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(page(i)->title());
        item->setSizeHint(QSize(50, 30));
        list->addItem(item);
    }
    this->setSideWidget(list);

    auto *group = new QButtonGroup(ui->wizardPage1);
    group->addButton(ui->radioButton_timeTrigger);
    group->addButton(ui->radioButton_idleTrigger);
    group->addButton(ui->radioButton_netTrigger);
    group->addButton(ui->radioButton_fileTrigger);
    group->addButton(ui->radioButton_processTrigger);

    ui->groupBox_fileTrigger->setVisible(false);
    ui->groupBox_processTrigger->setVisible(false);
    ui->groupBox_netTrigger->setVisible(false);
    ui->groupBox_idle->setVisible(false);

    auto *completer = new QCompleter(this);
#ifndef Q_WS_X11
    completer->setCaseSensitivity(Qt::CaseInsensitive);
#endif
#ifdef USE_CUSTOM_DIR_MODEL
    completer->setModel(new CDirModel(completer));
#else
    completer->setModel(new QDirModel(completer));
#endif
    ui->lineEdit_watchFile->setCompleter(completer);

    QRegExp regExp(R"(\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b)");
    auto *validator = new QRegExpValidator(regExp, ui->lineEdit_ip);
    ui->lineEdit_ip->setValidator(validator);

    connect(ui->radioButton_netTrigger, SIGNAL(toggled(bool)), this, SLOT(checkAvaliable()));
    connect(ui->radioButton_fileTrigger, SIGNAL(toggled(bool)), this, SLOT(checkAvaliable()));
    connect(ui->radioButton_processTrigger, SIGNAL(toggled(bool)), this, SLOT(checkAvaliable()));
    connect(ui->lineEdit_ip, SIGNAL(textChanged(QString)), this, SLOT(checkAvaliable()));
    connect(ui->lineEdit_netContent, SIGNAL(textChanged(QString)), this, SLOT(checkAvaliable()));
    connect(ui->lineEdit_watchFile, SIGNAL(textChanged(QString)), this, SLOT(checkAvaliable()));
    connect(ui->lineEdit_watchProcess, SIGNAL(textChanged(QString)), this, SLOT(checkAvaliable()));
    connect(ui->lineEdit_taskName, SIGNAL(textChanged(QString)), this, SLOT(checkAvaliable()));
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(checkAvaliable()));
}

CNewTaskWizard::CNewTaskWizard(CTrigger *trigger, QWidget *parent) :
    CNewTaskWizard(parent)
{
    this->setWindowTitle(tr(u8"修改任务"));
    m_trigger = trigger;
    Q_ASSERT(m_trigger);
    auto *groupTrigger = qobject_cast<CGroupTrigger *>(m_trigger);
    Q_ASSERT(groupTrigger);
    Q_ASSERT(groupTrigger->getTriggers().count() == 1);
//    m_editTriggerIsWatch = m_trigger->getIsWatch();
//    m_trigger->stopWatch();

    CTrigger *_trigger = groupTrigger->getTriggers().at(0);
    Q_ASSERT(_trigger);
    ui->radioButton_repeat->setChecked(m_trigger->getRepeatMode() == CTrigger::EPM_REPEAT);
    if(auto *trg = qobject_cast<CTimerTrigger *>(_trigger))
    {
        ui->radioButton_timeTrigger->setChecked(true);
        ui->radioButton_countDown->setChecked(trg->getTimerMode() == CTimerTrigger::ETM_CountDown);
        ui->radioButton_timer->setChecked(trg->getTimerMode() == CTimerTrigger::ETM_Timer);
        ui->timeEdit_time->setTime(trg->getTime());
    }
    else if(auto *trg = qobject_cast<CIdleTrigger *>(_trigger))
    {
        ui->radioButton_idleTrigger->setChecked(true);
        const quint64 &idleSec = trg->getIdleSeconds();
        ui->timeEdit_idle->setTime(QTime(idleSec/3600000, (idleSec%3600000)/60000, (idleSec%60000)/1000));
    }
    else if(auto *trg = qobject_cast<CWatchFileTrigger *>(_trigger))
    {
        ui->radioButton_fileTrigger->setChecked(true);
        ui->lineEdit_watchFile->setText(trg->getFileName());
    }
    else if(auto *trg = qobject_cast<CWatchProcessTrigger *>(_trigger))
    {
        ui->radioButton_processTrigger->setChecked(true);
        ui->lineEdit_watchProcess->setText(trg->getProcessName());
        ui->radioButton_processRunning->setChecked(trg->getRunning());
        ui->radioButton_processNotRunning->setChecked(!trg->getRunning());
    }
    else if(auto *trg = qobject_cast<CUDPTrigger *>(_trigger))
    {
        ui->radioButton_netTrigger->setChecked(true);
        ui->radioButton_udp->setChecked(true);
        ui->lineEdit_ip->setText(trg->getNetCmd().remoteHost);
        ui->lineEdit_netContent->setText(trg->getNetCmd().command);
    }
    else if(auto *trg = qobject_cast<CHTTPTrigger *>(_trigger))
    {
        ui->radioButton_netTrigger->setChecked(true);
        ui->radioButton_http->setChecked(true);
        ui->lineEdit_ip->setText(trg->getNetCmd().remoteHost);
        ui->lineEdit_netContent->setText(trg->getNetCmd().command);
    }

    parseCondition(m_trigger->getCondition());
    parseAction(m_trigger->getAction());
}

CNewTaskWizard::~CNewTaskWizard()
{
    delete ui;
}

void CNewTaskWizard::parseCondition(const std::shared_ptr<CCondition> &condition)
{
    if(std::shared_ptr<CGroupCondition> cdt = std::dynamic_pointer_cast<CGroupCondition>(condition))
    {
        ui->radioButton_or->setChecked(cdt->getAlg() == CGroupCondition::EAT_OR);
        ui->radioButton_and->setChecked(cdt->getAlg() == CGroupCondition::EAT_AND);
        parseCondition(cdt->getCdt1());
        parseCondition(cdt->getCdt2());
    }
    else if(condition)
    {
        ui->listWidget_condition->addItem(new CListWidgetItem(condition->getConditionName(), condition));
    }
}

void CNewTaskWizard::parseAction(const std::shared_ptr<CAction> &action)
{
    if(std::shared_ptr<CGroupAction> act = std::dynamic_pointer_cast<CGroupAction>(action))
    {
        foreach (std::shared_ptr<CAction> a, act->getActionList()) {
            parseAction(a);
        }
    }
    else if(action)
    {
        ui->listWidget_action->addItem(new CListWidgetItem(action->getActionName(), action));
    }
}

void CNewTaskWizard::on_CNewTaskWizard_accepted()
{
    std::shared_ptr<CGroupAction> action = CMakeFactory::makeUnique<CGroupAction>();
    for(auto i = 0; i < ui->listWidget_action->count(); ++i)
    {
        auto *item = dynamic_cast<CListWidgetItem*>(ui->listWidget_action->item(i));
        std::shared_ptr<CAction> act = std::static_pointer_cast<CAction>(item->ptr());
        action->addAction(act);
    }

    std::shared_ptr<CCondition> condition{nullptr};
    for(auto i = 0; i < ui->listWidget_condition->count(); ++i)
    {
        std::shared_ptr<CCondition> cdt;
        auto *item = dynamic_cast<CListWidgetItem*>(ui->listWidget_condition->item(i));
        cdt = std::static_pointer_cast<CCondition>(item->ptr());
        if(i == 0) {
            condition = cdt;
            if(ui->listWidget_condition->count() == 1)
            {
                condition = CMakeFactory::makeUnique<CGroupCondition>(condition, nullptr,
                                                                      ui->radioButton_and->isChecked() ? CGroupCondition::EAT_AND : CGroupCondition::EAT_OR);
            }
        }
        else {
            condition = CMakeFactory::makeUnique<CGroupCondition>(condition, cdt, ui->radioButton_and->isChecked() ? CGroupCondition::EAT_AND : CGroupCondition::EAT_OR);

        }

    }

    CGroupTrigger *trigger = CMakeFactory::makePtr<CGroupTrigger>(ui->radioButton_once->isChecked() ? CTrigger::EPM_ONCE : CTrigger::EPM_REPEAT,
                                                                  action, condition, this->parent());

    if(ui->radioButton_timeTrigger->isChecked())
    {
        trigger->addTrigger(CMakeFactory::makePtr<CTimerTrigger>(ui->timeEdit_time->time(),
                                                                 ui->radioButton_countDown->isChecked() ? CTimerTrigger::ETM_CountDown : CTimerTrigger::ETM_Timer,
                                                                 ui->radioButton_once->isChecked() ? CTrigger::EPM_ONCE : CTrigger::EPM_REPEAT));
    }
    else if(ui->radioButton_idleTrigger->isChecked())
    {
        const QTime &time = ui->timeEdit_idle->time();
        quint64 milliSeconds = time.hour() * static_cast<quint64>(3600000) + time.minute() * static_cast<quint64>(60000) + time.second() * static_cast<quint64>(1000);
        trigger->addTrigger(CMakeFactory::makePtr<CIdleTrigger>(milliSeconds, 10000,
                                                                ui->radioButton_once->isChecked() ? CTrigger::EPM_ONCE : CTrigger::EPM_REPEAT));
    }
    else if(ui->radioButton_netTrigger->isChecked())
    {
        if(ui->radioButton_udp->isChecked())
            trigger->addTrigger(CMakeFactory::makePtr<CUDPTrigger>(ui->lineEdit_ip->text().trimmed(),
                                                                   ui->lineEdit_netContent->text().trimmed(),
                                                                   ui->radioButton_once->isChecked() ? CTrigger::EPM_ONCE : CTrigger::EPM_REPEAT));
        else if(ui->radioButton_http->isChecked())
            trigger->addTrigger(CMakeFactory::makePtr<CHTTPTrigger>(ui->lineEdit_ip->text().trimmed(),
                                                                   ui->lineEdit_netContent->text().trimmed(),
                                                                   ui->radioButton_once->isChecked() ? CTrigger::EPM_ONCE : CTrigger::EPM_REPEAT));
    }
    else if(ui->radioButton_fileTrigger->isChecked())
    {
        trigger->addTrigger(CMakeFactory::makePtr<CWatchFileTrigger>(ui->lineEdit_watchFile->text().trimmed(), 10000,
                                                             ui->radioButton_once->isChecked() ? CTrigger::EPM_ONCE : CTrigger::EPM_REPEAT));
    }
    else if(ui->radioButton_processTrigger->isChecked())
    {
        trigger->addTrigger(CMakeFactory::makePtr<CWatchProcessTrigger>(ui->lineEdit_watchProcess->text().trimmed(), ui->radioButton_processRunning->isChecked(),
                                                                10000, ui->radioButton_once->isChecked() ? CTrigger::EPM_ONCE : CTrigger::EPM_REPEAT));
    }
    if(m_trigger)
    {
        delete m_trigger;
        m_trigger = trigger;
    }
    else
    {
        m_trigger = trigger;
    }
}

CTrigger *CNewTaskWizard::trigger() const
{
    return m_trigger;
}

void CNewTaskWizard::on_timeEdit_time_timeChanged(const QTime &time)
{
    if(ui->radioButton_timer->isChecked())
    {
        QDateTime dateTime;
        dateTime.setDate(QDate::currentDate());
        dateTime.setTime(time);
        const QDateTime &curDT = QDateTime::currentDateTime();
        if(curDT.time().msecsTo(time) < 0)
        {
            dateTime.setDate(curDT.addDays(1).date());
        }
        ui->label->setText(QStringLiteral("于%1触发").arg(dateTime.toString(QStringLiteral("yyyy/MM/dd HH:mm"))));
    }
    else
    {
        ui->label->setText(QStringLiteral("%1时%2分后触发").arg(time.hour()).arg(time.minute()));
    }
}

void CNewTaskWizard::on_radioButton_timer_toggled(bool checked)
{
    if(checked)
    {
        ui->timeEdit_time->setMinimumTime(QTime::fromString(QStringLiteral("00:00"), QStringLiteral("hh:mm")));
    }
    else
    {
        ui->timeEdit_time->setMinimumTime(QTime::fromString(QStringLiteral("00:01"), QStringLiteral("hh:mm")));
        if(ui->timeEdit_time->time().secsTo(QTime::fromString(QStringLiteral("00:01"), QStringLiteral("hh:mm"))) > 0)
            ui->timeEdit_time->setTime(QTime::fromString(QStringLiteral("00:01"), QStringLiteral("hh:mm")));
    }
    on_timeEdit_time_timeChanged(ui->timeEdit_time->time());
}

void CNewTaskWizard::on_pushButton_explorer_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr(u8"打开"));
    if(fileName.isEmpty())
        return;

    ui->lineEdit_watchFile->setText(fileName);
}

void CNewTaskWizard::on_pushButton_addCondition_clicked()
{
    CConditionEditDialog dlg(this);
    if(QDialog::Accepted == dlg.exec())
    {
        std::shared_ptr<CCondition> condition(dlg.condition());
        ui->listWidget_condition->addItem(new CListWidgetItem(condition->getConditionName(), condition));
    }
}

void CNewTaskWizard::on_pushButton_removeCondition_clicked()
{
    delete ui->listWidget_condition->takeItem(ui->listWidget_condition->currentRow());
}

void CNewTaskWizard::on_pushButton_addAction_clicked()
{
    CActionEditDialog dlg(this);
    if(QDialog::Accepted == dlg.exec())
    {
        std::shared_ptr<CAction> action(dlg.action());
        ui->listWidget_action->addItem(new CListWidgetItem(action->getActionName(), action));
    }
    checkAvaliable();
}

void CNewTaskWizard::on_pushButton_removeAction_clicked()
{
    delete ui->listWidget_action->takeItem(ui->listWidget_action->currentRow());
    checkAvaliable();
}

void CNewTaskWizard::checkAvaliable()
{
    bool ava = true;
    switch (this->currentId()) {
    case 0:
        if(ui->radioButton_netTrigger->isChecked() && (ui->lineEdit_netContent->text().trimmed().isEmpty()
                || ui->lineEdit_ip->text().trimmed().isEmpty()))
        {
            ava = false;
            goto getValue;
        }
        if(ui->radioButton_fileTrigger->isChecked() && ui->lineEdit_watchFile->text().trimmed().isEmpty())
        {
            ava = false;
            goto getValue;
        }
        if(ui->radioButton_processTrigger->isChecked() && ui->lineEdit_watchProcess->text().trimmed().isEmpty())
        {
            ava = false;
            goto getValue;
        }
        break;
    case 2:
        ava = ui->listWidget_action->count() > 0;
        break;
    case 3:
        ava = !ui->lineEdit_taskName->text().trimmed().isEmpty();
        break;
    default:
        break;
    }


getValue:
    this->button(QWizard::NextButton)->setEnabled(ava);
    this->button(QWizard::FinishButton)->setEnabled(ava);
}


void CNewTaskWizard::on_CNewTaskWizard_currentIdChanged(int id)
{
    if(id == 3)
    {
        QString str;
        if(ui->radioButton_timeTrigger->isChecked())
        {
            if(ui->radioButton_countDown->isChecked()) {
                str = ui->timeEdit_time->time().toString(QStringLiteral("HH小时mm分种后"));
            }
            else {
                str = ui->timeEdit_time->time().toString(QStringLiteral("HH:mm(时:分)时"));
            }
        }
        if(ui->radioButton_idleTrigger->isChecked())
        {
            str = QStringLiteral("计算机空闲时");
        }
        else if(ui->radioButton_netTrigger->isChecked())
        {
            str = QStringLiteral("收到%1指令后").arg(ui->radioButton_udp->isChecked() ? QStringLiteral("UDP") : QStringLiteral("HTTP"));
        }
        else if(ui->radioButton_fileTrigger->isChecked())
        {
            str = QStringLiteral("文件“%1”存在时").arg(QFileInfo(ui->lineEdit_watchFile->text().trimmed()).fileName());
        }
        else if(ui->radioButton_processTrigger->isChecked())
        {
            str = QStringLiteral("进程“%1”%2时").arg(ui->lineEdit_watchProcess->text().trimmed(), ui->radioButton_processRunning->isChecked() ? QStringLiteral("运行") : QStringLiteral("退出"));
        }

        for(auto i = 0; i < ui->listWidget_action->count(); ++i)
        {
            auto *item = dynamic_cast<CListWidgetItem*>(ui->listWidget_action->item(i));
            std::shared_ptr<CAction> act = std::static_pointer_cast<CAction>(item->ptr());
            str += act->getActionName() + u8"、";
        }
        str.chop(1);

        ui->lineEdit_taskName->setText(str);
    }
}

void CNewTaskWizard::on_listWidget_condition_itemDoubleClicked(QListWidgetItem *item)
{
    auto *e = dynamic_cast<CListWidgetItem*>(item);
    CConditionEditDialog dlg(std::static_pointer_cast<CCondition>(e->ptr()), this);
    if(QDialog::Accepted == dlg.exec())
    {
        e->setPtr(dlg.condition()->getConditionName(), dlg.condition());
    }
}

void CNewTaskWizard::on_listWidget_action_itemDoubleClicked(QListWidgetItem *item)
{
    auto *e = dynamic_cast<CListWidgetItem*>(item);
    CActionEditDialog dlg(std::static_pointer_cast<CAction>(e->ptr()), this);
    if(QDialog::Accepted == dlg.exec())
    {
        e->setPtr(dlg.action()->getActionName(), dlg.action());
    }
}

void CNewTaskWizard::on_pushButton_up_clicked()
{
    int row = ui->listWidget_action->currentRow();
    if(row < 1)
        return;
    QListWidgetItem *item = ui->listWidget_action->takeItem(row);
    ui->listWidget_action->insertItem(row - 1, item);
    ui->listWidget_action->setCurrentItem(item);
}

void CNewTaskWizard::on_pushButton_down_clicked()
{
    int row = ui->listWidget_action->currentRow();
    if(row < 0 || row > ui->listWidget_action->count() - 2)
        return;
    QListWidgetItem *item = ui->listWidget_action->takeItem(row);
    ui->listWidget_action->insertItem(row + 1, item);
    ui->listWidget_action->setCurrentItem(item);
}


void CNewTaskWizard::on_pushButton_selectProcess_clicked()
{
    CGetProcessDialog dlg(this);
    if(QDialog::Accepted == dlg.exec())
    {
        ui->lineEdit_watchProcess->setText(dlg.selectProcessName());
    }
}
