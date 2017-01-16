#ifndef TRANDATA_H
#define TRANDATA_H
#include "src/HardWare/posix_qextserialport.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "common.h"

#define TEST_QSerialPort 1

class TranData
{
public:
    TranData();
    virtual ~TranData();
#if !TEST_QSerialPort
	Posix_QextSerialPort *g_pCom6;
#else
	QSerialPort *g_pCom6;
	QSerialPort *g_pCom5;
#if HAVE_PRINTF
    QSerialPort *g_pCom3;
#endif

#endif
    void com6_open(void);
    void com6_close(void);

	int com6_send(const char *c,int len);
	int com6_receive(quint8 *tmp_data, int l);
	int com6_receive_all(void);
	QByteArray com6_recHex();

    void com5_open(void);
    void com5_close(void);
	int com5_send(const char *c,int len);
	int com5_receive(quint8 *tmp_data, int l);
	int com5_receive_all(void);
	QByteArray com5_recHex();

#if HAVE_PRINTF
    void com3_open(void);
    void com3_close(void);
    int com3_send(const char *c,int len);
    int com3_receive(quint8 *tmp_data, int l);
    int com3_receive_all(void);
    //QByteArray com5_recHex();
#endif

	qint32 BaudRate;
	QSerialPort::DataBits DataBit;
	QSerialPort::Parity Parity;
	QSerialPort::StopBits StopBit;
	QSerialPort::FlowControl FlowControl;

};

extern TranData *pTranData;
#endif // TRANDATA_H
