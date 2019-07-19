#include "csyswin.h"
#include <QDebug>
#include <QTimer>
#include <atlconv.h>
#include <cstring>
#include <TlHelp32.h>
#include <direct.h>
#include <winternl.h>
#include <Psapi.h>
#include <powrprof.h>
//#pragma comment(lib,"Kernel32.lib")
//#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"PowrProf.lib")

CSysWin *CSysWin::m_instance = nullptr;
QMutex CSysWin::m_mutex;

CSysWin::CSysWin(QObject *parent) : CSysInfo(parent)
{
}

CSysWin::~CSysWin() = default;

CSysWin *CSysWin::getInstance(QObject *parent)
{
    if(m_instance == nullptr)
    {
        QMutexLocker locker(&m_mutex);
        if(m_instance == nullptr)
        {
            m_instance = new CSysWin(parent);
        }
    }
    return m_instance;
}

void CSysWin::releaseInstance()
{
    if(m_instance)
    {
        QMutexLocker locker(&m_mutex);
        if(m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }
}

int64_t CSysWin::Filetime2Int64(const FILETIME* ftime)
{
    LARGE_INTEGER li;
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = static_cast<LONG>(ftime->dwHighDateTime);
    return li.QuadPart;
}

int64_t CSysWin::CompareFileTime(FILETIME preTime,FILETIME nowTime)
{
    return this->Filetime2Int64(&nowTime) - this->Filetime2Int64(&preTime);
}

//将单字节char*转化为宽字节wchar_t*
inline wchar_t* CSysWin::AnsiToUnicode( const char* szStr ) const
{
    int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, nullptr, 0 );
    if (nLen == 0) {
        return nullptr;
    }
    auto *pResult = new wchar_t[static_cast<uint>(nLen)];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
    return pResult;
}

//将宽字节wchar_t*转化为单字节char*
inline char* CSysWin::UnicodeToAnsi( const wchar_t* szStr ) const
{
    int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, nullptr, 0, nullptr, nullptr );
    if (nLen == 0) {
        return nullptr;
    }
    char* pResult = new char[static_cast<uint>(nLen)];
    WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, nullptr, nullptr );
    return pResult;
}

inline std::string CSysWin::UnicodeToString( const wchar_t* szStr) const
{
    int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, nullptr, 0, nullptr, nullptr );
    if (nLen == 0) {
        return std::string();
    }
    std::string result(static_cast<std::string::size_type>(nLen), 0);
    WideCharToMultiByte( CP_ACP, 0, szStr, -1, const_cast<LPSTR> (result.c_str()), nLen, nullptr, nullptr );
    return result;
}

bool CSysWin::GetSysCpu(int &nCpuRate)
{
    HANDLE hEvent;
    bool ret;
    static FILETIME preIdleTime;
    static FILETIME preKernelTime;
    static FILETIME preUserTime;

    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    ret = GetSystemTimes(&idleTime, &kernelTime, &userTime);
    if(!ret)
        return FALSE;

    preIdleTime = idleTime;
    preKernelTime = kernelTime;
    preUserTime = userTime;

    hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr); //初始值为nonsignaled
    if(hEvent) {
        WaitForSingleObject(hEvent, 500); //等待500毫秒
        CloseHandle(hEvent);
    }

    ret = GetSystemTimes(&idleTime, &kernelTime, &userTime);
    if(!ret) {
        return FALSE;
    }

    int64_t idle = CompareFileTime(preIdleTime,idleTime);
    int64_t kernel = CompareFileTime(preKernelTime,kernelTime);
    int64_t user = CompareFileTime(preUserTime,userTime);

    nCpuRate = static_cast<int>(ceil( 100.0*( kernel + user - idle ) / ( kernel + user ) ));
    return ret;
}

bool CSysWin::GetSysMemory(DWORDLONG &nMemTotal, DWORDLONG &nMemUsed)
{
    MEMORYSTATUSEX memsStat;
    memsStat.dwLength = sizeof(memsStat);
    if(!GlobalMemoryStatusEx(&memsStat))//如果获取系统内存信息不成功，就直接返回
    {
        nMemTotal = 0;
        nMemUsed  = 0;
        return false;
    }
    DWORDLONG nMemFree = memsStat.ullAvailPhys/(1024*1024);
    nMemTotal = memsStat.ullTotalPhys/(1024*1024);
    nMemUsed = nMemTotal- nMemFree;
    return true;
}

