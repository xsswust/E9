/*
 * TranData.cpp
 *
 *  Created on: 2015-3-13
 *      Author: root
 */

#include "com_trandata.h"
#include "QFile"
#include <QString>
#include <qdebug.h>
#include "common.h"
#include "qserialport.h"
#include "qextserialbase.h"
#include "posix_qextserialport.h"


TranData *pTranData; // 定义外部调用函数
TranData::TranData()
{
    // TODO Auto-generated constructor stub
#if 0
// com 6
	QFile fcom6(RS485_DRIVE_COM6_NAME);    // COM6 初始化
	if (fcom6.exists())
    {
		BaudRate = QSerialPort::Baud9600;  // 波特率 9600
		Parity = QSerialPort::NoParity;				//检验位 none
		DataBit = QSerialPort::Data8;				// 数据位	8
//		DataBit = QSerialPort::UnknownDataBits;				// 数据位	8
		StopBit = QSerialPort::OneStop;			// 停止位	1
		FlowControl = QSerialPort::NoFlowControl;  // 控制流	none

		g_pCom6  = new QSerialPort();
		//qDebug()<<ui->cboxPortName->currentText();
		g_pCom6->setPortName(RS485_DRIVE_COM6_NAME1);  // 串口号
		//m_pCom2->getChar()
		g_pCom6->close();
		if(g_pCom6->open(QIODevice::ReadWrite))
		{
			g_pCom6->setBaudRate(BaudRate);
			g_pCom6->setDataBits(DataBit);
			g_pCom6->setParity(Parity);
			g_pCom6->setStopBits(StopBit);
			g_pCom6->setFlowControl(FlowControl);
			//g_pCom6->set
            qDebug()<<("open com6 ================ok\r\n");
		}
		else
		{
			qDebug()<<("error ================open error\r\n");
			g_System_Info = ERROR_COM6_OPEN_ERROR;
            //return ;
		}
    }
	else
	{
		qDebug()<<("error ================file not exists error\r\n");
		g_System_Info = ERROR_COM6_DRIVER_NOT_EXIST;

        //return ;
	}
// com5
	QFile fcom5(RS485_DRIVE_COM5_NAME);    // COM5 初始化
	if (fcom5.exists())
	{
		BaudRate = QSerialPort::Baud9600;  // 波特率 9600
		Parity = QSerialPort::NoParity;				//检验位 none
		DataBit = QSerialPort::Data8;				// 数据位	8
//		DataBit = QSerialPort::UnknownDataBits;				// 数据位	8
		StopBit = QSerialPort::OneStop;			// 停止位	1
		FlowControl = QSerialPort::NoFlowControl;  // 控制流	none

		g_pCom5  = new QSerialPort();
		//qDebug()<<ui->cboxPortName->currentText();
		g_pCom5->setPortName(RS485_DRIVE_COM5_NAME1);  // 串口号
		//m_pCom2->getChar()
		g_pCom5->close();
		if(g_pCom5->open(QIODevice::ReadWrite))
		{
			g_pCom5->setBaudRate(BaudRate);
			g_pCom5->setDataBits(DataBit);
			g_pCom5->setParity(Parity);
			g_pCom5->setStopBits(StopBit);
			g_pCom5->setFlowControl(FlowControl);
            //g_pCom6->set
            qDebug()<<("open com5 ================ok\r\n");
		}
		else
		{
            qDebug()<<("error =====com5===========open error\r\n");
			g_System_Info = ERROR_COM7_OPEN_ERROR;
            //return ;
		}
	}
	else
	{
        qDebug()<<("error =========com5=======file not exists error\r\n");
		g_System_Info = ERROR_COM7_DRIVER_NOT_EXIST;

        //return ;
	}
#else

	g_pCom5  = new QSerialPort();
	g_pCom6  = new QSerialPort();
#if HAVE_PRINTF
    g_pCom3  = new QSerialPort();
    com3_open();
#endif
	//com5_close();
	com5_open();

	//com6_close();
	com6_open();

#endif
}

TranData::~TranData()
{
	com5_close();
	com6_close();
#if HAVE_PRINTF
    com3_close();
#endif
//	delete g_pCom6;
//	delete g_pCom5;
    // TODO Auto-generated destructor stub
}

