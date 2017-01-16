#include <QApplication>
#include "mainwindow.h"
#include "src/HardWare/com_trandata.h"
#include "src/Thread/comtran_thread.h"
#include "src/Thread/com5tran_thread.h"
#include "testing_thread.h"
#include "UdpCommunication.h"
#include "sql_control.h"
#include "common.h"

//#include <QWSServer>



ST_TEST_INFO gst_Test_Info[MAX_TEST_NUM];
ST_TEST_INFO pSt_CalibrateInfo[MAX_TEST_NUM];

void Get_Sys_st(void)
{
	QFile file(FILE_SYS_ST);
	file.open(QIODevice::ReadOnly);

	int flag = 0;

	file.seek(0);
    file.read((char*)&flag, sizeof(flag));//读flag个字节大小的数传给flag
	file.read((char*)&g_Test_Over_Time, sizeof(g_Test_Over_Time));
    file.read((char*)&g_mpn_test_st, sizeof(g_mpn_test_st));
    file.read((char*)&g_mpn_test_mui, sizeof(g_mpn_test_mui));
    file.read((char*)&g_mpn_test_val, sizeof(g_mpn_test_val));

    printf("\n[Get_Sys_st] g_mpn_test_st == %d g_mpn_test_mui = %d", g_mpn_test_st, g_mpn_test_mui);
    qDebug()<<QString("\n[Get_Sys_st]  g_mpn_test_st ==%1").arg( g_mpn_test_st);

    qDebug()<<"\n[GetF_Sys_st]   g_mpn_test_mui ==" << g_mpn_test_mui;
	if(g_Test_Over_Time <= 0)
	{
		g_Test_Over_Time = FAST_TIME_LIMIT;
	}
	if(true == flag)
	{
		g_test_st = flag;
		file.read((char*)&gst_Test_Info, sizeof(ST_TEST_INFO)*MAX_TEST_NUM);
		for(int i = 0; i < MAX_TEST_NUM; i++)
		{
            qDebug()<<QString("gst_Test_Info[%1] = %2 ").arg(i).arg(gst_Test_Info[i].Test_st);
        //    qDebug()<<"\r\n  gst_Test_Info[] =  "+gst_Test_Info[i].Test_st;
		}

        file.read((char*)&pSt_CalibrateInfo, sizeof(ST_TEST_INFO)*MAX_TEST_NUM);
        for(int i = 0; i < MAX_TEST_NUM; i++)
        {
            qDebug()<<QString("&pSt_CalibrateInfo[%1] = %2 ").arg(i).arg(pSt_CalibrateInfo[i].Test_st);
        }

		file.read((char *)&gst_His_Info_to_plc, sizeof(ST_HISTORY_DATA)*BOTTLE_MAX_NUM);
		for(int i = 0; i < BOTTLE_MAX_NUM; i++)
		{
			qDebug()<<QString("gst_His_Info_to_plc.bt_id[%1] = %2 ").arg(i).arg(gst_His_Info_to_plc[i].bt_id);
		}

        file.read((char *)&g_work_Test_type, sizeof(g_work_Test_type));
		file.read((char *)&gst_His_Info, sizeof(gst_His_Info));
	}
	else
	{
		qDebug()<<QString("no data = ================== ");
	}

	file.close();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 
    //QT_NO_CURSOR;
    QApplication::setOverrideCursor(Qt::BlankCursor);  // 去除鼠标显示

    //QWSServer::setCursorVisible(FALSE);  // 去除鼠标
	//注册MessageHandler  保存 log 信息
#if 0//!DEBUD_TEST
	qInstallMessageHandler(outputMessage);
#endif
    QDateTime time= QDateTime::currentDateTime();
    QString strDate= time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"systemm start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<strDate;

	HardWare_Init();  //硬件初始化

	memset(gst_His_Info, 0, sizeof(ST_HISTORY_DATA)*MAX_TEST_NUM);
	memset(gst_Test_Info, 0, sizeof(ST_TEST_INFO)*MAX_TEST_NUM);
	memset(gst_His_Info_to_plc, 0, sizeof(ST_TEST_INFO)*BOTTLE_MAX_NUM);
	// 打开sql 数据库
	gp_sql = new sql_control();			// 建立sql
	// 串口控制
	pTranData = new TranData();

	// 创建com5 通讯线程
	pcom5tran_thread = new Com5Tran_Thread();
	pcom5tran_thread->start();  //线程运行

	// 创建com6 通讯线程
	pcomtran_thread=new ComTran_Thread();
	pcomtran_thread->start();  //线程运行

	// 创建检测线程
	ptesting_thread =new testing_thread();
	ptesting_thread->start();  //线程运行

	// 建立UDP 发送
	pUDPCommunication=new UDPCommunication();

	g_workmode = pUDPCommunication->getWorkMode();
	// 初始化 全局变量
	memset(&gst_Test_Info, 0, sizeof(ST_TEST_INFO)*MAX_TEST_NUM);
    memset(&pSt_CalibrateInfo, 0, sizeof(ST_TEST_INFO)*MAX_TEST_NUM);
	for(int i= 0; i < MAX_TEST_NUM; i++)
	{
		gst_Test_Info[i].Test_st = TEST_ST_IDEL;
		//gst_Test_Info[i].Test_st = TEST_ST_TESTING;
	}

	// 获取系统状态
    Get_Sys_st();


	//MainWindow mainwin;  // 主界面
	pmainwin = new MainWindow();
	pmainwin->MainWindow_Init();  // 初始化界面
	pmainwin->showFullScreen();

    
    return a.exec();

	HardWare_UnInit();  //硬件初始化
#if 0
    QPixmap fullScreenPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    fullScreenPixmap->save("fullScreen.jpg");
#endif
}
