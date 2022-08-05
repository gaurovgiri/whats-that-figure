#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "paintarea.h"
#include "client.h"
#include "player.h"
#include "notificationbox.h"

#include <QMainWindow>
#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QSizePolicy>
#include <iostream>
#include <QString>

#include <QThread>

#define MAIN_WIN_WIDTH 1000
#define MAIN_WIN_HEIGHT 800
#define PAINT_AREA_WIDTH 1000
#define PAINT_AREA_HEGHT 600
#define NUMBER_OF_GUESSES 3
#define GAME_TITLE "WhatsThatFigure"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread workerThread;

public:
    MainWindow(const std::string serverAddr, const unsigned int serverPort , QWidget *parent = nullptr);
    ~MainWindow();
private:

    // UI Components
    Ui::MainWindow *ui;
    QMenu *optionMenu = nullptr;
    QLineEdit* inputMsgLine = nullptr;
    QPushButton* submitMsgButton = nullptr;
    QTextBrowser* infoTextField = nullptr;
    QFormLayout* scrollAreaLayoutHomePage = nullptr;
    QFormLayout* scrollAreaLayoutGamePage = nullptr;
    QLabel* scoreInfoRow = nullptr;
    NotificationBox* notificationBox = nullptr;
    bool onHomePage;

    // Actions
    QAction* penColorAction = nullptr;
    QAction* penWidthAction = nullptr;
    QAction* clearScreenAction = nullptr;

    // Objects
    Player* player = nullptr;
    Client* client = nullptr;
    PaintArea* paintAreaPtr = nullptr;

private:

    //Client
    void createClient(const std::string _serverAddr, const unsigned int _serverPort);
    void connectClientSignals();
    void registerClient();

    // Generic UI
    void setUIDimensions();

    // Create Home Page UI Components
    void intializeHomePage();
    void switchToHomePage();
    void switchToGamePage();
    void updateNumOfPlayersBeforeGameStart(int);

    // Home Page Scroll Area

    void createScrollAreaHomePage();
    void fillScoreAreaHomePage(QList<QString>,QList<int>, QList<int>);
    void clearScoreScrollAreaHomePage();

    // Create Game Page UI Components
    void intializeGamePage();
    void setupGamePageUIForCurrentPlayerState();
    void createPaintAreaMenu();
    void createPaintArea();
    void createAdditionUIComponents();
    void deleteAdditionUIComponents();

    // Position UI Components
    void positionPaintArea();
    void positionAdditionUIComponents();

    // Game Page Scroll Area
    void createScrollAreaGamePage();
    void fillScoreAreaGamePage(QList<QString>,QList<int>,QList<int>);
    void clearScoreScrollAreaGamePage();

    // Display UI Components
    void displayUIComponents();
    void displayPlayerInfo();
    void displayGameState(bool, QString);
    void displayPlayerLeftInfo(QString);

    // Helper Functions
    void reIntialize(int);
    void restartGame();
    QString getUserName();
    void sleep(int);



private slots:

    // Close Handler
    void closeEvent(QCloseEvent*);

    // Painter Area Action Handlers
    void selectPenColor();
    void selectPenWidth();
    void clearPainterArea();

public slots:

    void onPainterAreaModified();

    // Client Action Handlers
    void onPixMapReadySIGNAL(QPixmap& pixMap);
    void forwardMsgToClient();
    void onServerMsgReadySIGNAL(QList<QString>, QList<int>, QList<int>, QList<int>, QString, QString, int, int, int);


signals:
    void intializePlayerSIGNAL(QString);
    void sendDataReadySIGNAL(QPixmap&);
    void sendDisconnectSIGNAL();
    void sendMsgSIGNAL(QString&, unsigned int);
};
#endif // MAINWINDOW_H
