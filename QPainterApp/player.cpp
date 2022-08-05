#include "player.h"

Player::Player(int _state,int _numberOfGuesses)
    :
      state{_state},
      numberOfGuesses{_numberOfGuesses}
{
    intialize(state);
}

Player::~Player()
{

}

// Getters
int Player::getState()
{
    return state;
}

bool Player::isPaintingNameSet()
{
    return paintingNameSetFlag;
}

//Setters
void Player::setState(int _state)
{
    state = _state;
}

void Player::guess()
{
    guessesLeft -= 1;
}

void Player::setPaintingName()
{
    paintingNameSetFlag = true;
}

void Player::intialize(int _state)
{
    guessesLeft = numberOfGuesses;
    paintingNameSetFlag = false;
    state = _state;
}
