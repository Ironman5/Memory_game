/*
 * Ohjelman kirjoittaja
 * Nimi: Mika Valtonen
 * Opiskelijanumero: 166364
 * Käyttäjätunnus: valtone6
 * E-Mail: mika.valtonen@tuni.fi
*/

#include "dialog.hh"
#include "player.hh"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // add background picture
    background_image_ = new QPixmap(QString::fromStdString(":/bart.png"));

    // add timer
    timer_ = new QTimer();
    connect(timer_, &QTimer::timeout, this, &Dialog::showPlayTime);

    createOutputGroupBox();
    createInputGroupBox();
    initGame(false);

    mainLayout->addWidget(outputBox_,3);
    mainLayout->addWidget(inputBox_,1);

    setLayout(mainLayout);
    setWindowTitle(tr("Memory game"));
}

Dialog::~Dialog()
{
    delete background_image_;
    delete time_;
    delete timer_;

}

void Dialog::initGame(bool game_is_on = false)
{

    // clear layout, vectors and variables when replay
    clearLayout(radioLayout_);
    clearLayout(gridLayout_);
    radio_buttons_.clear();
    cards_.clear();
    selected_cards_.clear();
    player_.clear();
    selected_fruits_.clear();

    in_turn_ = 0;
    founded_pairs_ = 0;

    // set margins to zero to center view
    gridLayout_->setContentsMargins(0,0,0,0);

    // create selected number of players and radiobuttons
    int number_of_players = players_spinbox_->value();

    for(int i = 0; i < number_of_players; i++)
    {
        Player player("Player " + std::to_string(i+1));
        player_.push_back(player);
        //        qDebug() << QString::fromStdString(player->get_name());

        QRadioButton *radioButton =
                new QRadioButton(QString::fromStdString(player.get_name()));
        radio_buttons_.push_back(radioButton);
        radioLayout_->addWidget(radioButton);

    }
    // set first radiobutton active
    radio_buttons_.at(0)->setChecked(true);

    // check selected number of cards and find their factors
    int number_of_cards = cards_spinbox_->value();
    std::vector<int> factors = findFactors(number_of_cards);
    int rows = factors.at(0);
    int columns = factors.at(1);

    // save selected cards of empty buttons to a vector
    for (int i = 0; i < number_of_cards; i++){
        QPushButton* pushButton = new QPushButton();
        pushButton->setFixedSize(CARD_WIDTH, CARD_HEIGHT);
        pushButton->setIcon(*background_image_);
        pushButton->setIconSize(QSize(CARD_WIDTH, CARD_HEIGHT));
        pushButton->setStyleSheet
                ("QPushButton{background-color: white}");
        cards_.push_back(pushButton);
    }

    // copy selected pictures
    copy(fruits.begin(),
         fruits.begin() + number_of_cards,
         back_inserter(selected_fruits_));

    // shuffle selected fruits
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(selected_fruits_.begin(), selected_fruits_.end(),
                 std::default_random_engine(seed));

    //    // copy chosen amount of letters
    //    selected_letters_.clear();
    //    selected_letters_= all_letters_.substr(0, number_of_cards);

    // shuffle chosen letters
    //    unsigned seed =
    //    std::chrono::system_clock::now().time_since_epoch().count();
    //    std::shuffle(selected_letters_.begin(), selected_letters_.end(),
    //                 std::default_random_engine(seed));

    // fill view with labeled cards
    int counter = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            gridLayout_->addWidget(cards_.at(counter), i, j);
            connect(cards_.at(counter), &QPushButton::clicked,
                    this,&Dialog::cardButtonClicked);
            counter++;
        };
    }

    // enable/disable inputs and outputs
    if(game_is_on)
    {
        enable_user_inputs(false);
        enable_cards(true);
    }
    else
    {
        enable_user_inputs(true);
        enable_cards(false);
    }

}

void Dialog::playButtonClicked()
{   
    // add zerotime
    time_ = new QTime(0, 0);

    // init game (game_is_on)
    initGame(true);

    // start timer
    timer_->start(TIMER_);
}