// 打开com6
void TranData::com6_open(void)
{
	QFile fcom6(RS485_DRIVE_COM6_NAME);    // COM6 初始化
	if (fcom6.exists())
	{
		BaudRate = QSerialPort::Baud9600;  // 波特率 9600
		Parity = QSerialPort::NoParity;				//检验位 none
		DataBit = QSerialPort::Data8;				// 数据位	8
//		DataBit = QSerialPort::UnknownDataBits;				// 数据位	8
		StopBit = QSerialPort::OneStop;			// 停止位	1
		FlowControl = QSerialPort::NoFlowControl;  // 控制流	none


		//qDebug()<<ui->cboxPortName->currentText();
		g_pCom6->setPortName(RS485_DRIVE_COM6_NAME1);  // 串口号
		//m_pCom2->getChar()
		g_pCom6->close();
		if(g_pCom6->open(QIODevice::ReadWrite))
		{
			g_pCom6->setBaudRate(BaudRate);
			g_pCom6->setDataBits(DataBit);
			g_pCom6->setParity(Parity);
			g_pCom6->setStopBits(StopBit);
			g_pCom6->setFlowControl(FlowControl);
			//g_pCom6->set
			qDebug()<<("open com6 ================ok\r\n");
		}
		else
		{
			qDebug()<<("error ================open error\r\n");
			g_System_Info = ERROR_COM6_OPEN_ERROR;
			//return ;
		}
	}
	else
	{
		qDebug()<<("error ================file not exists error\r\n");
		g_System_Info = ERROR_COM6_DRIVER_NOT_EXIST;

		//return ;
	}
	qDebug()<<("\n com6_open ============\r\n");
}
// com6 关闭
void TranData::com6_close(void)
{
	qDebug()<<("\n com6_close ============\r\n");
	g_pCom6->close();
	//delete g_pCom6;
}

// 发送数据
int TranData::com6_send(const char *c,int len)
{
	QMutex mutex;

	int l = 0;

    if((ERROR_COM6_OPEN_ERROR == g_System_Info)||(ERROR_COM6_DRIVER_NOT_EXIST == g_System_Info))
    {
        qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
		return -1;
    }
#if 0
	QString str = c;
	QByteArray outData = str.toUtf8();
	int size = outData.size();

	size = outData.size();
	g_pCom6->write(outData);
#else
//    printf("\n send ====11 \r\n");
//	qDebug()<<c;
	//mutex.lock(); // 加锁保护
	l = g_pCom6->write(c, len);
	//mutex.unlock(); // 加锁保护
//    printf("\n send ====222 \r\n");
	if(l != len)
	{
		qDebug()<<QString("com6_send error l = %1 len = %2").arg(l).arg(len);
	}
#endif

	return l;
}
// 接收数据
int TranData::com6_receive(quint8 *tmp_data, int l)
{
	//QString str;
	char data[LEN_TEMP_DATA + 1];
	int len;
	QMutex mutex;

    if((ERROR_COM6_OPEN_ERROR == g_System_Info)||(ERROR_COM6_DRIVER_NOT_EXIST == g_System_Info))
    {
        qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
        return 0;
    }

#if 0
	if(g_pCom6->bytesAvailable()<=0)
	{
		return 0;
	}//这个判断避免空闲时和会出现高内存占用
#endif
	usleep(200*1000);//延时200毫秒保证接收到的是一条完整的数据,而不是脱节的


	//str = g_pCom6->readAll();
	memset(data, 0, sizeof(data));

	//mutex.lock();  // 加锁保护
	len = g_pCom6->read(data, l);
	//mutex.unlock();

	memcpy(tmp_data,data, len);
    //qDebug()<<QString("com6 receive len = %1").arg(len);
	return len;


}

// 接收数据 all
int TranData::com6_receive_all(void)
{
	QString str;
	char data[1024];
	int len;
	QMutex mutex;


#if DEBUD_TEST
    return 0;
#endif


    if((ERROR_COM6_OPEN_ERROR == g_System_Info)||(ERROR_COM6_DRIVER_NOT_EXIST == g_System_Info))
    {
        qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
        return 0;
    }
#if 0
	if(g_pCom6->bytesAvailable()<=0)
	{
		return 0;
	}//这个判断避免空闲时和会出现高内存占用
#endif
	usleep(100*1000);//延时200毫秒保证接收到的是一条完整的数据,而不是脱节的

#if 1
	str = g_pCom6->readAll();
#else
	memset(data, 0, sizeof(data));


	//mutex.lock();  // 加锁保护
	len = g_pCom6->read(data, 1024);
	//mutex.unlock();
//	memcpy(tmp_data,data, len);
#endif
	return len;

}


// 读取 16进制数
QByteArray TranData::com6_recHex()
{
	QByteArray cData = g_pCom6->readAll();
    return cData;
}


