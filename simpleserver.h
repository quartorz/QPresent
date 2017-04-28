#ifndef SIMPLESERVER_H
#define SIMPLESERVER_H

#include <QTcpServer>

class SimpleServer : public QTcpServer
{
public:
    SimpleServer();

    void setDocumentRoot(const QString &root);

private:
    QString root;

    void parseRequestLine(QTcpSocket *connection);
};

#endif // SIMPLESERVER_H
