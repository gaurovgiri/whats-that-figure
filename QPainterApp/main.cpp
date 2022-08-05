#include "mainwindow.h"
#include <QApplication>

const std::string serverAddr = "127.0.0.1";
const unsigned int serverPort = 8090;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w(serverAddr, serverPort);
    w.show();

    return app.exec();
}
