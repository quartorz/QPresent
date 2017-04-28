#include "simpleserver.h"

#include <QRegularExpression>
#include <QDir>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDateTime>
#include <QTcpSocket>
#include <QElapsedTimer>
#include <QThread>

namespace {
    QRegularExpression statusLineRegExp("([^ ]+) (?:https?://[^/]+)?/([^?]*)[^ ]* .*");

    void writeResponse(QTcpSocket *socket, const QFileInfo &info)
    {
        if (info.exists()) {
            auto filePath = info.absoluteFilePath();
            QFile file(filePath);

            file.open(QFile::ReadOnly);

            if (!file.isReadable()) {
                auto message = QString("\"%1\" cannot be accessed").arg(filePath).toUtf8();

                socket->write("HTTP/1.1 403 Forbidden\r\n");
                socket->write(QString("Content-Length: %1\r\n\r\n").arg(message.size()).toUtf8());
                socket->write(message);

                return;
            }

            QMimeDatabase md;
            QDateTime dateTime = QDateTime(info.lastModified()).toTimeSpec(Qt::UTC);
            QLocale locale(QLocale::C);
            QString date = locale.toString(dateTime, "ddd, dd MMM yyyy");
            date += dateTime.toString(" hh:mm:ss");
            date += " GMT";

            socket->write("HTTP/1.1 200 OK\r\n");
            socket->write(QString("Content-Type: %1\r\n")
                          .arg(md.mimeTypeForFile(filePath).name()).toUtf8());
            socket->write(QString("Content-Length: %1\r\n")
                          .arg(file.size()).toUtf8());
            socket->write(QString("Last-Modified: %1\r\n\r\n").arg(date).toUtf8());
            socket->write(file.readAll());
        } else {
            auto message = QString("\"%1\" not found").arg(info.absoluteFilePath()).toUtf8();

            socket->write("HTTP/1.1 404 Not Found\r\n");
            socket->write(QString("Content-Length: %1\r\n\r\n").arg(message.size()).toUtf8());
            socket->write(message);
        }
    }
}

SimpleServer::SimpleServer()
{
    connect(this, &QTcpServer::newConnection, [&]() {
        auto c = this->nextPendingConnection();
        connect(c, &QTcpSocket::readyRead, [&, c]() {
            QElapsedTimer timer;

            timer.start();

            while (!c->canReadLine() && timer.hasExpired(1000)) {
                QThread::msleep(1);
            }

            if (c->canReadLine()) {
                parseRequestLine(c);
            }

            c->close();
        });
    });
}

void SimpleServer::setDocumentRoot(const QString &root)
{
    this->root = root;
}

void SimpleServer::parseRequestLine(QTcpSocket *connection)
{
    QString line = connection->readLine();
    auto match = statusLineRegExp.match(line);

    if (match.captured(1) != "GET") {
        connection->write("HTTP/1.0 501 Not Implemented\r\n");
        connection->write("Content-Length: 0\r\n\r\n");
    } else {
        QFileInfo info;

        info.setFile(QDir(root), match.captured(2));

        qDebug() << root << info.absoluteFilePath();

        if (info.isDir()) {
            info.setFile(QDir(root), "index.html");
        }

        writeResponse(connection, info);
    }
}
