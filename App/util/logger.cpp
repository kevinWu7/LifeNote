#include <QDebug>
#include "logger.h"


Logger *logger=new Logger("log.txt");

Logger::Logger(const std::string& file_name) : log_file_(file_name, std::ios::app)
{

}


void Logger::log(const std::string& message)
{
    time_t now = time(0);
    char* dt = ctime(&now);
    dt[strlen(dt)-1]=0; //去掉最后一个 '\n' 换行符
    std::string logStr=dt+std::string(":")+message;
    qDebug()<< QString::fromStdString(logStr);
    log_file_ << logStr<< std::endl;
    if(logEdit!=nullptr)
    {
        logEdit->append(QString::fromStdString(logStr));
    }
}

void Logger::log(const QString& message)
{
    time_t now = time(0);
    char* dt = ctime(&now);
    dt[strlen(dt)-1]=0; //去掉最后一个 '\n' 换行符
    QString str= QString(dt)+QString(":")+message;
    qDebug()<<str;
    log_file_ << str.toStdString()<<std::endl;
    if(logEdit!=nullptr)
    {
        logEdit->append(str);
    }
}

