#include "cfilecondition.h"
#include <QFileInfo>
#include <utility>

CFileCondition::CFileCondition()
{
    qDebug() << "CFileCondition" << this;
}

CFileCondition::CFileCondition(QString fileName) :
    m_fileName(std::move(fileName))
{
    qDebug() << "CFileCondition" << this;
}

CFileCondition::~CFileCondition()
{
    qDebug() << "~CFileCondition" << this;
}

QString CFileCondition::getConditionName() const
{
    return QStringLiteral("文件“%1”存在").arg(QFileInfo(m_fileName).fileName());
}

bool CFileCondition::getStatus() const
{
    QFileInfo fileInfo(m_fileName);
    return fileInfo.exists();
}

QDataStream &CFileCondition::operator <<(QDataStream &stream)
{
    CCondition::operator <<(stream);
    stream >> m_fileName;
    return stream;
}

QDataStream &CFileCondition::operator >>(QDataStream &stream) const
{
    CCondition::operator >>(stream);
    stream << m_fileName;
    return stream;
}

QString CFileCondition::getFileName() const
{
    return m_fileName;
}