bool CSysWin::GetSysDisk(ulong &nDiskTotal, ulong &nDiskUsed)
{
    static char path[_MAX_PATH];//存储当前系统存在的盘符
//    int curdrive = _getdrive();
    ULONG lFreeAll = 0UL;
    ULONG lTotalAll = 0UL;
    for(int drive = 1; drive <= 26; drive++ )//遍历所有盘符
    {
        if( !_chdrive( drive ) )
        {
            ULARGE_INTEGER caller, total, free;
            WCHAR wszClassName[_MAX_PATH];
            memset(wszClassName, 0, sizeof(wszClassName));
            MultiByteToWideChar(CP_ACP, 0, path, static_cast<int>(strlen(path) + 1), wszClassName,
                sizeof(wszClassName)/sizeof(wszClassName[0]));
            if (GetDiskFreeSpaceEx(wszClassName, &caller, &total, &free) == 0)
            {
                qDebug()<<"GetDiskFreeSpaceEx Filed!";
                return false;
            }

            DOUBLE dTepFree = free.QuadPart/( 1024.0*1024.0 );
            DOUBLE dTepTotal = total.QuadPart/( 1024.0*1024.0 );
            //qDebug()<<"Get Windows Disk Information:"<<path<<"--free:"<<dTepFree<<"--total:"<<dTepTotal;
            lFreeAll += static_cast<unsigned long>(ceil(dTepFree));
            lTotalAll += static_cast<unsigned long>(ceil(dTepTotal));
        }
    }
    nDiskUsed = lFreeAll;
    nDiskTotal= lTotalAll;
    return true;
}

QMap<quint64, QString> CSysWin::GetAllProcess()
{
    HANDLE hProcessSnap;	//进程快照的句柄
//    HANDLE hProcess;	//用于获取进程的句柄
    PROCESSENTRY32 pe32;//进程信息的结构体
    QMap<quint64, QString> pidMap;//保存所有进程的PID
    // 获取系统进程信息的快照
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
    {
        qDebug() << "CreateToolhelp32Snapshot Failed!";// 打印错误信息
        if(nullptr !=hProcessSnap)
        {
            CloseHandle( hProcessSnap );          // 关闭进程快照信息
            hProcessSnap = nullptr;
        }
        return pidMap;
    }
    // 在使用之前设置PROCESSENTRY32结构体的初始大小值,如果不初始化dwSize, Process32First 调用会失败.
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    if( !Process32First( hProcessSnap, &pe32 ) )// 开始获取第一个进程的信息，如果获取失败就返回
    {
        qDebug()<<"Process32First Failed!"; // 打印错误信息
        if(nullptr !=hProcessSnap)
        {
            CloseHandle( hProcessSnap );          // 关闭进程快照信息
            hProcessSnap = nullptr;
        }
        return pidMap;
    }
    //开始遍历所有进程
    pidMap.clear();
    do
    {
        //加入PID
        if(!pidMap.contains(pe32.th32ProcessID))
        {
            std::string c = UnicodeToString(pe32.szExeFile);
            if(!c.empty())
                pidMap.insert(pe32.th32ProcessID, QString::fromStdString(c));
        }

        //当然还可以获取到很多其他信息，例如进程名字(szExeFile[MAX_PATH])、父进程PPID(th32ParentProcessID)。。。
        /* 附上该结构体信息
        typedef struct tagPROCESSENTRY32 {
        DWORD     dwSize;
        DWORD     cntUsage;
        DWORD     th32ProcessID;
        ULONG_PTR th32DefaultHeapID;
        DWORD     th32ModuleID;
        DWORD     cntThreads;
        DWORD     th32ParentProcessID;
        LONG      pcPriClassBase;
        DWORD     dwFlags;
        TCHAR     szExeFile[MAX_PATH];
        } PROCESSENTRY32, *PPROCESSENTRY32;
        */
    }
    while( Process32Next( hProcessSnap, &pe32 ) );// 获取下一个进程的信息
    if(nullptr !=hProcessSnap)//最后关闭快照句柄
    {
        CloseHandle( hProcessSnap );
        hProcessSnap = nullptr;
    }
    return pidMap;
}

int CSysWin::killProcess(const QString& exe)
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return -1;
    }

    bool bRet = FALSE;
    DWORD dwPid = 0;
    while (Process32Next(hProcessSnap, &pe32))
    {
        //将WCHAR转成const char*
        int iLn = WideCharToMultiByte(CP_UTF8, 0, const_cast<LPWSTR> (pe32.szExeFile), static_cast<int>(sizeof(pe32.szExeFile)), nullptr, 0, nullptr, nullptr);
        std::string result(static_cast<std::string::size_type>(iLn), 0);
        WideCharToMultiByte (CP_UTF8, 0, pe32.szExeFile, static_cast<int>(sizeof(pe32.szExeFile)), const_cast<LPSTR> (result.c_str()), iLn, nullptr, nullptr);
        if (0 == strcmp(exe.toStdString().c_str(), result.c_str()))
        {
            dwPid = pe32.th32ProcessID;
            bRet = TRUE;
            break;
        }
    }
    if(bRet == FALSE)
        return -1;
    CloseHandle(hProcessSnap);
    return killProcess(dwPid);
}

