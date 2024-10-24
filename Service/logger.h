#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QDir>
#include <QStandardPaths>
#include <QTextEdit>
#include <QThread>
#include <QQueue>

// #define logmsg(msg) Logger::instance().logMessage(LogMessage(Info, msg))
#define logmsg(log) Logger::instance().addLogMsg(log);

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

    struct LogMessage{
        QObject* caller;
        LogLevel level;
        QString msg;
        LogMessage(LogLevel lvl, QString msg) : caller(nullptr), level(lvl), msg(msg) {};
        LogMessage(QObject* caller, LogLevel lvl, QString msg) : caller(caller), level(lvl), msg(msg) {};
    };

    Q_ENUM(LogLevel)

    //база
    static Logger& instance();
    void logMessage(LogMessage log);

    //инициализирующие функции
    void createLogFile();
    void setPath(const QDir &newPath);

    void addLogMsg(QString log);

private:
    Logger();
    ~Logger();

    //мультипоточность
    QMutex mutex;
    QThread logThread;
    bool stopLogging;
    void run();

    //для работы с файлами
    QFile* logFile;
    QTextStream* logStream;
    QDir path;

    //для работы с сообщениями
    void appendTextToWindow(QString text);
    QQueue<LogMessage> logQueue;
    bool consoleOutput;
    QTextEdit* logWindow;    
    int maxLines;
    void processQueue();

    //бог знает зачем
    QString logLevelToString(LogLevel level);


signals:
};



#endif // LOGGER_H
