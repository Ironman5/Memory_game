/*
 * Ohjelman kirjoittaja
 * Nimi: Mika Valtonen
 * Opiskelijanumero: 166364
 * Käyttäjätunnus: valtone6
 * E-Mail: mika.valtonen@tuni.fi
*/

#ifndef PLAYER_HH
#define PLAYER_HH

#include <iostream>
#include <vector>


#include <QDialog>
#include <QPushButton>
#include <QDebug>

class Player
{
public:
    // Constructor
    Player(const std::string& name);
    // Destructor
    ~Player();

    // save buttons of founded pairs to a vector
    void founded_cards(QPushButton* button);
    // return players name
    const std::string& get_name();
    // return number of founded pairs
    int get_pairs();

private:
    const std::string name_ = "";
    std::vector<QPushButton*> founded_cards_;
    std::vector<char> founded_pairs_;

};

#endif // PLAYER_HH