// 打开com5
void TranData::com5_open(void)
{
	QFile fcom5(RS485_DRIVE_COM5_NAME);    // COM5 初始化
	if (fcom5.exists())
	{
		BaudRate = QSerialPort::Baud9600;  // 波特率 9600
		Parity = QSerialPort::NoParity;				//检验位 none
		DataBit = QSerialPort::Data8;				// 数据位	8
//		DataBit = QSerialPort::UnknownDataBits;				// 数据位	8
		StopBit = QSerialPort::OneStop;			// 停止位	1
		FlowControl = QSerialPort::NoFlowControl;  // 控制流	none


		//qDebug()<<ui->cboxPortName->currentText();
		g_pCom5->setPortName(RS485_DRIVE_COM5_NAME1);  // 串口号
		//m_pCom2->getChar()
		//g_pCom5->close();
		if(g_pCom5->open(QIODevice::ReadWrite))
		{
			g_pCom5->setBaudRate(BaudRate);
			g_pCom5->setDataBits(DataBit);
			g_pCom5->setParity(Parity);
			g_pCom5->setStopBits(StopBit);
			g_pCom5->setFlowControl(FlowControl);
			//g_pCom6->set
			qDebug()<<("open com5 ================ok\r\n");
		}
		else
		{
			qDebug()<<("error =====com5===========open error\r\n");
			g_System_Info = ERROR_COM7_OPEN_ERROR;
			//return ;
		}
	}
	else
	{
		qDebug()<<("error =========com5=======file not exists error\r\n");
		g_System_Info = ERROR_COM7_DRIVER_NOT_EXIST;

		//return ;
	}

	qDebug()<<("\n com5_open ============\r\n");
}
// com5 关闭
void TranData::com5_close(void)
{
	qDebug()<<("\n com5_close ============\r\n");
	g_pCom5->clearError();
	g_pCom5->close();

	//delete g_pCom5;
}

// 发送数据
int TranData::com5_send(const char *c,int len)
{
	QMutex mutex;

	int l;

    if((ERROR_COM7_OPEN_ERROR == g_System_Info)||(ERROR_COM7_DRIVER_NOT_EXIST == g_System_Info))
    {
        qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
		return -1;
    }
#if 0
	QString str = c;
	QByteArray outData = str.toUtf8();
	int size = outData.size();

	size = outData.size();
	g_pCom5->write(outData);
#else
//	printf("\n send ====11 \r\n");
//	qDebug()<<c;
	//mutex.lock(); // 加锁保护
	l = g_pCom5->write(c, len);
	//mutex.unlock(); // 加锁保护
//	printf("\n send ====222 \r\n");
	if(l != len)
	{
		qDebug()<<QString("com5 error l= %1 len = %2").arg(l).arg(len);
	}
#endif

	return l;
}
// 接收数据
int TranData::com5_receive(quint8 *tmp_data, int l)
{
	//QString str;
	char data[LEN_TEMP_DATA + 1];
	int len;
	QMutex mutex;

    if((ERROR_COM7_OPEN_ERROR == g_System_Info)||(ERROR_COM7_DRIVER_NOT_EXIST == g_System_Info))
    {
        qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
        return 0;
    }
#if 0
	if(g_pCom5->bytesAvailable()<=0)
	{
		return 0;
	}//这个判断避免空闲时和会出现高内存占用
#endif
	usleep(200*1000);//延时200毫秒保证接收到的是一条完整的数据,而不是脱节的


	//str = g_pCom6->readAll();
	memset(data, 0, sizeof(data));
//
	//mutex.lock();  // 加锁保护
	len = g_pCom5->read(data, l);
	//mutex.unlock();

	memcpy(tmp_data,data, len);

	return len;


}

// 接收数据 all
int TranData::com5_receive_all(void)
{
	QString str;
	QMutex mutex;
	//char data[1024];
	int len = 0;

    if((ERROR_COM7_OPEN_ERROR == g_System_Info)||(ERROR_COM7_DRIVER_NOT_EXIST == g_System_Info))
    {
        qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
        return 0;
    }
#if 0
	if(g_pCom5->bytesAvailable()<=0)
	{
		return 0;
	}//这个判断避免空闲时和会出现高内存占用
#endif
//	usleep(200*1000);//延时200毫秒保证接收到的是一条完整的数据,而不是脱节的

#if 1
	str = g_pCom5->readAll();
#else
	memset(data, 0, sizeof(data));

	//mutex.lock();  // 加锁保护
	len = g_pCom5->read(data, l);
//	mutex.unlock();
	memcpy(tmp_data,data, len);
#endif
	return len;
}

// 读取 16进制数
QByteArray TranData::com5_recHex()
{
    if((ERROR_COM7_OPEN_ERROR == g_System_Info)||(ERROR_COM7_DRIVER_NOT_EXIST == g_System_Info))
    {
        qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
    }
    else{
        QByteArray cData = g_pCom5->readAll();
        return cData;
    }
}

#if HAVE_PRINTF

