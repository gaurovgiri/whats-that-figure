#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const std::string serverAddr, const unsigned int serverPort , QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);
    setUIDimensions();

    // Intialize Home Page UI
    intializeHomePage();
    onHomePage = true;

    // Create and Register TCP Client
    createClient(serverAddr,serverPort);
    registerClient();
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();

    if(player != nullptr)
        delete player;

    if(paintAreaPtr != nullptr)
        delete paintAreaPtr;

    delete scrollAreaLayoutHomePage;
    delete scrollAreaLayoutGamePage;
    delete ui;
}

//Client

void MainWindow::createClient(const std::string _serverAddr, const unsigned int _serverPort)
{
    client = new Client(_serverAddr, _serverPort);

    client->moveToThread(&workerThread);

    connectClientSignals();

    workerThread.start();
}

void MainWindow::connectClientSignals()
{
    connect(&workerThread,SIGNAL(started()),client,SLOT(createSocket()));
    connect(&workerThread, SIGNAL(finished()), client, SLOT(deleteLater()));
    connect(this, SIGNAL(intializePlayerSIGNAL(QString)) , client, SLOT(onIntializePlayerSIGNAL(QString)));
    connect(this, SIGNAL(sendDataReadySIGNAL(QPixmap&)), client, SLOT(sendPixMap(QPixmap&)));

    connect(client, SIGNAL(pixMapReadySIGNAL(QPixmap&)), this, SLOT(onPixMapReadySIGNAL(QPixmap&)));
    connect(client, SIGNAL(serverMsgReadySIGNAL(QList<QString>, QList<int>, QList<int>, QList<int>, QString, QString, int, int, int)),
            this, SLOT(onServerMsgReadySIGNAL(QList<QString>, QList<int>, QList<int>, QList<int>, QString,QString, int, int, int)));

    connect(this, SIGNAL(sendDisconnectSIGNAL()),client, SLOT(onSendDisconnectSIGNAL()));
    connect(this, SIGNAL(sendMsgSIGNAL(QString&, unsigned int)),client,SLOT(onSendMsgSIGNAL(QString&, unsigned int)));

}

void MainWindow::registerClient()
{
    QString userName = getUserName();
    if(userName.length() > 0)
    {
        emit intializePlayerSIGNAL(userName);
    }
}

// Generic UI

void MainWindow::setUIDimensions()
{
    resize(MAIN_WIN_WIDTH,MAIN_WIN_HEIGHT);
    this->setMaximumHeight(MAIN_WIN_HEIGHT);
    this->setMinimumHeight(MAIN_WIN_HEIGHT);
    this->setMaximumWidth(MAIN_WIN_WIDTH);
    this->setMinimumWidth(MAIN_WIN_WIDTH);
}

// Create Home Page UI Components

void MainWindow::intializeHomePage()
{
    ui->gameTitleField_HP->setText(GAME_TITLE);
    ui->gameTitleField_HP->setAlignment(Qt::AlignCenter);
    ui->infoField_HP->setText("WhatsThatFigure is a game where you either play a Painter or a Guesser. Painter's task is to set the Name of their Painting and draw it.  Each Player  has 3 Guesses to figure out the drawing name. if all players fail to figure out what the painting represents, the Painter gets a point.\n\nWaiting for other players to join before Game Start...");
    ui->infoField_HP->setAlignment(Qt::AlignLeft);
    updateNumOfPlayersBeforeGameStart(-1);

    createScrollAreaHomePage();
}

void MainWindow::switchToGamePage()
{
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
    onHomePage = false;
}

void MainWindow::switchToHomePage()
{
    ui->stackedWidget->setCurrentWidget(ui->homePage);
    onHomePage = true;
}

void MainWindow::updateNumOfPlayersBeforeGameStart(int deltaPlayers)
{
    ui->playersLeftField_HP->setText("Players Left Before Game Start: " + QString::number(deltaPlayers));
    ui->playersLeftField_HP->setAlignment(Qt::AlignCenter);
}

// Home Page Scroll Area

