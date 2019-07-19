#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QCloseEvent>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include "common/cappenv.hpp"
#include "common/csyswin.h"
#include "version.h"
#include "clistmodel.h"
#include "cnewtaskwizard.h"
#include "cmakefactory.hpp"
#include "trigger/cgrouptrigger.h"

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
namespace {
constexpr qint32 g_magicNumber = 0x5A98BDA;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    ui->action_debug->setVisible(false);
#endif

    QPalette plt = ui->mainToolBar->palette();
    plt.setColor(QPalette::Window, Qt::white);
    ui->mainToolBar->setPalette(plt);
    ui->mainToolBar->setBackgroundRole(QPalette::Window);
    ui->mainToolBar->setAutoFillBackground(true);
    initSysTray();

    CSysWin::getInstance(this);
    restoreTask();

    ui->listView->setModel(m_model = new CListModel(&m_taskList, ui->listView));
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

    m_menu = new QMenu(this);
    m_menu->addAction(ui->action_editTask);
    m_menu->addSeparator();
    m_menu->addAction(ui->action_startTask);
    m_menu->addAction(ui->action_stopTask);
    m_menu->addSeparator();
    m_menu->addAction(ui->action_doTask);

    updateAction();
}

MainWindow::~MainWindow()
{
    CSysWin::releaseInstance();
    delete ui;
}

void MainWindow::showNotify(const QString &msg)
{
    m_sysTray->showMessage(QApplication::applicationName(), msg, QSystemTrayIcon::Information, 200);
}

void MainWindow::on_action_startAllTask_triggered()
{
    foreach (CTask *task, m_taskList) {
        task->startWatch();
    }
}

void MainWindow::on_action_stopAllTask_triggered()
{
    foreach (CTask *task, m_taskList) {
        task->stopWatch();
    }
}

void MainWindow::initSysTray()
{
    m_sysTray = new QSystemTrayIcon(this);
    m_sysTray->setIcon(QIcon(":/image/Attachment.png"));
    m_sysTray->setToolTip(tr(u8"AutoDoIt is running"));
//    m_sysTray->showMessage(QApplication::applicationName(), tr(u8"欢迎使用%0！").arg(QApplication::applicationName()),
//                           QSystemTrayIcon::Information, 2000);
    connect(m_sysTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(sysTrayActivated(QSystemTrayIcon::ActivationReason)));

    m_sysTrayMenu = new QMenu(this);
    m_sysTrayMenu->addAction(ui->action_addTask);
    m_sysTrayMenu->addSeparator();
    m_sysTrayMenu->addAction(ui->action_startAllTask);
    m_sysTrayMenu->addAction(ui->action_stopAllTask);
    m_sysTrayMenu->addSeparator();
    m_sysTrayMenu->addAction(ui->action_autoStart);
    ui->action_autoStart->setChecked(getIsAutoStart());
    m_sysTrayMenu->addSeparator();
    m_sysTrayMenu->addAction(ui->action_about);
    m_sysTrayMenu->addSeparator();
    m_sysTrayMenu->addAction(ui->action_quit);

    m_sysTray->setContextMenu(m_sysTrayMenu);
    m_sysTray->show();
}

bool MainWindow::getIsAutoStart()
{
    QSettings settings(QStringLiteral(REG_RUN), QSettings::NativeFormat);
    return settings.contains(QApplication::applicationName());
}

void MainWindow::updateAction()
{
    const QModelIndex &index = ui->listView->currentIndex();
    if(index.isValid()) {
        ui->action_editTask->setEnabled(true);
        ui->action_doTask->setEnabled(true);
        ui->action_startTask->setEnabled(true);
        ui->action_stopTask->setEnabled(true);
        ui->action_removeTask->setEnabled(true);
    }
    else {
        ui->action_editTask->setEnabled(false);
        ui->action_doTask->setEnabled(false);
        ui->action_startTask->setEnabled(false);
        ui->action_stopTask->setEnabled(false);
        ui->action_removeTask->setEnabled(false);
    }
}

