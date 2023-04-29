#ifndef CALENDARCENTRAL_H
#define CALENDARCENTRAL_H

#include <mutex>
#include "checkinconfig.h"

typedef std::function<void(checkin_dateitem*)> checkBtnCallback;

class CalendarCentral
{
public:
    static CalendarCentral& getInstance();
    // 注册按钮点击全局事件
    void registerGlobalEvent(checkBtnCallback callback);
    //void unregisterGlobalEvent(checkBtnCallback callback);
    // 触发按钮点击全局事件
    void triggerGlobalEvent(checkin_dateitem * dateItem);

    std::vector<checkBtnCallback> callbackList ;
private:
    CalendarCentral();
    CalendarCentral(const CalendarCentral&) = delete;
    CalendarCentral& operator=(const CalendarCentral&) = delete;

    static CalendarCentral* instance;
    static std::once_flag onceFlag;
};


#endif // CALENDARCENTRAL_H
