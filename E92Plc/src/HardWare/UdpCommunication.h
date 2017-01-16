#ifndef UDPCOMMUNICATION_H
#define UDPCOMMUNICATION_H
#include <QtNetwork>

class UDPCommunication
{
public:
    UDPCommunication();
    void writeDatagramMsg(QString);
    QString getClientIP();
    void getServeIP();
	int getWorkMode();
public:
    QUdpSocket *sender;
    QByteArray datagram;
    QHostAddress serverAddress;
    qint16 serverPort;
private:
    QString configFileDirectory;
};
extern UDPCommunication *pUDPCommunication;
#endif // UDPCOMMUNICATION_H