void MainWindow::createScrollAreaHomePage()
{
    if(scrollAreaLayoutHomePage == nullptr)
    {
        scrollAreaLayoutHomePage = new QFormLayout();
        ui->playerScrollArea_HP->widget()->setLayout(scrollAreaLayoutHomePage);

        scoreInfoRow = new QLabel("Player    Score    Lives");
        ui->playerScrollArea_HP->widget()->layout()->addWidget(scoreInfoRow);
    }
}

void MainWindow::fillScoreAreaHomePage(QList<QString> playerList, QList<int> scoreList, QList<int> guessList)
{
    for(int i{0}; i < playerList.length(); i++)
    {
        QString rowString = playerList[i] + "  " +  QString::number(scoreList[i]) + "  " + QString::number(guessList[i]) + "  ";
                    qDebug() << "Row: " << rowString << "\n";
        ui->playerScrollArea_HP->widget()->layout()->addWidget(new QLabel(rowString));
    }
}

void MainWindow::clearScoreScrollAreaHomePage()
{
    if(scrollAreaLayoutHomePage->rowCount() > 1)
    {
        QLayoutItem* child;
        while ((child = scrollAreaLayoutHomePage->takeAt(1)) != 0)
        {
                delete child->widget(); //get rid of widget too
                delete child;
         }
    }
}

// Create Game Page UI Components

void MainWindow::intializeGamePage()
{
    ui->gameTitleField_GP->setText("WhatsThatFigure");
    ui->gameTitleField_GP->setAlignment(Qt::AlignCenter);
    createScrollAreaGamePage();
}

void MainWindow::createPaintAreaMenu()
{
    optionMenu = new QMenu(tr("Drawing Options"),this);

    optionMenu->addAction(penColorAction);
    optionMenu->addAction(penWidthAction);
    optionMenu->addAction(clearScreenAction);

    menuBar()->addMenu(optionMenu);
}

void MainWindow::createPaintArea()
{
    paintAreaPtr = new PaintArea(this);

    positionPaintArea();

    connect(paintAreaPtr, &PaintArea::painterAreaModifiedSIGNAL, this, &MainWindow::onPainterAreaModified);
    penColorAction = new QAction(tr("&Pen Color"),this);
    penWidthAction = new QAction(tr("&Pen Width"),this);
    clearScreenAction = new QAction(tr("&Clear Sceen"),this);

    connect(penColorAction, SIGNAL(triggered()), this, SLOT(selectPenColor()));
    connect(penWidthAction, SIGNAL(triggered()), this, SLOT(selectPenWidth()));
    connect(clearScreenAction, SIGNAL(triggered()), this, SLOT(clearPainterArea()));

}

void MainWindow::positionPaintArea()
{
    int paintArea_start_x = 0;
    int paintArea_start_y = 40;

    if(player->getState() == 0)
        paintArea_start_y = 60;

    paintAreaPtr->setGeometry(paintArea_start_x,paintArea_start_y,PAINT_AREA_WIDTH, PAINT_AREA_HEGHT);
}

void MainWindow::positionAdditionUIComponents()
{
    int width = 300;
    int height = 30;
    int heightOffset = 100;
    int widthOffset = 50;

    if(player->getState() == 0)
    {
        heightOffset -= 20;
    }

    inputMsgLine->setGeometry(QRect(this->width() / 2 - width/2 - widthOffset,
                                    this->height() - heightOffset,
                                    width,
                                    height));

    submitMsgButton->setGeometry(QRect(inputMsgLine->x() + inputMsgLine->width(),
                                       this->height() - heightOffset,
                                       submitMsgButton->width(),
                                       height));
}

void MainWindow::createAdditionUIComponents()
{
    if(player->getState() == 0)
    {
         ui->gameInfoField_GP->setText("You are currently the Drawer, below you should enter your drawing name for other players to guess");
         createPaintAreaMenu();
    }
    else if(player->getState() == 1)
    {
         ui->gameInfoField_GP->setText("You are currently the Guesser, bellow you can enter your guess for what the other player is drawing");
    }
    ui->gameInfoField_GP->setAlignment(Qt::AlignCenter);

    if(inputMsgLine == nullptr)
    {
        inputMsgLine = new QLineEdit(this);
        inputMsgLine->setStyleSheet("background-color: rgb(190,190,190); color:rgb(80,80,80); border:none; text:bold");
    }

    if(submitMsgButton == nullptr)
    {
        submitMsgButton = new QPushButton("&Submit",this);
        submitMsgButton->setStyleSheet("background-color: rgb(166, 31, 52); color:white; border:none");
        connect( submitMsgButton, SIGNAL(released()), this, SLOT(forwardMsgToClient()));
    }

    positionAdditionUIComponents();

}

