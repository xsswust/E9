#include "comtran.h"
#include "ui_comtran.h"
#include "src/HardWare/com_trandata.h"
#include "src/HardWare/app.h"
#include "src/HardWare/posix_qextserialport.h"
#include "src/Thread/comtran_thread.h"
#include <stdio.h>
#include <QTextEdit>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>

ComTran::ComTran(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComTran)
{
    ui->setupUi(this);
#if 0
	//QFile fcom2("/dev/ttySAC2");    // COM2 初始化
	QFile fcom2("/dev/ttymxc1");    // COM2 初始化
    if (fcom2.exists())
    {
#if 0
		//m_pCom2 = new Posix_QextSerialPort("/dev/ttySAC2", QextSerialBase::Polling);
		m_pCom2 = new Posix_QextSerialPort("/dev/ttymxc2", QextSerialBase::Polling);
        if (m_pCom2->open(QIODevice::ReadWrite))
        {
            m_pCom2->setBaudRate(BAUD115200);
            m_pCom2->setDataBits(DATA_8);
            m_pCom2->setParity(PAR_NONE);
            m_pCom2->setStopBits(STOP_1);
            m_pCom2->setFlowControl(FLOW_OFF);
            m_pCom2->setTimeout(10);
        }
#else
		BaudRate = QSerialPort::Baud9600;  // 波特率 9600
		Parity = QSerialPort::NoParity;				//检验位 none
		DataBit = QSerialPort::Data8;				// 数据位	8
		StopBit = QSerialPort::OneStop;			// 停止位	1
		FlowControl = QSerialPort::NoFlowControl;  // 控制流	none

		m_pCom2  = new QSerialPort();
		//qDebug()<<ui->cboxPortName->currentText();
		m_pCom2->setPortName("ttymxc1");  // 串口号
		if(m_pCom2->open(QIODevice::ReadWrite))
		{
			m_pCom2->setBaudRate(BaudRate);
			m_pCom2->setDataBits(DataBit);
			m_pCom2->setParity(Parity);
			m_pCom2->setStopBits(StopBit);
			m_pCom2->setFlowControl(FlowControl);
		}
#endif
    }
#endif
    ui->teSend->setAcceptRichText(true);
    connect(pcomtran_thread,SIGNAL(emitcontranTest()),this,SLOT(recData()));
}

ComTran::~ComTran()
{
    delete ui;
}

/*
    函数名称：
    函数功能：返回主页面
    输入参数：
    输出参数：
    修改记录：
*/
void ComTran::on_pbExit_clicked()
{
    this->close();
}

/*
    函数名称：
    函数功能：发送数据
    输入参数：
    输出参数：
    修改记录：
*/
void ComTran::on_pbSend_clicked()
{

}

void ComTran::on_pbReceive_clicked()
{
    //ui->lineEdit->setText("1");
}

/*
    函数名称：
    函数功能：读取数据
    输出参数：
    修改记录：
*/
void ComTran::recData()
{
    ui->lineEdit->setText("1");
}

/*
    函数名称：
    函数功能：读取十六进制
    输出参数：
    修改记录：
*/
void ComTran::on_pbRecHex_clicked()
{
    QByteArray temp;
    if(m_pCom2->bytesAvailable() >= 1)
    {
        temp = m_pCom2->readAll(); //读串口缓冲区数据
        char ch[100];
        for(int i = 0;i<temp.length();i++)
        {
            ch[i] = temp.at(i);
            qDebug()<<ch[i];
        }
        //qDebug()<<temp.toHex();
    }

    QDataStream out(&temp,QIODevice::ReadWrite);    //将字节数组读入
    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out>>outChar;   //每字节填充一次，直到结束
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
        //十六进制的转换
        ui->recBrowser->insertPlainText(str);
    }
    ui->recBrowser->insertPlainText(tr("\n"));
}


//union
//{
// float sub_float;
// struct  {
//     char b[4];
//      } sep_float ;
//} u_float;



///*
//    函数名称：
//    函数功能：将4个字节转换成float
//    输入参数：
//    输出参数：
//    修改记录：
//*/
//float ComTran::char4ToFloat(char *arrary, float write_data)
//{
//    u_float.sep_float.b[0] = arrary[3];
//    u_float.sep_float.b[1] = arrary[2];
//    u_float.sep_float.b[2] = arrary[1];
//    u_float.sep_float.b[3] = arrary[0];

//    write_data = u_float.sub_float;
//    return write_data;
//}



///*
//    函数名称：
//    函数功能：将float转换成4个字节
//    输入参数：
//    输出参数：
//    修改记录：
//*/
//void ComTran::floatToChar4(char *arrary, float write_data)
//{

//    u_float.sub_float = write_data;

//    arrary[0] = u_float.sep_float.b[3];
//    arrary[1] = u_float.sep_float.b[2];
//    arrary[2] = u_float.sep_float.b[1];
//    arrary[3] = u_float.sep_float.b[0];

//}
