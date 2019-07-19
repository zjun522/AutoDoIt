#include "cwatchfiletrigger.h"
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <utility>
#include <utility>

CWatchFileTrigger::CWatchFileTrigger(QObject *parent):
    CTrigger(parent)
{
    qDebug() << "CWatchFileTrigger()" << this;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(scanFile()));
}

CWatchFileTrigger::CWatchFileTrigger(QString fileName, int duration,
                                     ERepeatMode repeatMode,
                                     std::shared_ptr<CAction> action,
                                     std::shared_ptr<CCondition> condition,
                                     QObject *parent) :
    CTrigger(repeatMode, false, std::move(action), std::move(condition), parent),
    m_duration(duration),
    m_fileName(std::move(fileName))
{
    qDebug() << "CWatchFileTrigger()" << this;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(scanFile()));
}

CWatchFileTrigger::~CWatchFileTrigger()
{
    qDebug() << "~CWatchFileTrigger()" << this;
}

QString CWatchFileTrigger::getTriggerName() const
{
    return QStringLiteral("文件“%1”存在时").arg(QFileInfo(m_fileName).fileName());
}

void CWatchFileTrigger::startWatch()
{
    CTrigger::startWatch();
    m_timer.start(m_duration);
}

void CWatchFileTrigger::stopWatch()
{
    m_timer.stop();
    CTrigger::stopWatch();
}

QDataStream &CWatchFileTrigger::operator<<(QDataStream &stream)
{
    CTrigger::operator<<(stream);
    stream >> m_duration >> m_fileName;
    return stream;
}

QDataStream &CWatchFileTrigger::operator>>(QDataStream &stream) const
{
    CTrigger::operator>>(stream);
    stream << m_duration << m_fileName;
    return stream;
}

void CWatchFileTrigger::triggered()
{
//触发后，时候删除文件
    QFileInfo fileInfo(m_fileName);
    if(fileInfo.exists())
    {
        if(fileInfo.isDir())
        {
            QDir dir(m_fileName);
            if(!dir.removeRecursively())
            {
                m_errorString = tr(u8"删除文件错误");
            }
        }
        else if(fileInfo.isFile())
        {
            QFile file(m_fileName);
            if(!file.remove())
            {
                m_errorString = tr(u8"删除文件错误");
            }
        }
    }
    CTrigger::triggered();
}

void CWatchFileTrigger::scanFile()
{
    QFileInfo fileInfo(m_fileName);
    if(fileInfo.exists() && fileInfo.lastModified().secsTo(QDateTime::currentDateTime()) <= m_duration &&
            fileInfo.lastModified().secsTo(QDateTime::currentDateTime()) >= 0)
    {
        triggered();
    }
}

QString CWatchFileTrigger::getFileName() const
{
    return m_fileName;
}