void MainWindow::deleteAdditionUIComponents()
{
    switch(player->getState())
    {
        case 0:
            // delete ui componenets unique to player state 1
            break;
        case 1:
            // delete ui componenets unique to player state 1
            delete optionMenu;
            break;
    }
}

void MainWindow::setupGamePageUIForCurrentPlayerState()
{
    createPaintArea();
    createAdditionUIComponents();
    displayUIComponents();
    displayPlayerInfo();
}

void MainWindow::reIntialize(int newPlayerState)
{
    clearPainterArea();

    player->intialize(newPlayerState);
    paintAreaPtr->lockPaintArea();
    deleteAdditionUIComponents();
    positionPaintArea();

    createAdditionUIComponents();
    displayUIComponents();
    displayPlayerInfo();

     update();
}

void MainWindow::restartGame()
{
    delete player;
    delete optionMenu;

    paintAreaPtr->hide();
    submitMsgButton->hide();
    inputMsgLine->hide();
}

// Game Page Scroll Area

void MainWindow::createScrollAreaGamePage()
{
    if(scrollAreaLayoutGamePage == nullptr)
    {
        scrollAreaLayoutGamePage = new QFormLayout();
        ui->playerScrollArea_GP->widget()->setLayout(scrollAreaLayoutGamePage);

        scoreInfoRow = new QLabel("Player    Score    Lives");
        ui->playerScrollArea_GP->widget()->layout()->addWidget(scoreInfoRow);
    }
}

void MainWindow::fillScoreAreaGamePage(QList<QString> playerList, QList<int> scoreList, QList<int> guessList)
{
    for(int i{0}; i < playerList.length(); i++)
    {
        QString rowString = playerList[i] + "  " +  QString::number(scoreList[i]) + "  " + QString::number(guessList[i]) + "  ";
                    qDebug() << "Row: " << rowString << "\n";
        ui->playerScrollArea_GP->widget()->layout()->addWidget(new QLabel(rowString));
    }
}

void MainWindow::clearScoreScrollAreaGamePage()
{
    if(scrollAreaLayoutGamePage->rowCount() > 1)
    {
        QLayoutItem* child;
        while ((child = scrollAreaLayoutGamePage->takeAt(1)) != 0)
        {
                delete child->widget(); //get rid of widget too
                delete child;
         }
    }
}
// Display UI Components

void MainWindow::displayUIComponents()
{
    switch(player->getState())
    {
    case 0:
        menuBar()->show();
    case 1:
        inputMsgLine->show();
        submitMsgButton->show();
        paintAreaPtr->show();
    default:
        break;
    }
}

void MainWindow::displayPlayerInfo()
{
    std::string info;
    if(player->getState() == 0)
    {
      info = "You are currently a Painter, please provide the name of you drawing below\n";
    }
    else if(player->getState() == 1)
    {
         info = "You are currently a Guesser, you have " + std::to_string(NUMBER_OF_GUESSES) + " guesses, to figure out what the painting represents\n";
    }

    QMessageBox::information(
        this,
        tr("QPainterApp"),
        tr(info.c_str()));
}

void MainWindow::displayGameState(bool guessFeedBack, QString playerName)
{
    QString msg;
    if(guessFeedBack == true)
    {
        msg = playerName + " Won!\n";
    }
    else
    {
        msg = playerName + " Failled!\n";
    }

    NotificationBox* nBox = new NotificationBox(msg,2000,this);
    nBox->display();
}

void MainWindow::displayPlayerLeftInfo(QString playerName)
{
    NotificationBox* nBox = new NotificationBox("Player: " + playerName + " left the Game",2000,this);
    nBox->display();
}

// Painter Area Action Handlers

void MainWindow::selectPenColor()
{
    QColor newColor = QColorDialog::getColor(paintAreaPtr->getPenColor());
    if(newColor.isValid())
    {
        paintAreaPtr->setPenColor(newColor);
    }
}

