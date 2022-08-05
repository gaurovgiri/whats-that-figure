#include "game.h"

Game::Game(const int _numberOfPlayer, const int _numberOfGuesses, QObject* parent)
    :
      numberOfPlayers{_numberOfPlayer},
      numberOfGuesses{_numberOfGuesses},
      gameRound{0},
      QObject(parent)
{
    initializeGame();
}

Game::~Game()
{

}

// Game

void Game::initializeGame()
{
    createContainers();
    restartGame();
}

void Game::restartGame()
{
    gameRound++;
    isPainterSet = false;
    paintingName = "";
    initializeGuessList();
    initializeStateList();
}

void Game::createContainers()
{
    scoreList.clear();
    guessList.clear();
    stateList.clear();

    for(int i{0}; i< numberOfPlayers; i++)
    {
        scoreList.push_back(0);
        guessList.push_back(-1);
        stateList.push_back(-1);
    }
}

void Game::initializeGuessList()
{

    for(int i{0}; i< guessList.length(); i++)
    {
        guessList[i] = numberOfGuesses;
    }
}

void Game::initializeStateList()
{

    for(int i{0}; i < stateList.length(); i++)
    {
        stateList[i] = 1;
    }
    if(!isPainterSet)
    {
        painterIndex = gameRound % stateList.length();
        stateList[painterIndex] = 0;
        isPainterSet = true;
    }
}

// Slots:

void Game::onAddPlayerToGameSIGNAL(QString userName, int playerID)
{
    playerList.push_back(userName);
    playerIDList.push_back(playerID);

    QString typeOfMsg;
    (isReady()) ? typeOfMsg = SMsg::typeGameStart : typeOfMsg = SMsg::typePlayersLeft;

    emit clientMsgReadySIGNAL(typeOfMsg,"",
                              playerList, stateList,
                              scoreList, guessList,
                              -1, -1, getNumberOfPlayersBeforeGameStart());
}

void Game::onErasePlayerSIGNAL(int playerIndex)
{
    QString player = playerList[playerIndex];

    removePlayer(playerIndex);

    if(playerList.length() == 0)
        return;

    if(playerList.length() == 1)
    {
        // Game Over

        emit gameOverSIGNAL();

        initializeGame();

        QThread::msleep(1000);
        emit newWorkerListReadySIGNAL(playerIDList);

        QThread::msleep(1000);
        emit clientMsgReadySIGNAL(SMsg::typePlayersLeft,"",
                                  playerList, stateList,
                                  scoreList, guessList,
                                  -1, -1,getNumberOfPlayersBeforeGameStart());
    }
    else
    {
        // Other n -1 players should keep playing
        emit newWorkerListReadySIGNAL(playerIDList);
        if(stateList.indexOf(0) == -1)
        {
            restartGame();
        }
        QThread::msleep(1000);
        emit clientMsgReadySIGNAL(SMsg::typePlayerLeft, player,
                                  playerList, stateList,
                                  scoreList, guessList,
                                  -1,-1,0);
    }
}

void Game::removePlayer(int playerIndex)
{
    /*
    Removes player from all containers
    */
    playerList.removeAt(playerIndex);
    playerIDList.removeAt(playerIndex);
    stateList.removeAt(playerIndex);
    scoreList.removeAt(playerIndex);
    guessList.removeAt(playerIndex);
}

void Game::onSetPaintingNameSIGNAL(QString _paintingName)
{
    if(paintingName.length() < 1)
    {
        paintingName = _paintingName;
    }
}

void Game::onCheckGuessSIGNAL(QString guess, int playerIndex)
{

    // Check if painting nam was Set
    if(paintingName.length() < 1)
    {
        return;
    }

    bool isGuessCorrect;
    QString playerName = playerList[playerIndex];
    if(guess == paintingName)
    {
        scoreList[playerIndex]++;
        restartGame();
        isGuessCorrect = true;
    }
    else
    {
        guessList[playerIndex]--;
        if(areGuessersOutOfGuesses())
        {
            scoreList[painterIndex]++;
            int currentPainterIndex = painterIndex;
            restartGame();
            emit clientMsgReadySIGNAL(SMsg::typePainterWon,playerList[currentPainterIndex],
                                      playerList, stateList,
                                      scoreList, guessList,
                                      -1,true, 0);

            return;
        }
        isGuessCorrect = false;
    }
    emit clientMsgReadySIGNAL(SMsg::typeFeedBack, playerName,
                              playerList, stateList,
                              scoreList, guessList,
                              -1, isGuessCorrect, 0);
}

bool Game::areGuessersOutOfGuesses()
{
    bool gameOver = true;

    for(int i{0}; i < guessList.length(); i++)
    {
        if(i != painterIndex)
        {
            if(guessList[i] > 0)
            {
                gameOver = false;
                break;
            }
        }
    }
    return gameOver;
}

// Getters

QList<QString> Game::getPlayerList()
{
    return playerList;
}

QList<int> Game::getStateList()
{
    return stateList;
}

QList<int> Game::getScoreList()
{
    return scoreList;
}

QList<int> Game::getGuessList()
{
    return guessList;
}

bool Game::isReady()
{
    return ( playerList.length() >  0 && playerList.length() == numberOfPlayers);
}

int Game::getCurrentPlayerCount()
{
    return playerList.length();
}

int Game::getNumOfMaxPlayers()
{
    return numberOfPlayers;
}

int Game::getNumberOfPlayersBeforeGameStart()
{
    return numberOfPlayers - playerList.length();
}

int Game::getPlayerState(int playerIndex)
{
    return stateList[playerIndex];
}

QString Game::getPlayer(int playerIndex)
{
    return playerList[playerIndex];
}





