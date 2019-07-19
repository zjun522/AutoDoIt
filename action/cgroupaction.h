#ifndef CGROUPACTION_H
#define CGROUPACTION_H

#include <list>
#include <memory>
#include "caction.h"

class CGroupAction : public CAction
{
    friend class MainWindow;
public:
    CGroupAction();
    ~CGroupAction() override;

    CGroupAction(const CGroupAction &a) = default;
    CGroupAction(CGroupAction &&a) noexcept = default;
    CGroupAction& operator=(const CGroupAction &a) = default;
    CGroupAction& operator=(CGroupAction &&a) noexcept = default;

    QString getActionName() const override;
    bool doIt() override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    void addAction(std::shared_ptr<CAction> act);

    QVector<std::shared_ptr<CAction> > getActionList() const;

private:
    QVector<std::shared_ptr<CAction>> m_actionList;
//    std::list<> m_actionList;
};

#endif // CGROUPACTION_H