void Dialog::cardButtonClicked()
{
    // receive the button signal and find the button index
    QPushButton* pressed_button = static_cast<QPushButton*>(sender());

    std::vector<QPushButton*>::iterator it =
            std::find(cards_.begin(), cards_.end(), pressed_button);
    int index = it - cards_.begin();

    // set a name on the button to allow later comparison
    pressed_button->setObjectName(
                (QString::fromStdString(selected_fruits_.at(index))));

    // get a filename of fruit and get an image
    std::string filename = PREFIX + selected_fruits_.at(index) + SUFFIX;
    QPixmap image(QString::fromStdString(filename));

    // Scale the pixmap
    image = image.scaled(FRUIT_SIZE, FRUIT_SIZE);

    // set frontside image to the button
    pressed_button->setIcon(QIcon(image));
    pressed_button->setStyleSheet("QPushButton{background-color:Lime}");

    // save selected card to a vector (fruits)
    if(selected_cards_.empty() ||
            (selected_cards_.size() < 2 &&
             pressed_button !=
             selected_cards_.at(0)))
    {
        selected_cards_.push_back(pressed_button);
    }

    //    // Change the color of the pressed button and display the letters
    //    std::string letter = "";
    //    letter += selected_letters_.at(index);
    //    pressed_button->setIcon(QIcon());
    //    pressed_button->setStyleSheet("QPushButton{background-color:Lime}");
    //    pressed_button->setText(QString::fromStdString(letter));

    //    // save selected card to a vector (letters)
    //    if(selected_cards_.empty() ||
    //            (selected_cards_.size() < 2 &&
    //             pressed_button != selected_cards_.at(0)))
    //    {
    //        selected_cards_.push_back(pressed_button);
    //    }

    // Check possible pairs, enable cards and start timer
    if(selected_cards_.size()==2)
    {
        QTimer::singleShot(TIMER_, this, &Dialog::checkPairs);
        enable_cards(false);
    }
}

void Dialog::closeButtonClicked()
{
    close();
}

void Dialog::checkPairs()
{
    // save founded pairs to a player-instance
    //    if(selected_cards_.at(0)->text() == selected_cards_.at(1)->text())
    if(selected_cards_.at(0)->objectName() ==
            selected_cards_.at(1)->objectName())
    {
        player_.at(in_turn_).founded_cards(selected_cards_.at(0));
        ++founded_pairs_;

        // hide founded pair and clear selected cards vector
        selected_cards_.at(0)->setVisible(false);
        selected_cards_.at(1)->setVisible(false);
        selected_cards_.clear();
        enable_cards(true);

        // all pairs are founded
        if(founded_pairs_ == cards_spinbox_->value()/2)
        {
            showResults();
        }
    }

    // cards are not pair
    else
    {
        selected_cards_.at(0)->setIcon(*background_image_);
        //        selected_cards_.at(0)->setText("");
        selected_cards_.at(0)->setStyleSheet
                ("QPushButton{background-color: white}");
        selected_cards_.at(1)->setIcon(*background_image_);
        //        selected_cards_.at(1)->setText("");
        selected_cards_.at(1)->setStyleSheet
                ("QPushButton{background-color: white}");
        selected_cards_.clear();
        enable_cards(true);

        // change player
        if(player_.size() > 1)
        {
            if(static_cast<int>(player_.size()) == in_turn_ + 1)
            {
                in_turn_ = -1;
            }
            ++in_turn_;
            radio_buttons_.at(in_turn_)->setChecked(true);
        }
    }
}

void Dialog::showPlayTime()
{
    // add 1 sec to zerotime
    *time_ = time_->addSecs(1);
    QString text = time_->toString("mm:ss");

    // show current playtime
    lcd_->display(text);
}

void Dialog::createOutputGroupBox()
{
    outputBox_ = new QGroupBox();
    QVBoxLayout *outputLayout = new QVBoxLayout;

    // layout to show cards
    gridLayout_ = new QGridLayout();

    outputLayout->addLayout(gridLayout_);

    outputBox_->setLayout(outputLayout);
}

