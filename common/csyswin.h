#ifndef CSYSWIN_H
#define CSYSWIN_H

#include <QObject>
#include <QMutex>
#include <QString>
#include <QMap>
#include <string>
#include <Windows.h>

//获取系统信息的接口
class CSysInfo : public QObject
{
    Q_OBJECT
public:
    explicit CSysInfo(QObject *parent = nullptr) : QObject(parent){}
    ~CSysInfo() override = default;
    Q_DISABLE_COPY_MOVE(CSysInfo)
    //***********************************************************************/
    //函数说明：获取系统CPU当前使用率
    //函数参数：nCpuRate->CPU使用率
    //返回结果：返回结果状态
    //作者时间：Bevis 2014-07-19
    //***********************************************************************/
    virtual bool GetSysCpu(int &nCpuRate) = 0;
    //***********************************************************************/
    //函数说明：获取系统内存信息
    //函数参数：nMemTotal->内存总共大小,(MB)；nMemUsed->内存已使用大小,(MB)
    //返回结果：返回结果状态
    //作者时间：Bevis 2014-07-19
    //***********************************************************************/
    virtual bool GetSysMemory(DWORDLONG &nMemTotal, DWORDLONG &nMemUsed) = 0;
    //***********************************************************************/
    //函数说明：获取系统磁盘信息
    //函数参数：nDiskTotal->磁盘总共大小,(MB)；nDiskUsed->磁盘已使用大小,(MB)
    //返回结果：返回结果状态
    //作者时间：Bevis 2014-07-19
    //***********************************************************************/
    virtual bool GetSysDisk(ulong &nDiskTotal, ulong &nDiskUsed) = 0;
    //***********************************************************************/
    //函数说明：遍历系统所有进程
    //函数参数：无
    //返回结果：进程PID和进程名称的映射集合
    //作者时间：Bevis 2014-07-19
    //***********************************************************************/
    virtual QMap<quint64, QString> GetAllProcess() = 0;
    virtual int killProcess(const QString &exe) =0;
    virtual int killProcess(const DWORD &pId) =0;
    virtual bool processIsRunning(const char *szProcessName) const = 0;
    virtual bool systemShutDown(int delay) = 0;
    virtual bool systemSuspend(int delay, bool hibernate) = 0;
    virtual DWORD getIdleTime() = 0;
    //***********************************************************************/
    //函数说明：获取单个进程的内存使用率
    //函数参数：nPid->进程唯一标识号；nProcessMemRate->进程的内存使用率
    //返回结果：返回结果状态
    //作者时间：Bevis 2014-07-19
    //***********************************************************************/
    virtual bool GetProcessMemory(DWORD nPid, qint64 &nProcessMemRate) = 0;

public slots:
    virtual bool systemShutDown() = 0;
    virtual bool systemSuspend() = 0;
};

class CSysWin: public CSysInfo
{
    Q_OBJECT
public:
    static CSysWin *getInstance(QObject *parent = nullptr);
    static void releaseInstance();

    bool GetSysCpu(int &nCpuRate) override;
    bool GetSysMemory(DWORDLONG &nMemTotal, DWORDLONG& nMemUsed) override;
    bool GetSysDisk(ulong &nDiskTotal, ulong &nDiskUsed) override;
    QMap<quint64,QString> GetAllProcess() override;
    bool GetProcessMemory(DWORD nPid, qint64 &nProcessMemRate) override;
    int killProcess(const QString &exe) override;
    int killProcess(const DWORD &pId) override;
    bool processIsRunning(const char *szProcessName) const override;
    bool systemSuspend(int delay, bool hibernate) override;
    bool systemShutDown(int delay) override;
    DWORD getIdleTime() override;

public slots:
    bool systemSuspend() override;
    bool systemShutDown() override;

protected:
    //时间转换
    int64_t Filetime2Int64(const FILETIME* ftime);
    //两个时间相减运算
    int64_t CompareFileTime(FILETIME preTime,FILETIME nowTime);

    wchar_t* AnsiToUnicode(const char *szStr) const;
    char *UnicodeToAnsi(const wchar_t *szStr) const;
    std::string UnicodeToString(const wchar_t *szStr) const;

private:
    CSysWin(QObject *parent = nullptr);
    ~CSysWin() override;
    Q_DISABLE_COPY_MOVE(CSysWin)

    bool m_hibernate = true;

    static CSysWin *m_instance;
    static QMutex m_mutex;
};

#endif // CSYSWIN_H
