#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QDir>
#include <QStandardPaths>

#define logmsg(msg) Logger::instance().logMessage(Logger::Info, QString(msg) + " " + __FUNCTION__)

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

    void logMessage(QObject* caller, LogLevel level, QString msg);
    void logMessage(LogLevel level, QString msg);
    void createLogFile();

    void setPath(const QDir &newPath);

private:
    Logger();
    ~Logger();

    QFile* logFile;
    QTextStream* logStream;
    QMutex mutex;
    bool consoleOutput;
    QDir path;

    QString logLevelToString(LogLevel level);
signals:
};



#endif // LOGGER_H
