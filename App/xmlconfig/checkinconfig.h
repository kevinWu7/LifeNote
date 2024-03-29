#ifndef CHECKINCONFIG_H
#define CHECKINCONFIG_H

#include <mutex>
#include <QString>
#include <QDate>
#include "baseinfo.h"

#define CHECKIN_CONFIG_PATH "/config/checkin.xml"

enum senderBtn
{
    weekBtn=0,
    monthBtn=1,
};

struct checkin_dateitem
{
    QString project_name;
    QDate date;
    bool ischecked;
    QString tips;
    senderBtn sender;
};

struct project_info
{
    QString project_name;
    QString iconIndex;
    bool selected;
    QString type;
    CheckinRule *rule;
};

struct xmlLoadResult
{
    std::vector<project_info*> project_list;
    std::map<QString,std::vector<checkin_dateitem*>> checkin_map;
};



class CheckinConfig
{
public:
    static CheckinConfig& getInstance();
    xmlLoadResult LoadCheckinConfig();
    void updateHabitXmlInEditMode(HabitOperationType type,project_info * project,project_info * old_project);
    void updateHabitXml(HabitOperationType action,project_info * project);
    void updateDetailXml(CheckinType type,checkin_dateitem* item);


private:
    CheckinConfig();
    CheckinConfig(const CheckinConfig&) = delete;
    CheckinConfig& operator=(const CheckinConfig&) = delete;

    static CheckinConfig* instance;
    static std::once_flag onceFlag;
};

#endif // CHECKINCONFIG_H
