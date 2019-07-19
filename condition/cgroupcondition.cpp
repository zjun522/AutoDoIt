#include "cmakefactory.hpp"
#include "cgroupcondition.h"

CGroupCondition::CGroupCondition()
{
    qDebug() << u8"CGroupCondition()" << this;
}

CGroupCondition::CGroupCondition(std::shared_ptr<CCondition> cdt1,
                                 std::shared_ptr<CCondition> cdt2, EAlgType algtype) :
    m_cdt1(std::move(cdt1)),
    m_cdt2(std::move(cdt2)),
    m_alg(algtype)
{
    qDebug() << u8"CGroupCondition()" << this;
}

CGroupCondition::~CGroupCondition()
{
    qDebug() << u8"~CGroupCondition()" << this;
}

QString CGroupCondition::getConditionName() const
{
    QString str;
    if(m_alg == EAlgType::EAT_OR)
    {
        if(m_cdt1 && m_cdt2) {
            str = "(" + m_cdt1->getConditionName() + u8"或" + m_cdt2->getConditionName() + ")";
        }
        else if(m_cdt1) {
            str = m_cdt1->getConditionName();
        }
        else if(m_cdt2) {
            str = m_cdt2->getConditionName();
        }
    }
    else if(m_alg == EAlgType::EAT_AND)
    {
        if(m_cdt1 && m_cdt2) {
            str = "(" + m_cdt1->getConditionName() + u8"且" + m_cdt2->getConditionName() + ")";
        }
        else if(m_cdt1) {
            str = m_cdt1->getConditionName();
        }
        else if(m_cdt2) {
            str = m_cdt2->getConditionName();
        }
    }
    return str;
}

bool CGroupCondition::getStatus() const
{
    if(m_alg == EAlgType::EAT_OR)
    {
        if(m_cdt1 && m_cdt2) {
            return m_cdt1->getStatus() || m_cdt2->getStatus();
        }
        if(m_cdt1) {
            return m_cdt1->getStatus();
        }
        if(m_cdt2) {
            return m_cdt2->getStatus();
        }
    }
    else if(m_alg == EAlgType::EAT_AND)
    {
        if(m_cdt1 && m_cdt2) {
            return m_cdt1->getStatus() && m_cdt2->getStatus();
        }
        if(m_cdt1) {
            return m_cdt1->getStatus();
        }
        if(m_cdt2) {
            return m_cdt2->getStatus();
        }
    }
    return true;
}


QDataStream &CGroupCondition::operator <<(QDataStream &stream)
{
    CCondition::operator <<(stream);
    for(auto i = 0; i < 2; ++i)
    {
        QString className;
        stream >> className;
        if(className == QStringLiteral("NULL")) {
            continue;
        }
        std::shared_ptr<CCondition> cdt = std::static_pointer_cast<CCondition>(CMakeFactory::makeSharedByClassName(className));
        if(cdt){
            *cdt << stream;
            if(i == 0)
                m_cdt1 = cdt;
            else
                m_cdt2 = cdt;
        }
    }
    qint8 alg = 0;
    stream >> alg;
    m_alg = EAlgType(alg);
    return stream;
}

QDataStream &CGroupCondition::operator >>(QDataStream &stream) const
{
    CCondition::operator >>(stream);
    if(m_cdt1)
        *m_cdt1 >> stream;
    else
        stream << QStringLiteral("NULL");
    if(m_cdt2)
        *m_cdt2 >> stream;
    else
        stream << QStringLiteral("NULL");
    stream << static_cast<qint8>(m_alg);
    return stream;
}

std::shared_ptr<CCondition> CGroupCondition::getCdt1() const
{
    return m_cdt1;
}

std::shared_ptr<CCondition> CGroupCondition::getCdt2() const
{
    return m_cdt2;
}

CGroupCondition::EAlgType CGroupCondition::getAlg() const
{
    return m_alg;
}