void MainWindow::sysTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger://单击
    case QSystemTrayIcon::DoubleClick://双击
    case QSystemTrayIcon::MiddleClick://滚轮
        this->show();
        if (windowState() & Qt::WindowMinimized)
        {
            setWindowState(Qt::WindowNoState);
            //            restoreGeometry();
        }
        this->activateWindow();
        this->raise();
        break;
    default:
        break;
    }
}

void MainWindow::on_action_autoStart_triggered(bool checked)
{
    QSettings settings(QStringLiteral(REG_RUN), QSettings::NativeFormat);
    if(checked)
    {
        QString appPath = QApplication::applicationFilePath();
        settings.setValue(QApplication::applicationName(), QStringLiteral("\"") + appPath.replace(QStringLiteral("/"), QStringLiteral("\\")) + QStringLiteral("\""));
    }
    else
    {
        settings.remove(QApplication::applicationName());
    }
}

void MainWindow::on_action_quit_triggered()
{
    saveTask();
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if (this->isVisible()) {
        this->hide();
        event->ignore();
        return;
    }
    on_action_quit_triggered();
}

void MainWindow::on_listView_customContextMenuRequested(const QPoint &point)
{
    const QModelIndex &index = ui->listView->indexAt(point);
    if(index.isValid())
    {
        updateAction();
        m_menu->exec(QCursor::pos());
    }
}

void MainWindow::on_action_addTask_triggered()
{
    CNewTaskWizard wizard(this);
    if(QDialog::Accepted == wizard.exec())
    {
        CTask *task = wizard.trigger();
        task->startWatch();
        m_model->beginResetModel();
        m_taskList.append(task);
        m_model->endResetModel();
    }
    updateAction();
}

void MainWindow::on_action_removeTask_triggered()
{
    int row = ui->listView->currentIndex().row();
    if(row >= 0 && row <= m_taskList.count() - 1)
    {
        m_model->removeRows(row, 1);
    }
    updateAction();
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    if(row < 0 || row >= m_taskList.count())
        return;
    CTask *curTask = m_taskList.at(row);
    CNewTaskWizard wizard(curTask, this);
    if(QDialog::Accepted == wizard.exec())
    {
        CTask *task = wizard.trigger();
        task->startWatch();
        m_model->beginResetModel();
        m_taskList.replace(row, task);
        m_model->endResetModel();
    }
}

void MainWindow::on_action_startTask_triggered()
{
    int row = ui->listView->currentIndex().row();
    if(row < 0 || row >= m_taskList.count())
        return;

    m_taskList.at(row)->startWatch();
}

void MainWindow::on_action_stopTask_triggered()
{
    int row = ui->listView->currentIndex().row();
    if(row < 0 || row >= m_taskList.count())
        return;

    m_taskList.at(row)->stopWatch();
}

void MainWindow::on_action_doTask_triggered()
{
    int row = ui->listView->currentIndex().row();
    if(row < 0 || row >= m_taskList.count())
        return;

    m_taskList.at(row)->doItManual();
}

void MainWindow::on_action_editTask_triggered()
{
    on_listView_doubleClicked(ui->listView->currentIndex());
}

void MainWindow::on_listView_clicked(const QModelIndex &)
{
    updateAction();
}

