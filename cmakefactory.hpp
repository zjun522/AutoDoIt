#ifndef CMAKEFACTORY_H
#define CMAKEFACTORY_H

#include <QString>
#include "action/cgroupaction.h"
#include "action/cstartstopprocessaction.h"
#include "action/cpoweroffaction.h"
#include "action/cmessageaction.h"
#include "action/cexitautodoitaction.h"
#include "action/cstartstoptaskaction.h"
#include "trigger/cgrouptrigger.h"
#include "trigger/ctimertrigger.h"
#include "trigger/cidletrigger.h"
#include "trigger/cudptrigger.h"
#include "trigger/chttptrigger.h"
#include "trigger/cwatchfiletrigger.h"
#include "trigger/cwatchprocesstrigger.h"
#include "condition/cgroupcondition.h"
#include "condition/ctimecondition.h"
#include "condition/cgroupcondition.h"
#include "condition/cfilecondition.h"
#include "condition/cidlecondition.h"
#include "condition/cprocesscondition.h"

class CMakeFactory
{
public:
    CMakeFactory() = delete;

    template<typename T, typename... Ts>
    static auto makePtr(Ts&&... params)
    {
        return new T(std::forward<Ts>(params)...);
    }

    template<typename T, typename... Ts>
    static auto makeUnique(Ts&&... params)
    {
        std::unique_ptr<T> ptr(nullptr);
        ptr.reset(new T(std::forward<Ts>(params)...));
        return ptr;
    }

    static auto makeSharedByClassName(const QString &className) {
        std::shared_ptr<void> ptr(nullptr);
        if(className == QString(typeid(CGroupAction).name()))
        {
            ptr = CMakeFactory::makeUnique<CGroupAction>();
        }
        else if(className == QString(typeid(CPowerOffAction).name()))
        {
            ptr = CMakeFactory::makeUnique<CPowerOffAction>();
        }
        else if(className == QString(typeid(CStartStopProcessAction).name()))
        {
            ptr = CMakeFactory::makeUnique<CStartStopProcessAction>();
        }
        else if(className == QString(typeid(CMessageAction).name()))
        {
            ptr = CMakeFactory::makeUnique<CMessageAction>();
        }
        else if(className == QString(typeid(CStartStopTaskAction).name()))
        {
            ptr = CMakeFactory::makeUnique<CStartStopTaskAction>();
        }
        else if(className == QString(typeid(CExitAutoDoItAction).name()))
        {
            ptr = CMakeFactory::makeUnique<CExitAutoDoItAction>();
        }
        else if(className == QString(typeid(CGroupTrigger).name()))
        {
            ptr = CMakeFactory::makeUnique<CGroupTrigger>();
        }
        else if(className == QString(typeid(CTimerTrigger).name()))
        {
            ptr = CMakeFactory::makeUnique<CTimerTrigger>();
        }
        else if(className == QString(typeid(CIdleTrigger).name()))
        {
            ptr = CMakeFactory::makeUnique<CIdleTrigger>();
        }
        else if(className == QString(typeid(CUDPTrigger).name()))
        {
            ptr = CMakeFactory::makeUnique<CUDPTrigger>();
        }
        else if(className == QString(typeid(CHTTPTrigger).name()))
        {
            ptr = CMakeFactory::makeUnique<CHTTPTrigger>();
        }
        else if(className == QString(typeid(CWatchFileTrigger).name()))
        {
            ptr = CMakeFactory::makeUnique<CWatchFileTrigger>();
        }
        else if(className == QString(typeid(CWatchProcessTrigger).name()))
        {
            ptr = CMakeFactory::makeUnique<CWatchProcessTrigger>();
        }
        else if(className == QString(typeid(CGroupCondition).name()))
        {
            ptr = CMakeFactory::makeUnique<CGroupCondition>();
        }
        else if(className == QString(typeid(CFileCondition).name()))
        {
            ptr = CMakeFactory::makeUnique<CFileCondition>();
        }
        else if(className == QString(typeid(CIdleCondition).name()))
        {
            ptr = CMakeFactory::makeUnique<CIdleCondition>();
        }
        else if(className == QString(typeid(CProcessCondition).name()))
        {
            ptr = CMakeFactory::makeUnique<CProcessCondition>();
        }
        else if(className == QString(typeid(CTimeCondition).name()))
        {
            ptr = CMakeFactory::makeUnique<CTimeCondition>();
        }
        return ptr;
    }

    static auto makePtrByClassName(const QString &className){
        void *ptr = nullptr;
        if(className == QString(typeid(CGroupAction).name()))
        {
            ptr = CMakeFactory::makePtr<CGroupAction>();
        }
        else if(className == QString(typeid(CPowerOffAction).name()))
        {
            ptr = CMakeFactory::makePtr<CPowerOffAction>();
        }
        else if(className == QString(typeid(CStartStopProcessAction).name()))
        {
            ptr = CMakeFactory::makePtr<CStartStopProcessAction>();
        }
        else if(className == QString(typeid(CMessageAction).name()))
        {
            ptr = CMakeFactory::makePtr<CMessageAction>();
        }
        else if(className == QString(typeid(CStartStopTaskAction).name()))
        {
            ptr = CMakeFactory::makePtr<CStartStopTaskAction>();
        }
        else if(className == QString(typeid(CExitAutoDoItAction).name()))
        {
            ptr = CMakeFactory::makePtr<CExitAutoDoItAction>();
        }
        else if(className == QString(typeid(CGroupTrigger).name()))
        {
            ptr = CMakeFactory::makePtr<CGroupTrigger>();
        }
        else if(className == QString(typeid(CTimerTrigger).name()))
        {
            ptr = CMakeFactory::makePtr<CTimerTrigger>();
        }
        else if(className == QString(typeid(CIdleTrigger).name()))
        {
            ptr = CMakeFactory::makePtr<CIdleTrigger>();
        }
        else if(className == QString(typeid(CUDPTrigger).name()))
        {
            ptr = CMakeFactory::makePtr<CUDPTrigger>();
        }
        else if(className == QString(typeid(CHTTPTrigger).name()))
        {
            ptr = CMakeFactory::makePtr<CHTTPTrigger>();
        }
        else if(className == QString(typeid(CWatchFileTrigger).name()))
        {
            ptr = CMakeFactory::makePtr<CWatchFileTrigger>();
        }
        else if(className == QString(typeid(CWatchProcessTrigger).name()))
        {
            ptr = CMakeFactory::makePtr<CWatchProcessTrigger>();
        }
        else if(className == QString(typeid(CGroupCondition).name()))
        {
            ptr = CMakeFactory::makePtr<CGroupCondition>();
        }
        else if(className == QString(typeid(CFileCondition).name()))
        {
            ptr = CMakeFactory::makePtr<CFileCondition>();
        }
        else if(className == QString(typeid(CIdleCondition).name()))
        {
            ptr = CMakeFactory::makePtr<CIdleCondition>();
        }
        else if(className == QString(typeid(CProcessCondition).name()))
        {
            ptr = CMakeFactory::makePtr<CProcessCondition>();
        }
        else if(className == QString(typeid(CTimeCondition).name()))
        {
            ptr = CMakeFactory::makePtr<CTimeCondition>();
        }
        return ptr;
    }
};

#endif // CMAKEFACTORY_H
