/*
 * Ohjelman kirjoittaja
 * Nimi: Mika Valtonen
 * Opiskelijanumero: 166364
 * Käyttäjätunnus: valtone6
 * E-Mail: mika.valtonen@tuni.fi
*/

#ifndef DIALOG_HH
#define DIALOG_HH

#include "player.hh"

#include <QDebug>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTimer>
#include <QTime>
#include <QLCDNumber>

#include <random>
#include <iterator>
#include <iostream>
#include <vector>
#include <chrono>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    // game initialization
    void initGame(bool);
    // start/restart game
    void playButtonClicked();
    // handles card clicks
    void cardButtonClicked();
    // end game
    void closeButtonClicked();
    // remove founded pairs from view and switch players
    void checkPairs();
    // show play time on LCD-screen
    void showPlayTime();

private:

    // constant for cards
    const int CARDS_MIN = 2;
    const int CARDS_MAX = 36;
    const int CARDS_DEFAULT = 4;
    const int CARD_WIDTH = 30;
    const int CARD_HEIGHT = 60;

    // const for pictures
    const int FRUIT_SIZE = 20;

    // constant for players
    const int PLAYERS_MIN = 1;
    const int PLAYERS_MAX = 4;
    const int PLAYERS_DEFAULT = 2;

    // Timer delay in ms
    const int TIMER_ = 1000;

    // main input-layout (rightside vertical-layout)
    void createInputGroupBox();
    // main output-layout (leftside vertical-layout)
    void createOutputGroupBox();
    // find the smallest common factors
    std::vector<int> findFactors(int cards);
    // clear layouts and widgets
    void clearLayout(QLayout *layout);
    // enable/disable cardbuttons
    void enable_cards(bool is_visible);
    // enable/disable spinboxes and radiobuttons
    void enable_user_inputs(bool is_visible);
    // print game results on view
    void showResults();

    QGroupBox *inputBox_;
    QGroupBox *outputBox_;
    QGridLayout *gridLayout_;
    QVBoxLayout *radioLayout_;

    QSpinBox *cards_spinbox_;
    QSpinBox *players_spinbox_;
    QPushButton *play_button_;
    QPushButton *close_button_;

    // card background picture
    QPixmap *background_image_;

    QTimer *timer_;
    QTime *time_;

    // show playtime
    QLCDNumber *lcd_;

    // vectors to save button pointers
    std::vector<QRadioButton*> radio_buttons_;
    std::vector<QPushButton*> cards_;
    std::vector<QPushButton*> selected_cards_;
    std::vector<QPushButton*> players_;

    // list of players
    std::vector<Player> player_;

    // Vector of fruits
    const std::vector<std::string> fruits =
    {"apple", "apple", "bananas", "bananas", "blueberry",
     "blueberry", "cherries", "cherries", "coconut", "coconut",
     "cowberry", "cowberry", "eggplant", "eggplant", "grapes",
     "grapes", "kiwi", "kiwi", "nut", "nut", "orange",
     "orange", "pear", "pear", "pineapple", "pineapple",
     "plum", "plum", "rasberry", "rasberry", "strawberry",
     "strawberry", "tomato", "tomato", "watermelon",
     "watermelon"};

    std::vector<std::string> selected_fruits_;

    // Defining where the images can be found and what kind of images they are
    const std::string PREFIX = (":/");
    const std::string SUFFIX = (".png");

    // letters on the front side of the card
    const std::string all_letters_ =
            "AABBCCDDEEFFGGHHIIJJKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ";
    std::string selected_letters_ = "";

    // shows the player's turn in the game
    int in_turn_ = 0;

    // founded total pairs
    int founded_pairs_ = 0;

};
#endif // DIALOG_HH
