#ifndef CFILECONDITION_H
#define CFILECONDITION_H

#include "ccondition.h"
#include <QString>

class CFileCondition : public CCondition
{
public:
    CFileCondition();
    CFileCondition(QString fileName);
    ~CFileCondition() override;

    CFileCondition(const CFileCondition &a) = default;
    CFileCondition(CFileCondition &&a) noexcept = default;
    CFileCondition& operator=(const CFileCondition &a) = default;
    CFileCondition& operator=(CFileCondition &&a) noexcept = default;

    QString getConditionName() const override;
    bool getStatus() const override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    QString getFileName() const;

private:
    QString m_fileName;
};

#endif // CFILECONDITION_H
