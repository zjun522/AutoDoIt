#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class CListModel;
class CTrigger;
using CTask = CTrigger;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() final;
    Q_DISABLE_COPY_MOVE(MainWindow)

    void showNotify(const QString &msg);

public slots:
    void on_action_stopAllTask_triggered();
    void on_action_startAllTask_triggered();
    void on_action_quit_triggered();

private slots:
    void sysTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void on_action_addTask_triggered();
    void on_action_removeTask_triggered();
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_listView_customContextMenuRequested(const QPoint &point);
    void on_action_startTask_triggered();
    void on_action_editTask_triggered();
    void on_listView_clicked(const QModelIndex &);
    void on_action_stopTask_triggered();
    void on_action_about_triggered();
    void on_action_autoStart_triggered(bool checked);

    void on_action_doTask_triggered();

    void on_action_debug_triggered();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_sysTray = nullptr;
    QMenu *m_sysTrayMenu = nullptr;
    CListModel *m_model = nullptr;
    QMenu *m_menu = nullptr;
    QList<CTask *> m_taskList;

    void initSysTray();
    bool getIsAutoStart();
    void updateAction();
    void restoreTask();
    void saveTask();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
