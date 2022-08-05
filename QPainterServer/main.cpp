#include <QCoreApplication>
#include "qserver.h"

QString serverAddr = "0.0.0.0";
//QString serverAddr = "192.168.137.1";
unsigned int serverPort = 8090;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QServer server(serverAddr,serverPort);

    return a.exec();
}
