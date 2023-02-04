#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <ctime>
#include <QString>


class Logger
{
private:
 std::ofstream log_file_;

public:
    Logger(const std::string& file_name);
    void log(const std::string& message);
    void log(const QString& message);
};

extern Logger *logger;

#endif // LOGGER_H