// 打开com3  打印机
void TranData::com3_open(void)
{
    QFile fcom3(RS232_DRIVE_COM2_NAME);    // COM2 初始化
    if (fcom3.exists())
    {
        BaudRate = QSerialPort::Baud9600;  // 波特率 9600
        Parity = QSerialPort::NoParity;				//检验位 none
        DataBit = QSerialPort::Data8;				// 数据位	8
//		DataBit = QSerialPort::UnknownDataBits;				// 数据位	8
        StopBit = QSerialPort::OneStop;			// 停止位	1
        FlowControl = QSerialPort::NoFlowControl;  // 控制流	none


        //qDebug()<<ui->cboxPortName->currentText();
        g_pCom3->setPortName(RS232_DRIVE_COM2_NAME1);  // 串口号
        //m_pCom2->getChar()
        //g_pCom5->close();
        if(g_pCom3->open(QIODevice::ReadWrite))
        {
            g_pCom3->setBaudRate(BaudRate);
            g_pCom3->setDataBits(DataBit);
            g_pCom3->setParity(Parity);
            g_pCom3->setStopBits(StopBit);
            g_pCom3->setFlowControl(FlowControl);

            qDebug()<<("open com3 ================ok\r\n");
        }
        else
        {
            qDebug()<<("error =====com3===========open error\r\n");
            g_System_Info = ERROR_COM7_OPEN_ERROR;
            //return ;
        }
    }
    else
    {
        qDebug()<<("error =========com5=======file not exists error\r\n");
        g_System_Info = ERROR_COM7_DRIVER_NOT_EXIST;

        //return ;
    }

    qDebug()<<("\n com3_open ============\r\n");
}
// com3 关闭
void TranData::com3_close(void)
{
    qDebug()<<("\n com3_close ============\r\n");
    g_pCom3->clearError();
    g_pCom3->close();

    //delete g_pCom5;
}



// 发送数据
int TranData::com3_send(const char *c,int len)
{
    QMutex mutex;

    int l;

//    if((ERROR_COM7_OPEN_ERROR == g_System_Info)||(ERROR_COM7_DRIVER_NOT_EXIST == g_System_Info))
//    {
//        qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
//        return -1;
//    }
#if 0
    QString str = c;
    QByteArray outData = str.toUtf8();
    int size = outData.size();

    size = outData.size();
    g_pCom5->write(outData);
#else
    for(l=0; l < len; l++)
    {
        printf("com3_send  c[%d] = %x \r\n", l, c[l]);
    }
     printf("com3_send  \r\n\r\n");
    l = g_pCom3->write(c, len);
    if(l != len)
    {
        qDebug()<<QString("com3 error l= %1 len = %2").arg(l).arg(len);
    }
#endif

    return l;
}
// 接收数据
int TranData::com3_receive(quint8 *tmp_data, int l)
{
    //QString str;
    char data[LEN_TEMP_DATA + 1];
    int len;
    QMutex mutex;

//    if((ERROR_COM7_OPEN_ERROR == g_System_Info)||(ERROR_COM7_DRIVER_NOT_EXIST == g_System_Info))
 //   {
  //      qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
  //      return 0;
  //  }
#if 0
    if(g_pCom5->bytesAvailable()<=0)
    {
        return 0;
    }//这个判断避免空闲时和会出现高内存占用
#endif
    usleep(200*1000);//延时200毫秒保证接收到的是一条完整的数据,而不是脱节的


    //str = g_pCom6->readAll();
    memset(data, 0, sizeof(data));
//
    //mutex.lock();  // 加锁保护
    len = g_pCom3->read(data, l);
    //mutex.unlock();

    memcpy(tmp_data,data, len);

    return len;


}

// 接收数据 all
int TranData::com3_receive_all(void)
{
    QString str;
    QMutex mutex;
    //char data[1024];
    int len = 0;

//    if((ERROR_COM7_OPEN_ERROR == g_System_Info)||(ERROR_COM7_DRIVER_NOT_EXIST == g_System_Info))
//    {
 //       qDebug()<<QString("error ==========g_System_Info = %1\r\n").arg(g_System_Info);
 //       return 0;
 //   }
#if 0
    if(g_pCom5->bytesAvailable()<=0)
    {
        return 0;
    }//这个判断避免空闲时和会出现高内存占用
#endif
//	usleep(200*1000);//延时200毫秒保证接收到的是一条完整的数据,而不是脱节的

#if 1
    str = g_pCom3->readAll();
#else
    memset(data, 0, sizeof(data));

    //mutex.lock();  // 加锁保护
    len = g_pCom5->read(data, l);
//	mutex.unlock();
    memcpy(tmp_data,data, len);
#endif
    return len;
}

#endif

