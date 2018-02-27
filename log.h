#ifndef LOG_H
#define LOG_H

#include "pch.h"

class Log : public QTextBrowser
{
    Q_OBJECT

public:
    explicit Log(QWidget* widget);
    ~Log() override = default;

    void accent(const QString& header, const QString& body);
    void error(const QString& header, const QString& body);
    void info(const QString& header, const QString& body);
    void normal(const QString& header, const QString& body);

private:
    void showMsg(const QString& header, const QString& body, const QColor& color);
};

#endif // LOG_H
