#include "cgroupaction.h"
#include "cmakefactory.hpp"


CGroupAction::CGroupAction()
{
    qDebug() << "CGroupAction()" << this;
}

CGroupAction::~CGroupAction()
{
    qDebug() << "~CGroupAction()" << this;
}

QString CGroupAction::getActionName() const
{
    QString str;
//    for (auto itor = m_actionList.cbegin(); itor != m_actionList.cend(); ++itor)
//    {
//        str += (*itor)->getActionName() + u8"、";
//    }
    foreach (std::shared_ptr<CAction> action, m_actionList) {
        str += action->getActionName() + u8"、";
    }
    str.chop(1);
    return str;
}

bool CGroupAction::doIt()
{
//    for (auto itor = m_actionList.cbegin(); itor != m_actionList.cend(); ++itor)
//    {
//        if(!(*itor)->doIt())
//            return false;
//    }
    foreach (std::shared_ptr<CAction> action, m_actionList) {
        if(!action->doIt())
        {
            m_errorString = action->getErrorString();
            return false;
        }
    }
    return CAction::doIt();
}

void CGroupAction::addAction(std::shared_ptr<CAction> act)
{
//    m_actionList.push_back(std::move(act));
    m_actionList.append(std::move(act));
    if(m_actionList.count() == 1)
        m_icon = m_actionList.at(0)->getIcon();
}

QVector<std::shared_ptr<CAction> > CGroupAction::getActionList() const
{
    return m_actionList;
}

QDataStream &CGroupAction::operator <<(QDataStream &stream)
{
    CAction::operator <<(stream);
    int count = 0;
    stream >> count;
    for(auto i = 0; i < count; ++i)
    {
        QString className;
        stream >> className;
        std::shared_ptr<CAction> action = std::static_pointer_cast<CAction>(CMakeFactory::makeSharedByClassName(className));
        if(action){
            *action << stream;
            addAction(action);
        }
    }
    return stream;
}

QDataStream &CGroupAction::operator >>(QDataStream &stream) const
{
    CAction::operator >>(stream);
    stream << m_actionList.count();
    foreach (std::shared_ptr<CAction> action, m_actionList) {
        *action >> stream;
    }
    return stream;
}
