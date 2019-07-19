#include <QButtonGroup>
#include <QFileDialog>
#include <QCompleter>
#include "cconditioneditdialog.h"
#include "ui_cconditioneditdialog.h"
#include "common/cappenv.hpp"
#include "cgetprocessdialog.h"
#include "cmakefactory.hpp"
#include "cgroupcondition.h"
#include "ctimecondition.h"
#include "cfilecondition.h"
#include "cprocesscondition.h"

CConditionEditDialog::CConditionEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CConditionEditDialog)
{
    ui->setupUi(this);

    ui->timeEdit_time->setTime(QTime::currentTime());

    auto *group = new QButtonGroup(this);
    group->addButton(ui->radioButton_timeCondition);
    group->addButton(ui->radioButton_idleCondition);
    group->addButton(ui->radioButton_fileCondition);
    group->addButton(ui->radioButton_processCondition);

    ui->groupBox_fileTrigger->setVisible(false);
    ui->groupBox_processTrigger->setVisible(false);
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
}

CConditionEditDialog::CConditionEditDialog(std::shared_ptr<CCondition> condition, QWidget *parent) :
    CConditionEditDialog(parent)
{
    m_condition = std::move(condition);

    if(std::dynamic_pointer_cast<CGroupCondition>(m_condition))
    {
        Q_ASSERT(false);
    }
    else if(std::shared_ptr<CFileCondition> cdt = std::dynamic_pointer_cast<CFileCondition>(m_condition))
    {
        ui->radioButton_fileCondition->setChecked(true);
        ui->lineEdit_watchFile->setText(cdt->getFileName());
    }
    else if(std::shared_ptr<CProcessCondition> cdt = std::dynamic_pointer_cast<CProcessCondition>(m_condition))
    {
        ui->radioButton_processCondition->setChecked(true);
        ui->lineEdit_watchProcess->setText(cdt->getProcessName());
        ui->radioButton_processRunning->setChecked(cdt->getRunning());
        ui->radioButton_processNotRunning->setChecked(!cdt->getRunning());
    }
    else if(std::shared_ptr<CTimeCondition> cdt = std::dynamic_pointer_cast<CTimeCondition>(m_condition))
    {
        ui->radioButton_timeCondition->setChecked(true);
        ui->radioButton_after->setChecked(cdt->getTimeType() == CTimeCondition::ETT_AFTER);
        ui->radioButton_before->setChecked(cdt->getTimeType() == CTimeCondition::ETT_BEFORE);
        ui->timeEdit_time->setTime(cdt->getTime());
    }
    else if(std::shared_ptr<CIdleCondition> cdt = std::dynamic_pointer_cast<CIdleCondition>(m_condition))
    {
        ui->radioButton_idleCondition->setChecked(true);
        const quint64 &idleSec = cdt->getIdleSeconds();
        ui->timeEdit_idle->setTime(QTime(idleSec/3600000, (idleSec%3600000)/60000, (idleSec%60000)/1000));
    }
}

CConditionEditDialog::~CConditionEditDialog()
{
    delete ui;
}

void CConditionEditDialog::on_buttonBox_accepted()
{
    std::shared_ptr<CCondition> condition{nullptr};
    if(ui->radioButton_timeCondition->isChecked())
    {
        condition = CMakeFactory::makeUnique<CTimeCondition>(ui->timeEdit_time->time(),
                                                         ui->radioButton_after->isChecked() ? CTimeCondition::ETT_AFTER : CTimeCondition::ETT_BEFORE);
    }
    else if(ui->radioButton_idleCondition->isChecked())
    {
        const QTime &time = ui->timeEdit_idle->time();
        quint64 milliSeconds = time.hour() * static_cast<quint64>(3600000) + time.minute() * static_cast<quint64>(60000) + time.second() * static_cast<quint64>(1000);
        condition = CMakeFactory::makeUnique<CIdleCondition>(milliSeconds);
    }
    else if(ui->radioButton_fileCondition->isChecked())
    {
        if(ui->lineEdit_watchFile->text().trimmed().isEmpty())
        {
            QPalette plt = ui->lineEdit_watchFile->palette();
            plt.setColor(QPalette::PlaceholderText, Qt::red);
            ui->lineEdit_watchFile->setPalette(plt);
            return;
        }
        condition = CMakeFactory::makeUnique<CFileCondition>(ui->lineEdit_watchFile->text().trimmed());
    }
    else if(ui->radioButton_processCondition->isChecked())
    {
        if(ui->lineEdit_watchProcess->text().trimmed().isEmpty())
        {
            QPalette plt = ui->lineEdit_watchProcess->palette();
            plt.setColor(QPalette::PlaceholderText, Qt::red);
            ui->lineEdit_watchProcess->setPalette(plt);
            return;
        }
        condition = CMakeFactory::makeUnique<CProcessCondition>(ui->lineEdit_watchProcess->text().trimmed(),
                                                             ui->radioButton_processRunning->isChecked());
    }

    m_condition = condition;
    this->accept();
}


void CConditionEditDialog::on_pushButton_explorer_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr(u8"打开"));
    if(fileName.isEmpty())
        return;

    ui->lineEdit_watchFile->setText(fileName);
}

std::shared_ptr<CCondition> CConditionEditDialog::condition() const
{
    return m_condition;
}

void CConditionEditDialog::on_pushButton_selectProcess_clicked()
{
    CGetProcessDialog dlg(this);
    if(QDialog::Accepted == dlg.exec())
    {
        ui->lineEdit_watchProcess->setText(dlg.selectProcessName());
    }
}
