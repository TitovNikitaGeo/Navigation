#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QDir>
#include <QStandardPaths>

class Logger : public QObject
{
    Q_OBJECT
public:
    enum LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };
    Q_ENUM(LogLevel)

    static Logger& instance();

    void log(QObject* caller, LogLevel level, QString msg);
    void log(LogLevel level, QString msg);
private:
    Logger();
    ~Logger();
    void createLogFile();

    QFile* logFile;
    QTextStream* logStream;
    QMutex mutex;
    bool consoleOutput;

    QString logLevelToString(LogLevel level);
signals:
};

#endif // LOGGER_H