void MainWindow::selectPenWidth()
{
    bool selectionOK = false;
    int newWidth = QInputDialog::getInt(this,
                                        tr("Scribble"),
                                        tr("Select Pen Width:"),
                                        paintAreaPtr->getPenWidth(),
                                        1, 100, 1, &selectionOK);
    if(selectionOK)
    {
        paintAreaPtr->setPenWidth(newWidth);
    }
    else
    {
        std::cout<<"Selection not ok\n";
    }
}

void MainWindow::clearPainterArea()
{
    paintAreaPtr->clearScreen();
}

void MainWindow::onPainterAreaModified()
{
    QPixmap pixMap = paintAreaPtr->getPixMap();
    emit sendDataReadySIGNAL(pixMap);
}

// Client Action Handlers

void MainWindow::onPixMapReadySIGNAL(QPixmap& pixMap)
{
    paintAreaPtr->clearScreen();
    paintAreaPtr->setPixMap(pixMap);
    update();
}

void MainWindow::forwardMsgToClient()
{
    QString inputText = inputMsgLine->text();
    if(inputText.size() < 1)
        return;

    if(player->getState() == 0 && !player->isPaintingNameSet())
    {
        emit sendMsgSIGNAL(inputText, player->getState());

        player->setPaintingName();
        paintAreaPtr->unlockPaintArea();
        NotificationBox* nBox = new NotificationBox("Drawing Name Submited",1000,this);
        nBox->display();
    }
    else if(player->getState() == 1)
    {
        emit sendMsgSIGNAL(inputText, player->getState());
    }
}

// Prepare UI

void MainWindow::onServerMsgReadySIGNAL(QList<QString> playerList, QList<int> stateList, QList<int> scoreList, QList<int> guessList, QString type , QString playerName, int playerIndex, int guessFeedBack, int deltaPlayers)
{
    /*
    Function receives data from the client, it interacts with UI and Game State differently
    depending on the type of message
    */

    if(type == SMsg::typePlayersLeft)
    {
        // Update Home Page UI
        updateNumOfPlayersBeforeGameStart(deltaPlayers);
        clearScoreScrollAreaHomePage();
        fillScoreAreaHomePage(playerList, scoreList, guessList);
    }
    if( type == SMsg::typeGameStart)
    {
        // Create a new player and intialize Game Page UI
        int playerState = stateList[playerIndex];
        player = new Player(playerState,NUMBER_OF_GUESSES);
        intializeGamePage();
        switchToGamePage();
        setupGamePageUIForCurrentPlayerState();
    }
    else if( type == SMsg::typeGameStats)
    {
        // Update The Score List
        clearScoreScrollAreaGamePage();
        fillScoreAreaGamePage(playerList, scoreList, guessList);
    }
    else if(type == SMsg::typeFeedBack)
    {
        // Display feed Back Pop Up Window
        displayGameState(guessFeedBack, playerName);
        sleep(2000);
        if(guessFeedBack)
        {
            // in case player guessed correctly, reintialize its player state and UI
            reIntialize(stateList[playerIndex]);
        }
    }
    else if(type == SMsg::typePainterWon)
    {
        // Display feed Back Pop Up Window and reintialize player state and UI
        displayGameState(guessFeedBack, playerName);
        sleep(2000);
        reIntialize(stateList[playerIndex]);
    }
    else if(type == SMsg::typePlayerLeft)
    {
        // Display Player Left Pop Up Window and reintilaize player State and UI
        displayPlayerLeftInfo(playerName);
        sleep(2000);
        reIntialize(stateList[playerIndex]);
    }
    else if(type == SMsg::typeGameOver)
    {
        // Restart Game and switch back to Home Page Widget
        restartGame();
        switchToHomePage();
    }

    if(!onHomePage)
    {
        clearScoreScrollAreaGamePage();
        fillScoreAreaGamePage(playerList, scoreList, guessList);
    }
}

// Helper Functions

QString MainWindow::getUserName()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter Info"),
                                             tr("User name:"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
        return text;
    else
        return "";
}

void MainWindow::sleep(int milliSeconds)
{
    QTimer* timer = new QTimer(this);
    timer->start(milliSeconds);
}

// Close Handler

void MainWindow::closeEvent(QCloseEvent*)
{
    emit sendDisconnectSIGNAL();
}
