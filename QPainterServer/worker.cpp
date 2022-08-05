#include "worker.h"

Worker::Worker(qintptr _socketDescriptor, unsigned int _workerIndex, QObject *parent)
    :
      QObject{parent},
      socketDescriptor{_socketDescriptor},
      workerIndex{_workerIndex}

{
}

Worker::~Worker()
{
    delete socket;
}

// Main
void Worker::process()
{
    socket = new QTcpSocket(this);

    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error("Could not Create Client Socket!\n");
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
}

void Worker::onReadReady()
{
    QByteArray data = socket->readAll();
    if(data.size() < 3000)
    {

        // Received JSON Message
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject receivedJson = jsonDoc.object();
        QString msgType = receivedJson[CMsg::fieldType].toString();

        if(msgType == CMsg::typeRegisterUser)
        {
            QString userName = receivedJson[CMsg::fieldUserName].toString();
            emit addPlayerToGameSIGNAL(userName, socketDescriptor);
        }
        else if(msgType == CMsg::typeSetPaintingName)
        {
            QString paintingName = receivedJson[CMsg::fieldPaintingName].toString();
            emit setPaintingNameSIGNAL(paintingName);
        }
        else if(msgType == CMsg::typeGuessWord)
        {
            QString guess = receivedJson[CMsg::fieldGuessWord].toString();
            emit checkGuessSIGNAL(guess, workerIndex);
        }
        else if(msgType == CMsg::typeDisconnectUser)
        {
            erasePlayerSIGNAL(workerIndex);
            emit finished();
        }
    }
    else
    {
        // Received PixelMap
        emit passDataToServerSIGNAL(data);
    }
}

// Game Slots

void Worker::onSendPixMapSIGNAL(QByteArray& data, QList<int> stateList)
{

    if(stateList[workerIndex] == 1)
    {
       socket->write(data);  // send pixMap to all guesser players
    }
}

void Worker::onClientMsgReadySIGNAL(QString typeOfMessage, QString playerName,
                                    QList<QString> playerList, QList<int> stateList,
                                    QList<int> scoreList, QList<int> guessList,
                                    int playerIndex,int feedBack,int deltaPlayers)
{
    if(playerIndex == -1)
    {
        playerIndex = workerIndex;
    }

    QJsonObject serverMsg = SMessage(typeOfMessage, playerName, playerList, stateList, scoreList, guessList, playerIndex, feedBack, deltaPlayers);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(serverMsg);
    socket->write(jsonDoc.toJson());
}

void Worker::onNewWorkerListReadySIGNAL(QList<int>playerIDList)
{
    /*
    Function Updates the workerIndex using socketDescriptor, thats stays constant
    */
    int index = playerIDList.indexOf(socketDescriptor);
    if(index != -1)
    {
        workerIndex = index;
    }
}

void Worker::onGameOverSIGNAL()
{
   QJsonObject gameOverMsg;
   gameOverMsg[SMsg::fieldType] = SMsg::typeGameOver;
   QJsonDocument jsonDoc;
   jsonDoc.setObject(gameOverMsg);
   socket->write(jsonDoc.toJson());

}

// Message

template <typename  T>
QJsonArray toJsonArray(QList<T> container)
{
    QJsonArray jsonArray;
    for(T& element: container)
    {
        jsonArray.push_back(element);
    }
    return jsonArray;
}

QJsonObject Worker::SMessage(QString typeOfMessage, QString playerName,
                             QList<QString> playerList, QList<int> stateList,
                             QList<int> scoreList, QList<int> guessList,
                             int playerIndex,int feedBack,int deltaPlayers)
{
    QJsonObject clientMessage;
    clientMessage[SMsg::fieldType] = typeOfMessage;
    clientMessage[SMsg::fieldPlayerName] = playerName;
    clientMessage[SMsg::fieldPlayerList] = toJsonArray(playerList);
    clientMessage[SMsg::fieldStateList]= toJsonArray(stateList);
    clientMessage[SMsg::fieldScoreList] = toJsonArray(scoreList);
    clientMessage[SMsg::fieldGuessList] = toJsonArray(guessList);
    clientMessage[SMsg::fieldPlayerIndex] = playerIndex;
    if(feedBack != -1)
        clientMessage[SMsg::fieldGuessFeedBack] = feedBack;
    clientMessage[SMsg::fieldDeltaPlayers] = deltaPlayers;

    return clientMessage;
}
