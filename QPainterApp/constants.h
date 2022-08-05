#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

// Server Message Constants
namespace SMsg
{
    const QString typePlayersLeft = "playersBeforeGameStart";
    const QString typeGameStart = "gameStart";
    const QString typeGameStats = "gameStats";
    const QString typeFeedBack = "feedBack";
    const QString typePainterWon = "painterWon";
    const QString typePlayerLeft = "playerLeft";
    const QString typeGameOver = "gameOver";

    const QString fieldType = "type";
    const QString fieldPlayerName = "playerName";
    const QString fieldPlayerIndex = "playerIndex";
    const QString fieldGuessFeedBack = "guessFeedBack";
    const QString fieldDeltaPlayers = "deltaPlayers";

    const QString fieldPlayerList = "playerList";
    const QString fieldStateList = "stateList";
    const QString fieldScoreList = "scoreList";
    const QString fieldGuessList = "guessList";
}

// Client Message Constants
namespace CMsg
{

    const QString typeSetPaintingName = "setPaintingName";
    const QString typeGuessWord = "guessWord";
    const QString typeDisconnectUser = "disconnectUser";
    const QString typeRegisterUser = "registerUser";


    const QString fieldType = "type";
    const QString fieldUserName = "userName";
    const QString fieldPaintingName = "paintingName";
    const QString fieldGuessWord = "quessWord";
}


#endif // CONSTANTS_H
