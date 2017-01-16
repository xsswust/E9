#ifndef NETSETTING_H
#define NETSETTING_H

#include <QWidget>
#include "src/HardWare/keyboard_dependence.h"

typedef struct
{
   QString localIP;
   QString serveIP;
   QString servePort;
}CommunicationNetWork;


namespace Ui {
class NetSetting;
}

class NetSetting : public QWidget
{
	Q_OBJECT

public:
	explicit NetSetting(QWidget *parent = 0);
	~NetSetting();
	void NetSetting_init();   // 初始化函数
	void NetSetting_Uninit();   // 界面跳转函数
	bool exit_flag;

	CommunicationNetWork UDPCommunicationInfo;  //UPD 相关

public slots:
	void NetSetting_KeyValue(QString str);   // 键盘数据回传

private slots:
	void on_pb_Exit_clicked();  // 返回按钮
	void on_pb_Set_clicked();  // 设置按钮

private:
	Ui::NetSetting *ui;
	Keyboard_dependence *im;  // 键盘
};

#endif // NETSETTING_H
