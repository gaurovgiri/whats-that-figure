#ifndef CLIENTMESSAGE_HPP
#define CLIENTMESSAGE_HPP

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

QJsonObject CMessage(const QString typeOfMessage, const QString userName = "",  const QString paintingName = "", const QString guessWord = "")
{
    // type can either be: register, setPaintingName, guessWord
    QJsonObject clientMessage;
    clientMessage["type"] = typeOfMessage;
    clientMessage["userName"] = userName;
    clientMessage["paintingName"] = paintingName;
    clientMessage["quessWord"] = guessWord;

    qDebug() << clientMessage << "\n";
    return clientMessage;
}



#endif // CLIENTMESSAGE_HPP
