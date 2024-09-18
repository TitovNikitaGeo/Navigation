#include "logger.h"

Logger::Logger() {
consoleOutput = false;
#ifdef QT_DEBUG
    consoleOutput = true;
#endif

    // Создаем окно для вывода логов
    logWindow = new QTextEdit();
    logWindow->setReadOnly(true);  // Окно только для чтения
    logWindow->show();            // Показать окно
    logWindow->move(0, 200);

}

Logger::~Logger()
{
    logStream->flush();
    logFile->close();
}

void Logger::setPath(const QDir &newPath)
{
    path = newPath;
}

Logger &Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::createLogFile()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString fileName = QString("%1_%2_%3_%4.log")
                           .arg(currentDateTime.time().hour())
                           .arg(currentDateTime.time().minute(), 2, 10, QChar('0'))  // Добавляем ведущий ноль для минут
                           .arg(currentDateTime.date().day(),2, 10, QChar('0'))
                           .arg(currentDateTime.date().month(),2, 10, QChar('0'));
    logFile = new QFile(path.absolutePath() + "/"+ fileName);
    logFile->open(QIODevice::WriteOnly);
    logStream = new QTextStream(logFile);

    QMutexLocker locker(&mutex);
}


void Logger::logMessage(QObject *caller, LogLevel level = LogLevel::Debug, QString msg = "")
{
    QMutexLocker locker(&mutex);
    QString callerName = caller->objectName();
    QString log = QString("%1 %2\n%3 %4").arg(callerName)
        .arg(QDateTime::currentDateTime().
            toString("yyyy-MM-dd HH:mm:ss"))
        .arg(logLevelToString(level))
        .arg(msg);
    if (logFile->isOpen()) {
        *logStream << log << "/n";
        logStream->flush();
    }
    if (consoleOutput) {
        qDebug() << log;
    }

    logWindow->setPlainText(log + "\n" + logWindow->toPlainText());
}

void Logger::logMessage(LogLevel level = LogLevel::Debug, QString msg = "")
{
    QMutexLocker locker(&mutex);
    QString log = QString("%1 %2 %3")
        .arg(QDateTime::currentDateTime().
          toString("yyyy-MM-dd HH:mm:ss"))
        .arg(logLevelToString(level))
        .arg(msg);
    if (logFile->isOpen()) {
        *logStream << log << "\n";
        logStream->flush();
    }
    if (consoleOutput) {
        qDebug() << log;
    }
    logWindow->setPlainText(log + "\n" + logWindow->toPlainText());
}

QString Logger::logLevelToString(LogLevel level)
{
    switch (level) {
    case 0:
        return "Debug";
    case 1:
        return "Info";
    case 2:
        return "Warning";
    case 3:
        return "Error";
    default:
        return "Hiring me was an error";
    }
}

