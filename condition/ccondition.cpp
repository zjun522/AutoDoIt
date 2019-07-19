#include "ccondition.h"

QDataStream &CCondition::operator <<(QDataStream &stream)
{
    return stream;
}

QDataStream &CCondition::operator >>(QDataStream &stream) const
{
    stream << QString(typeid(*this).name());
    return stream;
}


