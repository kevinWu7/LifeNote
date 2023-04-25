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

#define NODENAME_TOTALPROJECT "TotalProjet"
#define NODENAME_DETAILPROJECT "DetailProject"
#define NODENAME_PROJECT "project"
#define ATTRIBUTE_NAME "name"
#define ATTRIBUTE_DATE "date"
#define ATTRIBUTE_TIP "tip"
#define ATTRIBUTE_TYPE "type"
#define ATTRIBUTE_ICONINDEX "iconindex"

enum OperationType
{
    AddNode,
    AddNodeGroup,
    DeleteNode,
    MoveNode,
    RenameNode,
};

enum NodeType
{
    ChildNode,
    ParentNode,
};


#endif // BASEINFO_H
