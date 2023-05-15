#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <ctime>
#include <QString>
#include <QTextEdit> //应该通过回掉方式，但是这种方式需要传入一个公共方法(不属于任何对象）


class Logger
{
private:
 std::ofstream log_file_;


public:
    Logger(const std::string& file_name);
    void log(const std::string& message);
    void log(const QString& message);
    QTextEdit * logEdit=nullptr;
};

extern Logger *logger;

#endif // LOGGER_H
