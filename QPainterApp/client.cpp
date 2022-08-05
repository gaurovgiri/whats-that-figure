#include "client.h"

Client:: Client(const std::string _serverAddr,const unsigned int _serverPort)
    :
      serverPort{_serverPort},
      serverAddr{_serverAddr},
      playerStateRecevied{false}
{

}

void Client::connectToServer()
{
    socket->connectToHost(serverAddr.c_str(),serverPort);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected\n";
    }

    else
        qDebug() << "Could not Connect\n";
}


void Client::createSocket()
{
    socket = new QTcpSocket(this);
    connectToServer();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
}


void Client::onIntializePlayerSIGNAL(QString playerUserName)
{
    QJsonObject clientMsg = CMessage(CMsg::typeRegisterUser,playerUserName,"","");
    QJsonDocument jsonDoc;
    jsonDoc.setObject(clientMsg);
    socket->write(jsonDoc.toJson());
}

void Client::onReadReady()
{
    QByteArray receivedBuffer = socket->readAll();
    if(receivedBuffer.size() < EMPTY_PIX_MAP_SIZE)
    {
        // Json message

        QJsonDocument jsonDoc = QJsonDocument::fromJson(receivedBuffer);
        QJsonObject receivedJson = jsonDoc.object();

        QList<QString> playerList;
        QList<int> stateList;
        QList<int> scoreList;
        QList<int> guessList;
        QString type;
        QString playerName;
        int playerIndex;
        int guessFeedBack;
        int deltaPlayers;

        QJsonArray playerListJsonArray = receivedJson[SMsg::fieldPlayerList].toArray();
        for(auto player : playerListJsonArray)
        {
            playerList.push_back(player.toString());
        }
        QJsonArray playerStatesJsonArray = receivedJson[SMsg::fieldStateList].toArray();
        for(auto state : playerStatesJsonArray)
        {
            stateList.push_back(state.toInt());
        }
        QJsonArray scoreListJsonArray = receivedJson[SMsg::fieldScoreList].toArray();
        for(auto score : scoreListJsonArray)
        {
            scoreList.push_back(score.toInt());
        }
        QJsonArray guessListJsonArray = receivedJson[SMsg::fieldGuessList].toArray();
        for(auto guess : guessListJsonArray)
        {
            guessList.push_back(guess.toInt());
        }

        type = receivedJson[SMsg::fieldType].toString();
        playerName = receivedJson[SMsg::fieldPlayerName].toString();
        playerIndex = receivedJson[SMsg::fieldPlayerIndex].toInt();
        guessFeedBack = receivedJson[SMsg::fieldGuessFeedBack].toInt();
        deltaPlayers = receivedJson[SMsg::fieldDeltaPlayers].toInt();

        emit serverMsgReadySIGNAL(playerList, stateList, scoreList, guessList, type, playerName, playerIndex, guessFeedBack,deltaPlayers);

    }
    else
    {
        // PixelMap
        QDataStream inStream(&receivedBuffer, QIODevice::ReadOnly);
        QPixmap receivedPixMap;
        inStream >> receivedPixMap;

        if(!receivedPixMap.isNull())
        {
            emit pixMapReadySIGNAL(receivedPixMap);
        }
    }
}

void Client::sendPixMap(QPixmap& pixMap)
{
    if( socket->state() != QTcpSocket::ConnectedState)
    {
         connectToServer();
    }

    QByteArray buffer;
    QDataStream  outStream(&buffer,  QIODevice::WriteOnly);
    outStream << pixMap;
    socket->write(buffer);
}


void Client::onSendDisconnectSIGNAL()
{
    if( socket->state() != QTcpSocket::ConnectedState)
    {
         connectToServer();
    }

    QJsonObject clientMsg = CMessage(CMsg::typeDisconnectUser,"","","");
    QJsonDocument jsonDoc;
    jsonDoc.setObject(clientMsg);
    socket->write(jsonDoc.toJson());
}

void Client::onSendMsgSIGNAL(QString& msg, unsigned int playerState)
{
    QJsonObject clientMsg;
    if( socket->state() != QTcpSocket::ConnectedState)
    {
         connectToServer();
    }
    switch(playerState)
    {
    case 0:
        clientMsg = CMessage(CMsg::typeSetPaintingName,"",msg,"");
        break;
    case 1:
        clientMsg = CMessage(CMsg::typeGuessWord,"","",msg);
        break;
    }

    QJsonDocument jsonDoc;
    jsonDoc.setObject(clientMsg);
    socket->write(jsonDoc.toJson());
}

QJsonObject Client::CMessage(const QString typeOfMessage, const QString userName, const QString paintingName, const QString guessWord)
{
    QJsonObject clientMessage;
    clientMessage[CMsg::fieldType] = typeOfMessage;
    clientMessage[CMsg::fieldUserName] = userName;
    clientMessage[CMsg::fieldPaintingName] = paintingName;
    clientMessage[CMsg::fieldGuessWord] = guessWord;

    return clientMessage;
}
