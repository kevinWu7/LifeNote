#pragma once
#ifndef BASEINFO_H
#define BASEINFO_H

#define  NODENAME_RECYLE "废纸篓"
#define  NODENAME_COLLECT "收藏"
//xml-node 开头不能用数字，否则报错。使用START_FLAG加在n标题前面，如“蚌2022年计划”来作为替代
#define  START_FLAG "N"
#define  ATTRIBUTE_NOTETYPE "nodetype"
#define  ATTRIBUTE_COLORINDEX "colorindex"
#define  ATTRIBUTE_STARTFLAG "isStartWithDigit"
#define  WINDOW_TITLE "理想之城"

#define NODENAME_TOTALPROJECT "TotalProject"
#define NODENAME_DETAILPROJECT "DetailProject"
#define NODENAME_PROJECT "project"
#define NODENAME_CHECKINDATE "checkin"
#define ATTRIBUTE_NAME "name"
#define ATTRIBUTE_DATE "date"
#define ATTRIBUTE_TIP "tip"
#define ATTRIBUTE_TYPE "type"
#define ATTRIBUTE_ISCHECKED "ischeckd"
#define ATTRIBUTE_ICONINDEX "iconindex"
#define ATTRIBUTE_SELECTED "selected"
#define LINE_COLOR "rgb(239, 239, 239)"

enum OperationType
{
    AddNode,
    AddNodeGroup,
    DeleteNode,
    MoveNode,
    RenameNode,
};


enum HabitOperationType
{
    AddHabit, //添加习惯项目
    RemoveHabit,
    MoveHabit,
    ChangeSelectedHabit,//切换habit
};

enum CheckinType
{
    CheckinAction, //签到打卡
    CheckOutAction, //取消签到
};

enum NodeType
{
    ChildNode,
    ParentNode,
};


#endif // BASEINFO_H
