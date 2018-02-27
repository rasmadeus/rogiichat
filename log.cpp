#include "log.h"

Log::Log(QWidget* widget)
    : QTextBrowser{ widget }
{
    setReadOnly(true);
}

void Log::accent(const QString &header, const QString &body)
{
    showMsg(header, body, Qt::gray);
}

void Log::error(const QString& header, const QString& body)
{
    showMsg(header, body, Qt::red);
}

void Log::info(const QString& header, const QString& body)
{
    showMsg(header, body, Qt::blue);
}

void Log::normal(const QString& header, const QString& body)
{
    showMsg(header, body, Qt::black);
}

void Log::showMsg(const QString& header, const QString& body, const QColor& color)
{
    static const auto pattern = QString{"<font color=\"%1\"><b>[%2] %3:</b> %4</font>"};
    append(pattern
        .arg(color.name())
        .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
        .arg(header)
        .arg(body)
    );
}
