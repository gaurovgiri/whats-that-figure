#ifndef GAME_H
#define GAME_H

#include "constants.h"

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QSignalMapper>
#include <QTime>
#include <QThread>

class Game : public QObject
{
    Q_OBJECT
public:
    Game(const int _numberOfPlayers = 2, const int _numberOfGuesses = 3, QObject* parent = nullptr);
    ~Game();

private:
    // Game Atributes
    QList<QString> playerList;
    QList<int> playerIDList;
    QList<int> stateList;
    QList<int> scoreList;
    QList<int> guessList;

    bool isPainterSet;
    int painterIndex;
    QString paintingName;
    int numberOfPlayers;
    int numberOfGuesses;
    int gameRound;


public:

    // Getters
    QList<QString> getPlayerList();
    QList<int> getStateList();
    QList<int> getScoreList();
    QList<int> getGuessList();

    int getCurrentPlayerCount();
    int getNumOfMaxPlayers();
    int getPlayerState(int);
    int getNumberOfPlayersBeforeGameStart();
    QString getPlayer(int);
    bool isReady();

    // Game

    void initializeGame();
    void restartGame();
    void createContainers();
    void initializeGuessList();
    void initializeStateList();
    bool areGuessersOutOfGuesses();
    void removePlayer(int playerIndex);

public slots:

    void onAddPlayerToGameSIGNAL(QString,int);
    void onSetPaintingNameSIGNAL(QString);
    void onErasePlayerSIGNAL(int);
    void onCheckGuessSIGNAL(QString, int);

signals:
    void clientMsgReadySIGNAL(QString, QString, QList<QString>, QList<int>, QList<int>, QList<int>, int, int, int);
    void newWorkerListReadySIGNAL(QList<int>);
    void gameOverSIGNAL();
};

#endif // GAME_H