void MainWindow::restoreTask()
{
    QFile file(QDir::homePath() + "/" + QStandardPaths::displayName(QStandardPaths::DocumentsLocation)
               + QStringLiteral("/") + QStringLiteral(VER_PRODUCTNAME_STR) + QStringLiteral("/Tasks"));
    if(!file.exists())
    {
        file.setFileName(":/Tasks");
        showNotify(u8"AutoDoIt默认托盘运行！");
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        showNotify(QStringLiteral("载入任务文件失败"));
        return;
    }

    QDataStream stream(&file);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
    stream.setVersion(QDataStream::Qt_5_13);
#elif (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    in.setVersion(QDataStream::Qt_5_12);
#endif
    qint32 magicNumber = 0;
    stream >> magicNumber;
    if(g_magicNumber != magicNumber)
    {
        showNotify(QStringLiteral("文件“%1”不是%2任务文件").arg(file.fileName(), QApplication::applicationName()));
        return;
    }
    int total = 0;
    stream >> total;
    for(auto i = 0; i < total; ++i)
    {
        QString className;
        stream >> className;
        if(className == QString(typeid(CGroupTrigger).name()))
        {
            CGroupTrigger *trigger = CMakeFactory::makePtr<CGroupTrigger>(CTrigger::EPM_ONCE, nullptr, nullptr, QStringLiteral(""), this);
            *trigger << stream;
            m_taskList.append(trigger);
        }
        else
        {
            showNotify(QStringLiteral("载入任务文件失败"));
            qDeleteAll(m_taskList);
            m_taskList.clear();
            return;
        }
    }
    foreach (CTrigger *trigger, m_taskList) {
        if(trigger->getIsWatch())
            trigger->startWatch();
    }
}

void MainWindow::saveTask()
{
    QFile file(QDir::homePath() + QStringLiteral("/") + QStandardPaths::displayName(QStandardPaths::DocumentsLocation) + QStringLiteral("/") + QStringLiteral(VER_PRODUCTNAME_STR) + QStringLiteral("/Tasks"));
    if(!file.exists())
    {
        QDir dir(QDir::homePath() + QStringLiteral("/") + QStandardPaths::displayName(QStandardPaths::DocumentsLocation) + QStringLiteral("/"));
        if(!dir.mkpath(QStringLiteral(VER_PRODUCTNAME_STR)))
        {
            showNotify(QStringLiteral("保存任务文件失败"));
            return;
        }
    }
    if(!file.open(QIODevice::WriteOnly))
    {
        showNotify(QStringLiteral("保存任务文件失败"));
        return;
    }
    QDataStream stream(&file);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
    stream.setVersion(QDataStream::Qt_5_13);
#elif (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    in.setVersion(QDataStream::Qt_5_12);
#endif
    stream << g_magicNumber << m_taskList.count();
    foreach (const CTask *task, m_taskList) {
        *task >> stream;
    }
}

void MainWindow::on_action_about_triggered()
{
    const QDate buildDate = QLocale(QLocale::English).toDate( QStringLiteral( __DATE__ ).replace(QStringLiteral("  "), QStringLiteral(" ")), QStringLiteral("MMM dd yyyy"));
    const QTime buildTime = QTime::fromString(QStringLiteral(__TIME__), QStringLiteral("hh:mm:ss"));
    QMessageBox::about(this, QStringLiteral("关于 %0").arg(QApplication::applicationName()),
                       QStringLiteral("<h2> %0</h2>"
                                      "<p>Version: v%1 alpha"
                                      "<p>Author: ZJ"
                                      "<p>Based on Qt %2 (MSVC 2017 64 bit)"
                                      "<p>Built on %3 %4"
//                                      "<p>%5 %6"
                                      "<p>本软件是开源软件，<a href=\"https://github.com/zjun522/AutoDoIt\">代码仓库地址</a>")
                           .arg(QApplication::applicationName(),
                                QApplication::applicationVersion(),
                                QString::number(QT_VERSION_MAJOR) + QStringLiteral(".") + QString::number(QT_VERSION_MINOR) + QStringLiteral(".") + QString::number(QT_VERSION_PATCH),
                                buildDate.toString(QStringLiteral("yyyy/MM/dd")),
                                buildTime.toString(QStringLiteral("hh:mm:ss"))/*,
                                QStringLiteral(VER_LEGALCOPYRIGHT_STR),
                                QStringLiteral(VER_LEGALTRADEMARKS1_STR))*/));
}

void MainWindow::on_action_debug_triggered()
{
}
