/*
 * Ohjelman kirjoittaja
 * Nimi: Mika Valtonen
 * Opiskelijanumero: 166364
 * Käyttäjätunnus: valtone6
 * E-Mail: mika.valtonen@tuni.fi
*/

#include <iostream>
#include "player.hh"


Player::Player(const std::string& name):
    name_(name)
{

}

Player::~Player()
{

}

void Player::founded_cards(QPushButton *button)
{
    founded_cards_.push_back(button);
};


const std::string &Player::get_name()
{
    return name_;
}

int Player::get_pairs()
{
    return static_cast<int>(founded_cards_.size());
}