void Dialog::createInputGroupBox()
{

    inputBox_ = new QGroupBox();
    QVBoxLayout *inputLayout = new QVBoxLayout;

    // cards label and spinbox
    QLabel* card_label = new QLabel("Number of cards:");
    inputLayout->addWidget(card_label);

    cards_spinbox_ = new QSpinBox;
    cards_spinbox_->setRange(CARDS_MIN, CARDS_MAX);
    cards_spinbox_->setSingleStep(2);
    cards_spinbox_->setValue(CARDS_DEFAULT);
    inputLayout->addWidget(cards_spinbox_);

    // players label and spinbox
    QLabel* player_label = new QLabel("Number of players:");
    inputLayout->addWidget(player_label);

    players_spinbox_ = new QSpinBox;
    players_spinbox_->setRange(PLAYERS_MIN, PLAYERS_MAX);
    players_spinbox_->setValue(PLAYERS_DEFAULT);
    inputLayout->addWidget(players_spinbox_);

    // play-button
    play_button_ = new QPushButton;
    play_button_->setCheckable(true);
    play_button_->setText("Play");
    inputLayout->addWidget(play_button_);
    connect(play_button_, &QPushButton::clicked,
            this,&Dialog::playButtonClicked);

    // radio-buttons
    QGroupBox *radioBox = new QGroupBox;
    radioLayout_ = new QVBoxLayout;
    radioBox->setLayout(radioLayout_);
    inputLayout->addWidget(radioBox);

    // game time display
    QLabel* time_label = new QLabel("Play time:");
    inputLayout->addWidget(time_label);
    time_ = new QTime(0, 0);
    lcd_ = new QLCDNumber;
    lcd_->setStyleSheet("background: white");
    QString text = time_->toString("mm:ss");
    lcd_->display(text);
    inputLayout->addWidget(lcd_);

    // close game widget
    close_button_ = new QPushButton("Close");
    inputLayout->addWidget(close_button_);
    connect(close_button_,&QPushButton::clicked,
            this,&Dialog::closeButtonClicked);

    inputBox_->setLayout(inputLayout);
}


std::vector<int> Dialog::findFactors(int cards)
{
    int modulo = 0;
    int rows = 0;
    int columns = 0;
    std::vector<int> factors;

    for(int i = 1; i <= cards; ++i){
        modulo = cards / i;
        if(cards == i * modulo and i <= modulo){
            rows = i;
            columns = modulo;
        }
    }

    factors.push_back(rows);
    factors.push_back(columns);

    return factors;
}

void Dialog::clearLayout(QLayout *layout)
{
    if (layout) {
        while(layout->count() > 0){
            QLayoutItem *item = layout->takeAt(0);
            QWidget* widget = item->widget();
            if(widget)
                delete widget;
            delete item;
        }
    }
}

void Dialog::enable_cards(bool is_visible)
{
    if(!is_visible) // disable cards
    {
        for(auto& card : cards_)
        {
            card->setDisabled(true);
        }
    }
    else // enable cards
        for(auto& card : cards_)
        {
            card->setEnabled(true);
        }
}

void Dialog::enable_user_inputs(bool is_visible)
{
    if(is_visible) // disable inputs
    {
        cards_spinbox_->setDisabled(false);
        players_spinbox_->setDisabled(false);

        for(auto& rb : radio_buttons_)
        {
            rb->setDisabled(false);
        }
    }
    else // enable inputs
    {
        cards_spinbox_->setDisabled(true);
        players_spinbox_->setDisabled(true);

        for(auto& rb : radio_buttons_)
        {
            rb->setDisabled(true);
        }
    }
}


void Dialog::showResults()
{
    //    qDebug() << "Game over";
    timer_->stop();
    clearLayout(gridLayout_);

    QLabel* result_label = new QLabel("RESULTS:");

    gridLayout_->addWidget(result_label,0,0);
    gridLayout_->setContentsMargins(0,0,0,200);
    int row = 1;

    // show game situation on view
    for(auto& player : player_)
    {
        QLabel* player_label = new QLabel();
        player_label->setText(QString::fromStdString(player.get_name())
                              + ": " + QString::number(player.get_pairs())
                              + " pairs");
        gridLayout_->addWidget(player_label,row,0);
        ++row;
    }

    enable_user_inputs(true);

}
