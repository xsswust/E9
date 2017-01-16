#ifndef COMTRAN_H
#define COMTRAN_H

#include <QString>
#include <QWidget>
#include "src/HardWare/posix_qextserialport.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


namespace Ui {
class ComTran;
}

class ComTran : public QWidget
{
    Q_OBJECT
    
public:
    explicit ComTran(QWidget *parent = 0);
    ~ComTran();
    QString send2_message;
    QString receive2_message;

    int send2_len;
	//Posix_QextSerialPort *m_pCom2;
	 QSerialPort *m_pCom2;//串口声明
//    void floatToChar4(char* arrary,float write_data);// 将float转换为4个字节
//    float char4ToFloat(char* arrary,float write_data);//将4个字节转换为float

#if 0
	 int SendCount;//发送数据计数
	 int ReceiveCount;//接收数据计数
	 bool IsShow;//是否显示数据
	 bool IsDebug;//是否启用调试,接收到数据后模拟发送数据
	 bool IsAutoClear;//是否自动清空
	 bool IsHexSend;//是否16进制数据发送
	 bool IsHexReceive;//是否16进制数据接收
#endif
#if 0
	 QString str_baudRate;
	 QString str_dataBit;
	 QString str_parity;
	 QString str_stopBit;
	 QString str_flowControl;
#endif
	 qint32 BaudRate;
	 QSerialPort::DataBits DataBit;
	 QSerialPort::Parity Parity;
	 QSerialPort::StopBits StopBit;
	 QSerialPort::FlowControl FlowControl;
private slots:
    void on_pbExit_clicked();
    void on_pbSend_clicked();
    void on_pbReceive_clicked();
    void on_pbRecHex_clicked();
    void recData();

private:
    Ui::ComTran *ui;
};

#endif // COMTRAN_H
