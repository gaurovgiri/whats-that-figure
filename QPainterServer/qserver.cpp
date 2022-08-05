#include "qserver.h"

QServer::QServer(QString _serverAddr, unsigned int _serverPort)
    :
      serverAddr{_serverAddr},
      serverPort{_serverPort},
      clientCounter{0}
{
    startServer();
}

QServer::~QServer()
{
    delete game;
}
// Main
void QServer::startServer()
{
    if(!this->listen(QHostAddress(serverAddr),serverPort))
        qDebug() << "Could Not Start Server!\n";
    else
        qDebug() << "Server Started!\n";
}

void QServer::incomingConnection(qintptr _socketDescriptor)
{

    if(game == nullptr)
    {
        game = new Game();
    }
    if(game->getCurrentPlayerCount() + 1 <= game->getNumOfMaxPlayers())
    {
        // Create Unqiue Worker and Thread Object for new Client
        QThread* newThread = new QThread;
        Worker* worker = new Worker(_socketDescriptor, game->getCurrentPlayerCount());

        // Move worker to a new Thred
        worker->moveToThread(newThread);

        // Thread Functionality
        connectThreadSlots(worker, newThread);

        // Main Functionality
        connectDataExchangeSlots(worker);

        // Game Functionality
        connectGameSlots(game, worker);

        // Start new Thread
        newThread->start();
    }
}

// Connect Slots

void QServer::connectThreadSlots(Worker* worker, QThread* thread)
{
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()) , worker, SLOT(process()));
    connect(worker,SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}

void QServer::connectDataExchangeSlots(Worker* worker)
{
    connect(worker, SIGNAL(passDataToServerSIGNAL(QByteArray&)), this, SLOT(dataFromSocket(QByteArray&)));
    connect(this, SIGNAL(sendPixMapSIGNAL(QByteArray&,QList<int>)),worker, SLOT(onSendPixMapSIGNAL(QByteArray&,QList<int>)));
}

void QServer::connectGameSlots(Game* game, Worker* worker)
{
    connect(worker, SIGNAL(addPlayerToGameSIGNAL(QString,int)), game, SLOT(onAddPlayerToGameSIGNAL(QString,int)));
    connect(worker,SIGNAL(setPaintingNameSIGNAL(QString)), game, SLOT(onSetPaintingNameSIGNAL(QString)));
    connect(worker, SIGNAL(checkGuessSIGNAL(QString,int)), game, SLOT(onCheckGuessSIGNAL(QString,int)));
    connect(worker, SIGNAL(erasePlayerSIGNAL(int)), game, SLOT(onErasePlayerSIGNAL(int)));

    connect(game,SIGNAL(clientMsgReadySIGNAL(QString,QString,QList<QString>,QList<int>,QList<int>,QList<int>,int,int,int)),
            worker,SLOT(onClientMsgReadySIGNAL(QString,QString,QList<QString>,QList<int>,QList<int>,QList<int>,int,int,int)));
    connect(game,SIGNAL(newWorkerListReadySIGNAL(QList<int>)),worker,SLOT(onNewWorkerListReadySIGNAL(QList<int>)));

    connect(game,SIGNAL(gameOverSIGNAL()),worker, SLOT(onGameOverSIGNAL()));
}

// Slots
void QServer::dataFromSocket(QByteArray& data)
{
    emit sendPixMapSIGNAL(data, game->getStateList()); // send data to all other worker objects
}

void QServer::errorString(QString error)
{
    qDebug() << "Error in Worker thread: " << error << "\n";
}