int CSysWin::killProcess(const DWORD &pId)
{
    HANDLE hProcess=nullptr;
    //打开目标进程
    hProcess = OpenProcess(PROCESS_TERMINATE,FALSE, pId);
    if (hProcess == nullptr) {
        qDebug() << "Open Process fAiled, error:" << GetLastError();
        return -1;
    }
    //结束目标进程
    bool ret = TerminateProcess(hProcess, 0);
    if(!ret) {
        qDebug() << "Kill task faild, error:" << GetLastError();
        return -1;
    }
    return 0;
}

bool CSysWin::processIsRunning(const char* szProcessName) const
{
    PROCESSENTRY32 processEntry32;
    HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (toolHelp32Snapshot)
    {
        processEntry32.dwSize = sizeof(processEntry32);
        if (Process32First(toolHelp32Snapshot, &processEntry32))
        {
            do
            {
                //转换成功返回为非负值
//                char* chRtn = UnicodeToAnsi(processEntry32.szExeFile); //zhangjun modified
                std::string strRtn = UnicodeToString(processEntry32.szExeFile);
                if(strRtn.empty())
                    continue;
                if (strcmp(szProcessName, strRtn.c_str()) == 0) {
                    return TRUE;
                }
            }
            while (Process32Next(toolHelp32Snapshot, &processEntry32));
        }
        CloseHandle(toolHelp32Snapshot);
    }
    return FALSE;
}

bool CSysWin::GetProcessMemory(DWORD nPid, qint64& nProcessMemRate)
{
    HANDLE hProcess;//该线程的句柄
    PROCESS_MEMORY_COUNTERS pmc;//该线程的内存信息结构体
    hProcess = OpenProcess( PROCESS_ALL_ACCESS ,FALSE, nPid );//利用最大权限打开该线程并获得句柄
    if (nullptr == hProcess)
    {
        qDebug() << "OpenProcess Failed!";
        return false;
    }
    if ( !GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
    {
        qDebug() << "GetProcessMemoryInfo Failed!";
        return false;
    }

    DWORDLONG nMemTotal = 0;
    DWORDLONG nMemUsed = 0;
    this->GetSysMemory(nMemTotal, nMemUsed);
    double tep = (pmc.WorkingSetSize * 1.0);
    nProcessMemRate = static_cast<qint64>(ceil(100*(tep/(1024.0*1024.0)) / nMemTotal));

    CloseHandle(hProcess);

    return true;
}

bool CSysWin::systemShutDown()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    //获取进程标志
    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken))
        return false;
    //获取关机特权的LUID
    LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    //获取这个进程的关机特权
//    AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)nullptr, nullptr);
    AdjustTokenPrivileges(hToken, false, &tkp, 0, nullptr, nullptr);
    if(GetLastError() != ERROR_SUCCESS)
        return false;
    //强制关闭计算机
    wchar_t *pResult = AnsiToUnicode("Auto Do It: 计算机将关机");
    BOOL ret = InitiateSystemShutdownExW(nullptr, pResult, 20, false, false, 0);
    delete [] pResult;
    return ret != 0;
    //return ExitWindowsEx(EWX_SHUTDOWN, 0) != 0;
    //    return ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0) != 0;
}

bool CSysWin::systemShutDown(int delay)
{
    QTimer::singleShot(delay, this, SLOT(systemShutDown()));
    return true;
}

DWORD CSysWin::getIdleTime() //return milliseconds
{
    LASTINPUTINFO plii;
    plii.cbSize = sizeof(LASTINPUTINFO);
    if(GetLastInputInfo(&plii) != 0)
    {
        return GetTickCount() - plii.dwTime;
    }
    return 0;
}

bool CSysWin::systemSuspend(int delay, bool hibernate)
{
    m_hibernate = hibernate;
    QTimer::singleShot(delay, this, SLOT(systemSuspend()));
    return true;
}

bool CSysWin::systemSuspend()
{
    qDebug() << m_hibernate;
    return SetSuspendState(m_hibernate, false, false) != 0;
}
