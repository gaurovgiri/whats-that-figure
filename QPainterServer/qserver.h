#ifndef QSERVER_H
#define QSERVER_H

#include "worker.h"
#include "game.h"

#include <QTimer>
#include <QTcpServer>
#include <QByteArray>
#include <QThread>

class QServer : public QTcpServer
{
    Q_OBJECT
public:

    QServer(QString _serverAddr, unsigned int _serverPort);
    ~QServer();

private:
    QString serverAddr;
    unsigned int serverPort;
    unsigned int clientCounter;
    Game* game = nullptr;

protected:
    void incomingConnection(qintptr) override;

private:
    // Main
    void startServer();

    // Connect Slots
    void connectThreadSlots(Worker* worker, QThread* thread);
    void connectDataExchangeSlots(Worker* worker);
    void connectGameSlots(Game* game, Worker* worker);

public slots:
    void dataFromSocket(QByteArray&);
    void errorString(QString);

signals:
    void sendPixMapSIGNAL(QByteArray&, QList<int>);
    void pass_socket_descriptor_(qintptr);
};

#endif // QSERVER_H
