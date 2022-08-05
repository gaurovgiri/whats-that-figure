#ifndef CLIENT_H
#define CLIENT_H

#include "constants.h"

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QPixmap>
#include <QBuffer>
#include <QThread>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

#include <iostream>
#define EMPTY_PIX_MAP_SIZE 3000

class Client: public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Client(const std::string _serverAddr, const unsigned int _serverPort);

private:
    QTcpSocket* socket = nullptr;
    const unsigned int serverPort;
    const std::string serverAddr;
    bool playerStateRecevied;

private:

    void connectToServer();
    QJsonObject CMessage(const QString typeOfMessage, const QString userName = "",  const QString paintingName = "", const QString guessWord = "");

public slots:

    void createSocket();
    void onReadReady();
    void onIntializePlayerSIGNAL(QString playerUserName);
    void sendPixMap(QPixmap &pixMap);
    void onSendDisconnectSIGNAL();
    void onSendMsgSIGNAL(QString&,unsigned int playerState);

signals:
    void pixMapReadySIGNAL(QPixmap& pixMap);
    void serverMsgReadySIGNAL(QList<QString>, QList<int>, QList<int>, QList<int>, QString,QString, int, int, int);

    void playersLeftBeforeGameStartSIGNAL(int);
};

#endif // CLIENT_H
