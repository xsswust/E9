#include "UdpCommunication.h"
#include "common.h"
UDPCommunication *pUDPCommunication;
UDPCommunication::UDPCommunication()
{
    //qDebug()<<"UDPCommunication ================1111";
    sender = new QUdpSocket();
    datagram = "hello world!";
    serverAddress = QHostAddress("192.168.118.210");
    serverPort=1000;
	configFileDirectory = FILE_NET_SETTING; //CONFIG_FILE_PATH;
	QFile fcom2(FILE_NET_SETTING);    // 文件存在
	if (false == fcom2.exists())
	{
		printf("file not exit %s \r\n", FILE_NET_SETTING);
		QSettings settings(FILE_NET_SETTING , QSettings::IniFormat);
		settings.beginGroup("CommunicationNerwork");
		settings.setValue("ServiceIP","0.0.0.0");
		settings.setValue("NetPort","1000");
		settings.setValue("workmode","0");  // 工作模式
		settings.endGroup();

	}
    //qDebug()<<"UDPCommunication ================222"+configFileDirectory;
}
/*
  函数名称：发送数据
  */
void UDPCommunication::writeDatagramMsg(QString strData)
{
    //qDebug()<<"writeDatagramMsg===[1111]===\r\n";
   getServeIP();
   datagram.clear();
   datagram=strData.toLatin1();
   qDebug()<<datagram;
   qDebug()<<serverAddress;
   qDebug()<<serverPort;
   sender->writeDatagram(datagram.data(),datagram.size(),serverAddress,serverPort);
//   qDebug()<<"send datagram!";
}
/*
  函数名称：获取本机IP
  */
QString UDPCommunication::getClientIP()
{
    QString strLocalIP;
 //   qDebug()<<"getClientIP === [11111]=====\r\n";
    QList<QHostAddress> addrs =QNetworkInterface::allAddresses();
    for(int i=0;i<addrs.size();i++)
    {
        if(addrs.at(i).protocol()==QAbstractSocket::IPv4Protocol&&
                addrs.at(i)!=QHostAddress::Null&&
                addrs.at(i)!=QHostAddress::LocalHost)
        {
            strLocalIP=addrs.at(i).toString();
            qDebug()<<strLocalIP;
            return strLocalIP;
        }
//        qDebug()<<addrs.at(i).toString();
    }
 //    qDebug()<<"getClientIP === [222]=====\r\n";
}
// 获取IP地址
void UDPCommunication::getServeIP()
{

    QString ServeIP;
    QString ServePort;

	qDebug()<< "getServeIP =====[111]=1== \r\n"<<FILE_NET_SETTING;

	QSettings settings(FILE_NET_SETTING, QSettings::IniFormat);
   qDebug()<< "getServeIP =====[111]=== \r\n";
    settings.beginGroup("CommunicationNerwork");
	ServeIP = settings.value("ServiceIP","0.0.0.0").toString();
	qDebug()<< "getServeIP =====[222]=== \r\n";
	ServePort = settings.value("NetPort","1000").toString();
	qDebug()<< "getServeIP =====[333]=== \r\n";
    settings.endGroup();
	qDebug()<<"ServeIP"<<ServeIP;
	qDebug()<<"ServePort"<<ServePort;

    //格式转换
    serverAddress = QHostAddress(QstringTostring(ServeIP));
    serverPort = ServePort.toInt();
	qDebug()<<"nserverPort"<<serverPort;
}

// 获取工作模式
int UDPCommunication::getWorkMode()
{
	int workmode;


	QSettings settings(FILE_NET_SETTING, QSettings::IniFormat);
	settings.beginGroup("CommunicationNerwork");

	workmode = settings.value("workmode","0").toInt();

	settings.endGroup();

	return workmode;
}


