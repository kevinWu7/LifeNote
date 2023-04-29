#include "calendarcentral.h"


CalendarCentral* CalendarCentral::instance = nullptr;

std::once_flag CalendarCentral::onceFlag;

CalendarCentral::CalendarCentral() {}

CalendarCentral& CalendarCentral::getInstance()
{
    std::call_once(onceFlag, []() {
        instance = new CalendarCentral();
    });
    return *instance;
}

void CalendarCentral::registerGlobalEvent(checkBtnCallback callback)
{
    callbackList.push_back(callback);
}



void CalendarCentral::triggerGlobalEvent(checkin_dateitem *item)
{
    for (checkBtnCallback callback:callbackList)
    {
        callback(item);
    }
}
