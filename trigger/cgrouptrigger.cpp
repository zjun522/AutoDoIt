#include "cgrouptrigger.h"

#include "cmakefactory.hpp"
#include <QDebug>

CGroupTrigger::CGroupTrigger(ERepeatMode repeatMode,
                             std::shared_ptr<CAction> action,
                             std::shared_ptr<CCondition> condition, QString taskName,
                             QObject *parent) :
    CTrigger(repeatMode, false, std::move(action), std::move(condition), std::move(taskName), parent)
{
    qDebug() << "CGroupTrigger()" << this;
}

CGroupTrigger::~CGroupTrigger()
{
    qDebug() << "~CGroupTrigger()" << this;
}

QString CGroupTrigger::getTriggerName() const
{
    QString str;
//    auto beg = m_triggers.begin();
//    while(beg != m_triggers.end())
//    {
//        str += (*beg)->getTriggerName() + u8"或";
//        beg++;
//    }
    foreach (const CTrigger *trigger, m_triggers) {
        str += trigger->getTriggerName() + u8"或";
    }
    str.chop(1);
    return str;
}

void CGroupTrigger::addTrigger(CTrigger *trig)
{
    trig->setParent(this);
    connect(trig, SIGNAL(trigger()), this, SIGNAL(trigger()));
//    m_triggers.push_back(trig);
    m_triggers.append(trig);
}

QVector<CTrigger *> CGroupTrigger::getTriggers() const
{
    return m_triggers;
}

void CGroupTrigger::startWatch()
{
    //    auto beg = m_triggers.begin();
    //    while(beg != m_triggers.end())
//    {
//        (*beg)->startWatch();
//        beg++;
//    }
    CTrigger::startWatch();
    foreach (CTrigger *trigger, m_triggers) {
        trigger->startWatch();
    }
}

void CGroupTrigger::stopWatch()
{
//    auto beg = m_triggers.begin();
//    while(beg != m_triggers.end())
//    {
//        (*beg)->stopWatch();
//        beg++;
//    }
    foreach (CTrigger *trigger, m_triggers) {
        trigger->stopWatch();
    }
    CTrigger::stopWatch();
}

QString CGroupTrigger::getTipMsg()
{
    CTrigger::getTipMsg();
    QString str;
    foreach (CTrigger *trigger, m_triggers) {
        const QString &tip = trigger->getTipMsg();
        if(!tip.isEmpty())
            str += tip + "\n";
    }
    if(!str.isEmpty())
        str.chop(1);

    if(!m_tipMsg.isEmpty() && !str.isEmpty())
        m_tipMsg = m_tipMsg + "\n" + str;
    else if(m_tipMsg.isEmpty())
        m_tipMsg = str;
    return m_tipMsg;
}

bool CGroupTrigger::getIsWatch() const
{
    foreach (const CTrigger *trigger, m_triggers) {
        if(trigger->getIsWatch())
            return true;
    }
    return false;
}

QDataStream &CGroupTrigger::operator <<(QDataStream &stream)
{
    CTrigger::operator <<(stream);
    int count = 0;
    stream >> count;
    for(auto i = 0; i < count; ++i)
    {
        QString className;
        stream >> className;
        auto *trigger = static_cast<CTrigger *>(CMakeFactory::makePtrByClassName(className));
        if(trigger)
        {
            *trigger << stream;
            addTrigger(trigger);
        }
    }
    return stream;
}

QDataStream &CGroupTrigger::operator >>(QDataStream &stream) const
{
    CTrigger::operator >>(stream);
    stream << m_triggers.size();
    foreach (const CTrigger *trigger, m_triggers) {
        *trigger >> stream;
    }
    return stream;
}
