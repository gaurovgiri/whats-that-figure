#ifndef WORKER_H
#define WORKER_H

#include "constants.h"

#include <QTcpSocket>
#include <QObject>
#include <QStringList>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

#define EMPTY_PIX_MAP_SIZE 3000;

class Worker : public QObject
{
    Q_OBJECT
public:
     Worker(qintptr _socketDescriptor, unsigned int workerIndex, QObject *parent = nullptr);
     ~Worker();
private:
    qintptr socketDescriptor;
    QTcpSocket* socket;
    unsigned int workerIndex;

private:
    // Message
    QJsonObject SMessage(QString, QString, QList<QString>, QList<int>, QList<int>, QList<int>, int, int, int);
public slots:
    // Main
    void process();
    void onReadReady();

    // Game Slots
    void onSendPixMapSIGNAL(QByteArray&, QList<int>);
    void onClientMsgReadySIGNAL(QString, QString, QList<QString>, QList<int>, QList<int>, QList<int>, int, int, int);
    void onNewWorkerListReadySIGNAL(QList<int>);
    void onGameOverSIGNAL();
signals:

    // Game Signals
    void addPlayerToGameSIGNAL(QString, int);
    void setPaintingNameSIGNAL(QString);
    void checkGuessSIGNAL(QString, int);
    void passDataToServerSIGNAL(QByteArray&);
    void erasePlayerSIGNAL(int);

    // Thread Signals
    void finished();
    void error(QString);
};

#endif // WORKER_H
