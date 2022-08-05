#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player
{
public:
    Player(int _state, int _numberOfGuesses = 3);
    ~Player();
private:
    int state;
    int numberOfGuesses;
    int guessesLeft;
    bool paintingNameSetFlag;
public:
    // Getters
    int getState();
    bool isPaintingNameSet();

    // Setters
    void setState(int _state);
    void setPaintingName();
    void guess();
    void intialize(int _state);
};

#endif // PLAYER_H
