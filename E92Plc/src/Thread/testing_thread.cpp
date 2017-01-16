#include "testing_thread.h"
#include "common.h"
#include "sql_control.h"

testing_thread *ptesting_thread;

testing_thread::testing_thread()
{
	timer_flag = false;  //timer 没有启动
	time_Testing = new QTimer();
	timer_out_flag = false;  // 时间到
//	time_Testing->start(TIMER_LEN_1MINUTE);  // 定时一分钟
//	time_Testing->start(TIMER_LEN_1MINUTE);  // 定时一分钟
//	timer_flag = true;  //timer 没有启动
	// 建立连接
	connect(time_Testing,SIGNAL(timeout()),this,SLOT(timeUp_Testing()));  //

}


testing_thread::~testing_thread()
{
	//delete timeupTimer;
}


// 线程运行
void testing_thread::run()
{
	int i= 0, idle_num = 0;

	msleep(10*1000); // 10S
	while(1)
	{
		msleep(100); //100ms
		idle_num = 0;
		for(i = 0; i < MAX_TEST_NUM; i++)
		{
			if(TEST_ST_ZERO == gst_Test_Info[i].Test_st)  // 较零 处理
			{
				gp_sql->Del_All_Data_for_ad(i);
				//qDebug()<<QString("num = %1 check ok \r\n").arg(i);
				if(true ==  Check_SelfSt(&gst_Test_Info[i] ,i))  // 自检通过
				{
					qDebug()<<QString("num = %1 check ok \r\n").arg(i);
					Zero_Calibration(&gst_Test_Info[i]);
					//gst_Test_Info[i].sThouData_Cnt = 1075;
				}
			}
			else if(TEST_ST_IDEL == gst_Test_Info[i].Test_st)
			{
				idle_num ++;
			}
			else
			{
				idle_num = 0;
			}
		}
		//qDebug()<<QString("idle_num = %1 MAX_TEST_NUM = %2 timer_flag = %3").arg(idle_num).arg(MAX_TEST_NUM).arg(timer_flag);
		if((idle_num < MAX_TEST_NUM) && (false == timer_flag))
		{
#if DEBUD_TEST
			time_Testing->start(TIMER_LEN_5S);  // 定时一分钟
#else
			time_Testing->start(TIMER_LEN_1MINUTE);  // 定时一分钟
#endif
			//time_Testing->start(20*1000);  // 定时一分钟
			timer_flag = true;
			g_test_st = timer_flag;
		}
		else if(idle_num == MAX_TEST_NUM)
		{
			if(true == timer_flag){
				time_Testing->stop();  //定时取消
				timer_flag = false;
				g_test_st = timer_flag;
			}
		}

		if(true == timer_out_flag){// 开始测试
			QDateTime time;
			QString strDate;

			timer_out_flag = false;

			time= QDateTime::currentDateTime();
			strDate = time.toString("yyyy-MM-dd hh:mm:ss");

			qDebug()<<"^^^start "<<strDate;
			Testing();

			time= QDateTime::currentDateTime();
			strDate = time.toString("yyyy-MM-dd hh:mm:ss");
			qDebug()<<"^^^end "<<strDate;
		}
	}
}
// 测试
void testing_thread::Testing()
{
	int i, id = 0;

	ST_HISTORY_DATA st_his_data;

	QDateTime time= QDateTime::currentDateTime();
	QString strDate= time.toString("yyyy-MM-dd hh:mm:ss");


	st_his_data.SampleID = strDate;

	for(i = 0; i < ONE_TEST_NUM; i++) // 同时检测5路
	{
		if((TOL_COLI_ID -1) == i/MAX_ONE_GROUP_NUM)  // 总大肠
		{
			id = i%MAX_ONE_GROUP_NUM + TOLCOLI_START_NUM;
			if((TEST_ST_TESTING == gst_Test_Info[id].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st))
			{
				// 获取 暗值
				LED_Power( 0, GPIO_LOW);  // 灯全部灭
				usleep(LED_POWER_TIMERLEN);  // 500m秒 延时
				if(TEST_ST_TESTING == gst_Test_Info[id].Test_st)  //第一组
				{
					gst_Test_Info[id].Ad_DarkData_Cur = Get_ad(id);
                    qDebug()<<QString("gst_Test_Info[%1].Ad_DarkData_Cur == %2").arg(id).arg(gst_Test_Info[id].Ad_DarkData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id + MAX_ONE_GROUP_NUM].Test_st) //第二组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
				}
				// 获取 亮值
				LED_Power( id, GPIO_HIGH);
				usleep(LED_POWER_TIMERLEN);  // 2秒 延时

				if(TEST_ST_TESTING == gst_Test_Info[id].Test_st)  //第一组
				{
					gst_Test_Info[id].Ad_LightData_Cur = Get_ad(id);
                    //qDebug()<<QString("gst_Test_Info[%1].Ad_LightData_Cur == %2").arg(id).arg(gst_Test_Info[id].Ad_LightData_Cur);
					ThruRate_Calculate(&gst_Test_Info[id]);
					ThouData_Smooth(&gst_Test_Info[id]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id, &st_his_data);
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 ******** \r\n").arg(id).arg(gst_Test_Info[id].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+ MAX_ONE_GROUP_NUM].Test_st) //第二组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM);

					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
					ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
					ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*2, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*2).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
					ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*3, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*3).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
					ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*4, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*4).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].ThouData_Cur);
				}
				usleep(LED_POWER_TIMERLEN);  // 500m秒 延时
				LED_Power( 0, GPIO_LOW);  // 灯全部灭
			}
		}
		else if((FEC_COLI_ID -1) == i/MAX_ONE_GROUP_NUM)  // 耐热
		{
			id = i%MAX_ONE_GROUP_NUM + FECCOLI_START_NUM;
			if((TEST_ST_TESTING == gst_Test_Info[id ].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st))
			{
				// 获取 暗值
				LED_Power( 0, GPIO_LOW);  // 灯全部灭
				usleep(LED_POWER_TIMERLEN);  // 500m秒 延时
				if(TEST_ST_TESTING == gst_Test_Info[id].Test_st)  //第一组
				{
					gst_Test_Info[id].Ad_DarkData_Cur = Get_ad(id);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id + MAX_ONE_GROUP_NUM].Test_st) //第二组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_DarkData_Cur = Get_ad(id + MAX_ONE_GROUP_NUM );
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
				}
				// 获取 亮值
				LED_Power( id, GPIO_HIGH);
				usleep(LED_POWER_TIMERLEN);  // 2秒 延时

				if(TEST_ST_TESTING == gst_Test_Info[id].Test_st)  //第一组
				{
					gst_Test_Info[id].Ad_LightData_Cur = Get_ad(id);
					ThruRate_Calculate(&gst_Test_Info[id]);
					ThouData_Smooth(&gst_Test_Info[id]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id, &st_his_data);
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 ******** \r\n").arg(id).arg(gst_Test_Info[id].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+ MAX_ONE_GROUP_NUM].Test_st) //第二组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM);

					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
					ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
					ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*2, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*2).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
					ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*3, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*3).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
					ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*4, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*4).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].ThouData_Cur);
				}
				usleep(LED_POWER_TIMERLEN);  // 500m秒 延时
				LED_Power( 0, GPIO_LOW);  // 灯全部灭
			}
		}
		else if((TPC_COLI_ID -1) == i/MAX_ONE_GROUP_NUM)  // 菌落总数
		{
			id = i%MAX_ONE_GROUP_NUM + TPCCOLI_START_NUM;
			if((TEST_ST_TESTING == gst_Test_Info[id ].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st))
			{
				// 获取 暗值
				LED_Power( 0, GPIO_LOW);  // 灯全部灭
				usleep(LED_POWER_TIMERLEN);  // 500m秒 延时
				if(TEST_ST_TESTING == gst_Test_Info[id].Test_st)  //第一组
				{
					gst_Test_Info[id].Ad_DarkData_Cur = Get_ad(id);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id + MAX_ONE_GROUP_NUM].Test_st) //第二组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_DarkData_Cur = Get_ad(id + MAX_ONE_GROUP_NUM );
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
				}
				// 获取 亮值
				LED_Power( id, GPIO_HIGH);
				usleep(LED_POWER_TIMERLEN);  // 2秒 延时

				if(TEST_ST_TESTING == gst_Test_Info[id].Test_st)  //第一组
				{
					gst_Test_Info[id].Ad_LightData_Cur = Get_ad(id);
					ThruRate_Calculate(&gst_Test_Info[id]);
                    //ThouData_Smooth(&gst_Test_Info[id]);
                    E_Test_Cal(&gst_Test_Info[id]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id, &st_his_data);
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 ******** \r\n").arg(id).arg(gst_Test_Info[id].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+ MAX_ONE_GROUP_NUM].Test_st) //第二组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM);

					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*2, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*2).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*3, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*3).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*4, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*4).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].ThouData_Cur);
				}
				usleep(LED_POWER_TIMERLEN);  // 500m秒 延时
				LED_Power( 0, GPIO_LOW);  // 灯全部灭
			}
		}
		else if((E_COLI_ID -1) == i/MAX_ONE_GROUP_NUM)  // 埃希氏
		{
			id = i%MAX_ONE_GROUP_NUM + ECOLI_START_NUM;
			if((TEST_ST_TESTING == gst_Test_Info[id ].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st)||
				(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st))
			{
				// 获取 暗值
				LED_Power( 0, GPIO_LOW);  // 灯全部灭
				usleep(LED_POWER_TIMERLEN);  // 500m秒 延时
				if(TEST_ST_TESTING == gst_Test_Info[id].Test_st)  //第一组
				{
					gst_Test_Info[id].Ad_DarkData_Cur = Get_ad(id);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id + MAX_ONE_GROUP_NUM].Test_st) //第二组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_DarkData_Cur = Get_ad(id + MAX_ONE_GROUP_NUM );
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_DarkData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
				}
				// 获取 亮值
				LED_Power( id, GPIO_HIGH);
				usleep(LED_POWER_TIMERLEN);  // 2秒 延时

				if(TEST_ST_TESTING == gst_Test_Info[id].Test_st)  //第一组
				{
					gst_Test_Info[id].Ad_LightData_Cur = Get_ad(id);
					ThruRate_Calculate(&gst_Test_Info[id]);
                    //ThouData_Smooth(&gst_Test_Info[id]);
                    E_Test_Cal(&gst_Test_Info[id]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id, &st_his_data);
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 ******** \r\n").arg(id).arg(gst_Test_Info[id].ThouData_Cur);

                    //E_Test_Cal(&gst_Test_Info[id]); // 计算变化最大的点

				}
				if(TEST_ST_TESTING == gst_Test_Info[id+ MAX_ONE_GROUP_NUM].Test_st) //第二组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM);

					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM, &st_his_data);

                    //E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]); // 计算变化最大的点
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*2, &st_his_data);
                    //E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM *2]); // 计算变化最大的点
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*2).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*3, &st_his_data);
                    //E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]); // 计算变化最大的点
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*3).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].ThouData_Cur);
				}
				if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第五组
				{
					gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
					ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);

					st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
					st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
					gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*4, &st_his_data);
                    //E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]); // 计算变化最大的点
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*4).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].ThouData_Cur);
				}
				usleep(LED_POWER_TIMERLEN);  // 500m秒 延时
				LED_Power( 0, GPIO_LOW);  // 灯全部灭
			}
		}
	}
	// 计算浓度
	//gst_Test_Info[0].Test_st = TEST_ST_TESTING;
	//gst_Test_Info[0].Ad_Max_Move_Time = 100;
	//gst_Test_Info[0].ThouData_Cur = END_STANDARD;

	Check_OverTime();
}


// 埃希氏计算  菌落总数
void testing_thread::E_Test_Cal(ST_TEST_INFO *pSt_TestInfo)
{

    unsigned char j = 0;
    float  ccc = 0.0;

    if( TEST_ST_TESTING == pSt_TestInfo->Test_st)
    {
#if 1
        //qDebug()<<QString("[E_Test_Cal] pSt_TestInfo->ThouData_Cur = %1").arg(pSt_TestInfo->ThouData_Cur);
        //  透过率平滑处理
        pSt_TestInfo->sThouDataTemp[MAX_THOU_DATA] = pSt_TestInfo->ThouData_Cur;//更新数据
        for(j = 0; j < 10; j++)//移位求平均
        {
            pSt_TestInfo->sThouDataTemp[j] = pSt_TestInfo->sThouDataTemp[j+1];
            ccc = ccc+pSt_TestInfo->sThouDataTemp[j];
            //qDebug()<<QString(" pSt_TestInfo->sThouDataTemp[%1] = %2 \r\n").arg(j).arg(pSt_TestInfo->sThouDataTemp[j]);
            //qDebug()<<QString(" j = %1 cc = %2 \r\n").arg(j).arg(ccc);
        }
#endif
        //pSt_TestInfo->ThouData_Cur = ccc/MAX_THOU_DATA;
        pSt_TestInfo->ThouData_dis = ccc/MAX_THOU_DATA;
        if(pSt_TestInfo->ThouData_dis > 100.0)
        {
            pSt_TestInfo->ThouData_dis = 100.0;
        }
        else if(pSt_TestInfo->ThouData_dis < 0.0)
        {
            pSt_TestInfo->ThouData_dis = 0.0;
        }
		//qDebug()<<QString("E_Test_Cal == pSt_TestInfo->ThouData_dis = %1 \r\n").arg(pSt_TestInfo->ThouData_dis);
        //pSt_TestInfo->ThouData_Cur = 40;   // for test
        pSt_TestInfo->ThouData_Cur = (100 - pSt_TestInfo->ThouData_dis)*STANDARD_PARAM;
        if(pSt_TestInfo->ThouData_Cur < 0.0)
        {
            pSt_TestInfo->ThouData_Cur = 0.0;
        }
        pSt_TestInfo->sThouData_Cnt ++;//存储的数据个数加1
        //pSt_TestInfo->Ad_LightData_Last = pSt_TestInfo->Ad_LightData_Cur - pSt_TestInfo->Ad_LightData_Last;
        //pSt_TestInfo->Last_ThouData = pSt_TestInfo->ThouData_Cur;
        // 计算变化最快的点
		//qDebug()<<QString("[]***E_Test_Cal pSt_TestInfo->Ad_LightData_Cur= %1 pSt_TestInfo->Ad_LightData_Last = %2\r\n").arg(pSt_TestInfo->Ad_LightData_Cur).arg(pSt_TestInfo->Ad_LightData_Last);
        int move_data = pSt_TestInfo->Ad_LightData_Cur - pSt_TestInfo->Ad_LightData_Last;
		//qDebug()<<QString("[]***E_Test_Cal move_data= %1 pSt_TestInfo->Ad_Max_Move_Data = %2\r\n").arg(move_data).arg(pSt_TestInfo->Ad_Max_Move_Data);
		if(move_data > 0)
        {
			pSt_TestInfo->Ad_LightData_Last = pSt_TestInfo->Ad_LightData_Cur;  // 只记录大值

			if(move_data > pSt_TestInfo->Ad_Max_Move_Data){

				pSt_TestInfo->Ad_Max_Move_Data = move_data;
				pSt_TestInfo->Ad_Max_Move_Time = pSt_TestInfo->sThouData_Cnt;

				qDebug()<<QString("[&&&&&&&&&&&&&&&7]***E_Test_Cal move_data= %1 pSt_TestInfo->Ad_Max_Move_Data = %2 \r\n\r\n").arg(move_data).arg(pSt_TestInfo->Ad_Max_Move_Data);
			}
        }

		//qDebug()<<QString("***E_Test_Cal pSt_TestInfo->Ad_Max_Move_Data = %1 pSt_TestInfo->Ad_Max_Move_Time = %2\r\n").arg(pSt_TestInfo->Ad_Max_Move_Data).arg(pSt_TestInfo->Ad_Max_Move_Time);

		//qDebug()<<QString("***E_Test_Cal ThouData_Cur = %1 \r\n").arg(pSt_TestInfo->ThouData_Cur);
		//qDebug()<<QString("<E_Test_Cal> pSt_TestInfo->Ad_LightData_Begin = %1 pSt_TestInfo->Ad_LightData_Cur = %2\r\n").arg(pSt_TestInfo->Ad_LightData_Begin).arg(pSt_TestInfo->Ad_LightData_Cur);
    }
}
// 正常测试
void testing_thread::timeUp_Testing()
{
	timer_out_flag = true;
}


/********************************************************************************
* 名称：Check_SelfCheckSt()
* 功能：检查盘上状态 自检
* 入口参数：无
* 出口参数：无
*********************************************************************************/
bool testing_thread::Check_SelfSt(ST_TEST_INFO *pSt_TestInfo, unsigned int num)
{

	//float aa= 0, bb= 0, cc= 0;
	int aa= 0, bb= 0, cc= 0;
	int flag = false;


	LED_Power( 0, GPIO_LOW);  // 灯全部灭
	usleep(LED_POWER_TIMERLEN);  // 2秒 延时
	//采集该通道暗信号
	pSt_TestInfo->Ad_DarkData_Cur = Get_ad(num);
	 // 灯 亮 //打开该通道光源
	LED_Power( num, GPIO_HIGH);
	usleep(LED_POWER_TIMERLEN);  // 2秒 延时
	//采集该通道亮信号
	pSt_TestInfo->Ad_LightData_Cur = Get_ad(num);
	qDebug()<<QString("<1>pSt_TestInfo->Ad_LightData_Cur = %1 pSt_TestInfo->Ad_DarkData_Cur = %2 \r\n").arg(pSt_TestInfo->Ad_LightData_Cur).arg(pSt_TestInfo->Ad_DarkData_Cur);
	//记录第一次数据
	aa = pSt_TestInfo->Ad_LightData_Cur - pSt_TestInfo->Ad_DarkData_Cur;
	if(aa < 0.0)
	{
		aa = 0.0;
	}

	// 再测一次
	LED_Power( 0, GPIO_LOW);  // 灯全部灭
	usleep(LED_POWER_TIMERLEN);  // 2秒 延时
	//采集该通道暗信号
	pSt_TestInfo->Ad_DarkData_Cur = Get_ad(num);
	 // 灯 亮 //打开该通道光源
	LED_Power( num, GPIO_HIGH);
	usleep(LED_POWER_TIMERLEN);  // 2秒 延时
	//采集该通道亮信号
	pSt_TestInfo->Ad_LightData_Cur = Get_ad(num);
	qDebug()<<QString("<2>pSt_TestInfo->Ad_LightData_Cur = %1 pSt_TestInfo->Ad_DarkData_Cur = %2 \r\n").arg(pSt_TestInfo->Ad_LightData_Cur).arg(pSt_TestInfo->Ad_DarkData_Cur);
	//记录第二次数据
	bb = pSt_TestInfo->Ad_LightData_Cur - pSt_TestInfo->Ad_DarkData_Cur;
	if(bb < 0)
	{
		bb = 0;
	}
	qDebug()<<QString("<1>aa = %1 bb = %2 cc== %3 \r\n").arg(aa).arg(bb).arg(cc);
	if(aa != 0){
		cc = (bb*100)/aa;
		qDebug()<<QString("<22>aa = %1 bb = %2 cc== %3 \r\n").arg(aa).arg(bb).arg(cc);
	}
	else{
		qDebug()<<QString("<2>aa = %1 bb = %2 cc== %3 \r\n").arg(aa).arg(bb).arg(cc);
		cc = 0;
	}
	qDebug()<<QString("aa = %1 bb = %2 cc== %3 \r\n").arg(aa).arg(bb).arg(cc);
	if(num >= TPCCOLI_START_NUM)  //
	{
		qDebug()<<QString("pSt_TestInfo->Ad_LightData_Cur = %1 \r\n").arg(pSt_TestInfo->Ad_LightData_Cur);
		LED_Power( 0, GPIO_LOW);  // 灯全部灭
		return true;
	}
	if((cc <(FULL_STANDARD + ZERO_STANDARD))&&(cc > (FULL_STANDARD - ZERO_STANDARD)))//有效数据(判断数据的变化率不超过正负10%) 则转到完成
	{
		aa=0;
		bb=0;
		cc=0;

		flag = true;
	}
	usleep(LED_POWER_TIMERLEN);  // 2秒 延时
	LED_Power( 0, GPIO_LOW);  // 灯全部灭
	//return true;
	return flag;
}




/*******************************************************************************
* 名称：Zero_Calibration()
* 功能：校零
* 入口参数：样本信息
* 出口参数：无
*********************************************************************************/
void testing_thread::Zero_Calibration(ST_TEST_INFO *pSt_TestInfo)
{
	int i = 0;
	//
	qDebug()<<QString("pSt_TestInfo->Ad_DarkData_Cur = %1 pSt_TestInfo->Ad_LightData_Cur= %2").arg(pSt_TestInfo->Ad_DarkData_Cur).arg(pSt_TestInfo->Ad_LightData_Cur);
	pSt_TestInfo->Ad_DarkData_Begin = pSt_TestInfo->Ad_DarkData_Cur;
	pSt_TestInfo->Ad_LightData_Begin = pSt_TestInfo->Ad_LightData_Cur;
//	pSt_TestInfo->Ad_LightData_Begin = 8000;
	pSt_TestInfo->Ad_LightData_Last = pSt_TestInfo->Ad_LightData_Begin;

	for(i = 0; i < MAX_THOU_DATA; i++)  // 默认是 100
	{
		pSt_TestInfo->sThouDataTemp[i] = 100.0;
	}
	pSt_TestInfo->sThouDataNum = 0;
	pSt_TestInfo->Test_st = TEST_ST_TESTING;
	pSt_TestInfo->ThouData_Cur = 100.0;
	pSt_TestInfo->ThouData_dis = 100.0;
    pSt_TestInfo->sThouData_Cnt = 0;
	pSt_TestInfo->Save_st = false;
    // 变化
    pSt_TestInfo->Ad_Max_Move_Time = 0;
	//pSt_TestInfo->Ad_LightData_Last = 0;
    pSt_TestInfo->Ad_Max_Move_Data = 0;
	// 计算 透过率
	ThruRate_Calculate(pSt_TestInfo);
}


/********************************************************************************
* 名称：MasterThruRateCalculate()
* 功能：透过率计算
* 入口参数：无
* 出口参数：无
*********************************************************************************/
void testing_thread::ThruRate_Calculate(ST_TEST_INFO *pSt_TestInfo)
{
	double tmp = 0.0;
	double aa = 0, bb = 0;

	aa = pSt_TestInfo->Ad_LightData_Begin - pSt_TestInfo->Ad_DarkData_Begin;
	bb = pSt_TestInfo->Ad_LightData_Cur - pSt_TestInfo->Ad_DarkData_Cur;

	//qDebug()<<QString("ThruRate_Calculate bb = %1 aa = %2").arg(bb).arg(aa);
	if(aa != 0)
	{
		tmp = (bb/aa)*100;//各瓶位测量透过率
	}
	//qDebug()<<QString("ThruRate_Calculate tmp = %1 ").arg(tmp);
#if 0
	//pSt_TestInfo->Last_ThouData = 100 - pSt_TestInfo->Last_ThouData;
	bb = 100 - pSt_TestInfo->Last_ThouData;
	if(((bb - tmp) < THOU_DATA_STANARD)&&( aa >= 0)&&((tmp - bb) <= THOU_DATA_STANARD))//比较滤波
	{
		pSt_TestInfo->ThouData_Cur = tmp;
		qDebug()<<"ThouData_Cur  === oK *****";
	}
#else
	pSt_TestInfo->ThouData_Cur = tmp;
#endif
	if((pSt_TestInfo->ThouData_Cur >= 99.99)&&(pSt_TestInfo->ThouData_Cur <= 100.0))
	{
		pSt_TestInfo->ThouData_Cur = 100.0;
	}
	else if(pSt_TestInfo->ThouData_Cur > 105.0)
	{
		pSt_TestInfo->ThouData_Cur = 105.0;
	}
	else if(pSt_TestInfo->ThouData_Cur < 0.1)
	{
		pSt_TestInfo->ThouData_Cur = 0.0;
	}

//	qDebug()<<QString("***[ThruRate_Calculate] ThouData_Cur = %1 ").arg(pSt_TestInfo->ThouData_Cur);
	//pSt_TestInfo->ThouData_Cur = 40;
}

/****************************************************************************
* 名称：ThouData_Smooth()
* 功能：透过率平滑处理
* 入口参数：ST_TEST_INFO *pSt_TestInfo  -个瓶的数据
* 出口参数：
****************************************************************************/
void testing_thread::ThouData_Smooth(ST_TEST_INFO *pSt_TestInfo)
{
	unsigned char j = 0;
	float  ccc = 0.0;

	if( TEST_ST_TESTING == pSt_TestInfo->Test_st)
	{
#if 1
		//qDebug()<<QString("[ThouData_Smooth] pSt_TestInfo->ThouData_Cur = %1").arg(pSt_TestInfo->ThouData_Cur);
		//  透过率平滑处理
		pSt_TestInfo->sThouDataTemp[MAX_THOU_DATA] = pSt_TestInfo->ThouData_Cur;//更新数据
		for(j = 0; j < 10; j++)//移位求平均
		{
			pSt_TestInfo->sThouDataTemp[j] = pSt_TestInfo->sThouDataTemp[j+1];
			ccc = ccc+pSt_TestInfo->sThouDataTemp[j];
			//qDebug()<<QString(" pSt_TestInfo->sThouDataTemp[%1] = %2 \r\n").arg(j).arg(pSt_TestInfo->sThouDataTemp[j]);
			//qDebug()<<QString(" j = %1 cc = %2 \r\n").arg(j).arg(ccc);
		}
#endif
		//pSt_TestInfo->ThouData_Cur = ccc/MAX_THOU_DATA;
		pSt_TestInfo->ThouData_dis = ccc/MAX_THOU_DATA;
		if(pSt_TestInfo->ThouData_dis > 100.0)
		{
			pSt_TestInfo->ThouData_dis = 100.0;
		}
		else if(pSt_TestInfo->ThouData_dis < 0.0)
		{
			pSt_TestInfo->ThouData_dis = 0.0;
		}
		//qDebug()<<QString("ThouData_Smooth == pSt_TestInfo->ThouData_dis = %1").arg(pSt_TestInfo->ThouData_dis);
		//pSt_TestInfo->ThouData_Cur = 40;   // for test
		pSt_TestInfo->ThouData_Cur = (100 - pSt_TestInfo->ThouData_dis)*STANDARD_PARAM;
		if(pSt_TestInfo->ThouData_Cur < 0.0)
		{
			pSt_TestInfo->ThouData_Cur = 0.0;
		}
		pSt_TestInfo->sThouData_Cnt ++;//存储的数据个数加1
		//pSt_TestInfo->Ad_LightData_Last = pSt_TestInfo->Ad_LightData_Cur - pSt_TestInfo->Ad_LightData_Last;
		//pSt_TestInfo->Last_ThouData = pSt_TestInfo->ThouData_Cur;
        if((pSt_TestInfo->ThouData_Cur >= START_STANDARD) && (pSt_TestInfo->ThouData_Cur <= END_STANDARD))//限定查询判断区间 此区间为经验值
		{
			int movedata = pSt_TestInfo->Ad_LightData_Last - pSt_TestInfo->Ad_LightData_Cur;
			if(movedata > 0)  //
			{
				if((movedata < MAX_MOVE_DATA) && (movedata > pSt_TestInfo->Ad_Max_Move_Data))
				{
					pSt_TestInfo->Ad_Max_Move_Data = pSt_TestInfo->Ad_LightData_Last - pSt_TestInfo->Ad_LightData_Cur;
					pSt_TestInfo->Ad_Max_Move_Time = pSt_TestInfo->sThouData_Cnt;  // 最大变化时刻
					qDebug()<<QString("***[ThouData_Smooth] ThouData_Cur = %1 &&&&&&&&&").arg(pSt_TestInfo->Ad_Max_Move_Data);
				}
				pSt_TestInfo->Ad_LightData_Last = pSt_TestInfo->Ad_LightData_Cur ;  // 保存当前的亮值
			}
		}


		//qDebug()<<QString("***ThruRate_Calculate ThouData_Cur = %1 \r\n").arg(pSt_TestInfo->ThouData_Cur);
		//qDebug()<<QString("<ThruRate_Calculate> pSt_TestInfo->Ad_LightData_Begin = %1 pSt_TestInfo->Ad_LightData_Cur = %2\r\n").arg(pSt_TestInfo->Ad_LightData_Begin).arg(pSt_TestInfo->Ad_LightData_Cur);
	}
}


void testing_thread::Get_Param(int mode, ST_PARA_SETTING *st)
{
	int TargetNo = 0, ClassNo = 0, ClassNo_1 = 0;

	QFile file(FILE_QX_PARA_SETTING);  //
	file.open(QIODevice::ReadOnly);  // 打开文件 只读

	file.seek(PRAR_TARGET_START);
	file.read((char*)&TargetNo, PRAR_DATA_LEN);
	file.seek(PRAR_CLASS_START);
	file.read((char*)&ClassNo, PRAR_DATA_LEN);
	file.seek(PRAR_CLASS1_START);
	file.read((char*)&ClassNo_1, PRAR_DATA_LEN);

//	printf("TargetNo = %d ClassNo= %d, ClassNo_1= %d\r\n", TargetNo, ClassNo, ClassNo_1);

//	printf("mode = %d \r\n", mode);
	if(TOL_COLI_ID == mode)  // 总大肠
	{
		file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		//memset(st, 0,PRAR_ST_DATA_LEN);
		file.read((char*)st, PRAR_ST_DATA_LEN);
//		printf("PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}
	else if(FEC_COLI_ID == mode)  // 耐热大肠
	{
		file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
//                file.seek(PRAR_FEC_START+PRAR_ST_DATA_LEN*ClassNo);
		//memset(st, 0,PRAR_ST_DATA_LEN);
		file.read((char*)st, PRAR_ST_DATA_LEN);
	}
	else if(TPC_COLI_ID == mode)  // 菌落总数
	{
		file.seek(PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		//file.seek(PRAR_TPC_START+PRAR_ST_DATA_LEN*ClassNo);
//		memset(&st, 0,PRAR_ST_DATA_LEN);
		file.read((char*)st, PRAR_ST_DATA_LEN);
	}
	else if(E_COLI_ID == mode)  // 埃希氏
	{
		file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	   // file.seek(PRAR_E_START+PRAR_ST_DATA_LEN*ClassNo);
//		memset(&st, 0,PRAR_ST_DATA_LEN);
		file.read((char*)st, PRAR_ST_DATA_LEN);
	}
	file.close();

//	printf("st.para_A = %f st.para_B= %f, st.para_C= %f\r\n", st->para_A, st->para_B, st->para_C);
//	printf("st.para_D = %f st.para_Q = %f, st.para_R= %f\r\n", st->para_D, st->para_Q, st->para_R);

#if 0
	int class_no;

	QSettings settings(FILE_CON_PARA_SETTING, QSettings::IniFormat);
	settings.beginGroup(STR_PARASETTING);
	if(TOL_COLI_ID == mode)
	{
		class_no = settings.value(STR_TOl_CLASS).toInt();
		st->Unit = settings.value(STR_TOl_UNIT).toInt();
		if(COIL_CLASS_NO1 == class_no)
		{
			st->para_A = settings.value(STR_TOLCOIL_CLASS1_A).toDouble();
			st->para_B = settings.value(STR_TOLCOIL_CLASS1_B).toDouble();
			st->para_C = settings.value(STR_TOLCOIL_CLASS1_C).toDouble();
			st->para_D = settings.value(STR_TOLCOIL_CLASS1_D).toDouble();
			st->para_R = settings.value(STR_TOLCOIL_CLASS1_R).toDouble();
			st->para_Q = settings.value(STR_TOLCOIL_CLASS1_Q).toDouble();
		}
		else if(COIL_CLASS_NO2 == class_no)
		{
			st->para_A = settings.value(STR_TOLCOIL_CLASS2_A).toDouble();
			st->para_B = settings.value(STR_TOLCOIL_CLASS2_B).toDouble();
			st->para_C = settings.value(STR_TOLCOIL_CLASS2_C).toDouble();
			st->para_D = settings.value(STR_TOLCOIL_CLASS2_D).toDouble();
			st->para_R = settings.value(STR_TOLCOIL_CLASS2_R).toDouble();
			st->para_Q = settings.value(STR_TOLCOIL_CLASS2_Q).toDouble();
		}
		else if(COIL_CLASS_NO3 == class_no)
		{
			st->para_A = settings.value(STR_TOLCOIL_CLASS3_A).toDouble();
			st->para_B = settings.value(STR_TOLCOIL_CLASS3_B).toDouble();
			st->para_C = settings.value(STR_TOLCOIL_CLASS3_C).toDouble();
			st->para_D = settings.value(STR_TOLCOIL_CLASS3_D).toDouble();
			st->para_R = settings.value(STR_TOLCOIL_CLASS3_R).toDouble();
			st->para_Q = settings.value(STR_TOLCOIL_CLASS3_Q).toDouble();
		}
		else if(COIL_CLASS_NO4 == class_no)
		{
			st->para_A = settings.value(STR_TOLCOIL_CLASS4_A).toDouble();
			st->para_B = settings.value(STR_TOLCOIL_CLASS4_B).toDouble();
			st->para_C = settings.value(STR_TOLCOIL_CLASS4_C).toDouble();
			st->para_D = settings.value(STR_TOLCOIL_CLASS4_D).toDouble();
			st->para_R = settings.value(STR_TOLCOIL_CLASS4_R).toDouble();
			st->para_Q = settings.value(STR_TOLCOIL_CLASS4_Q).toDouble();
		}
	}
	else if(FEC_COLI_ID == mode)
	{
		class_no = settings.value(STR_FEC_CLASS).toInt();
		st->Unit = settings.value(STR_FEC_UNIT).toInt();
		if(COIL_CLASS_NO1 == class_no)
		{
			st->para_A = settings.value(STR_FECCOIL_CLASS1_A).toDouble();
			st->para_B = settings.value(STR_FECCOIL_CLASS1_B).toDouble();
			st->para_C = settings.value(STR_FECCOIL_CLASS1_C).toDouble();
			st->para_D = settings.value(STR_FECCOIL_CLASS1_D).toDouble();
			st->para_R = settings.value(STR_FECCOIL_CLASS1_R).toDouble();
			st->para_Q = settings.value(STR_FECCOIL_CLASS1_Q).toDouble();
		}
		else if(COIL_CLASS_NO2 == class_no)
		{
			st->para_A = settings.value(STR_FECCOIL_CLASS2_A).toDouble();
			st->para_B = settings.value(STR_FECCOIL_CLASS2_B).toDouble();
			st->para_C = settings.value(STR_FECCOIL_CLASS2_C).toDouble();
			st->para_D = settings.value(STR_FECCOIL_CLASS2_D).toDouble();
			st->para_R = settings.value(STR_FECCOIL_CLASS2_R).toDouble();
			st->para_Q = settings.value(STR_FECCOIL_CLASS2_Q).toDouble();
		}
		else if(COIL_CLASS_NO3 == class_no)
		{
			st->para_A = settings.value(STR_FECCOIL_CLASS3_A).toDouble();
			st->para_B = settings.value(STR_FECCOIL_CLASS3_B).toDouble();
			st->para_C = settings.value(STR_FECCOIL_CLASS3_C).toDouble();
			st->para_D = settings.value(STR_FECCOIL_CLASS3_D).toDouble();
			st->para_R = settings.value(STR_FECCOIL_CLASS3_R).toDouble();
			st->para_Q = settings.value(STR_FECCOIL_CLASS3_Q).toDouble();
		}
		else if(COIL_CLASS_NO4 == class_no)
		{
			st->para_A = settings.value(STR_FECCOIL_CLASS4_A).toDouble();
			st->para_B = settings.value(STR_FECCOIL_CLASS4_B).toDouble();
			st->para_C = settings.value(STR_FECCOIL_CLASS4_C).toDouble();
			st->para_D = settings.value(STR_FECCOIL_CLASS4_D).toDouble();
			st->para_R = settings.value(STR_FECCOIL_CLASS4_R).toDouble();
			st->para_Q = settings.value(STR_FECCOIL_CLASS4_Q).toDouble();
		}

	}
	else if(TPC_COLI_ID == mode)
	{
		class_no = settings.value(STR_TPC_CLASS).toInt();
		st->Unit = settings.value(STR_TPC_UNIT).toInt();
		if(COIL_CLASS_NO1 == class_no)
		{
			st->para_A = settings.value(STR_TPCCOIL_CLASS1_A).toDouble();
			st->para_B = settings.value(STR_TPCCOIL_CLASS1_B).toDouble();
			st->para_C = settings.value(STR_TPCCOIL_CLASS1_C).toDouble();
			st->para_D = settings.value(STR_TPCCOIL_CLASS1_D).toDouble();
			st->para_R = settings.value(STR_TPCCOIL_CLASS1_R).toDouble();
			st->para_Q = settings.value(STR_TPCCOIL_CLASS1_Q).toDouble();
		}
		else if(COIL_CLASS_NO2 == class_no)
		{
			st->para_A = settings.value(STR_TPCCOIL_CLASS2_A).toDouble();
			st->para_B = settings.value(STR_TPCCOIL_CLASS2_B).toDouble();
			st->para_C = settings.value(STR_TPCCOIL_CLASS2_C).toDouble();
			st->para_D = settings.value(STR_TPCCOIL_CLASS2_D).toDouble();
			st->para_R = settings.value(STR_TPCCOIL_CLASS2_R).toDouble();
			st->para_Q = settings.value(STR_TPCCOIL_CLASS2_Q).toDouble();
		}
		else if(COIL_CLASS_NO3 == class_no)
		{
			st->para_A = settings.value(STR_TPCCOIL_CLASS3_A).toDouble();
			st->para_B = settings.value(STR_TPCCOIL_CLASS3_B).toDouble();
			st->para_C = settings.value(STR_TPCCOIL_CLASS3_C).toDouble();
			st->para_D = settings.value(STR_TPCCOIL_CLASS3_D).toDouble();
			st->para_R = settings.value(STR_TPCCOIL_CLASS3_R).toDouble();
			st->para_Q = settings.value(STR_TPCCOIL_CLASS3_Q).toDouble();
		}
		else if(COIL_CLASS_NO4 == class_no)
		{
			st->para_A = settings.value(STR_TPCCOIL_CLASS4_A).toDouble();
			st->para_B = settings.value(STR_TPCCOIL_CLASS4_B).toDouble();
			st->para_C = settings.value(STR_TPCCOIL_CLASS4_C).toDouble();
			st->para_D = settings.value(STR_TPCCOIL_CLASS4_D).toDouble();
			st->para_R = settings.value(STR_TPCCOIL_CLASS4_R).toDouble();
			st->para_Q = settings.value(STR_TPCCOIL_CLASS4_Q).toDouble();
		}

	}
	else if(E_COLI_ID == mode)
	{
		class_no = settings.value(STR_E_CLASS).toInt();
		st->Unit = settings.value(STR_E_UNIT).toInt();
		if(COIL_CLASS_NO1 == class_no)
		{
			st->para_A = settings.value(STR_ECOIL_CLASS1_A).toDouble();
			st->para_B = settings.value(STR_ECOIL_CLASS1_B).toDouble();
			st->para_C = settings.value(STR_ECOIL_CLASS1_C).toDouble();
			st->para_D = settings.value(STR_ECOIL_CLASS1_D).toDouble();
			st->para_R = settings.value(STR_ECOIL_CLASS1_R).toDouble();
			st->para_Q = settings.value(STR_ECOIL_CLASS1_Q).toDouble();
		}
		else if(COIL_CLASS_NO2 == class_no)
		{
			st->para_A = settings.value(STR_ECOIL_CLASS2_A).toDouble();
			st->para_B = settings.value(STR_ECOIL_CLASS2_B).toDouble();
			st->para_C = settings.value(STR_ECOIL_CLASS2_C).toDouble();
			st->para_D = settings.value(STR_ECOIL_CLASS2_D).toDouble();
			st->para_R = settings.value(STR_ECOIL_CLASS2_R).toDouble();
			st->para_Q = settings.value(STR_ECOIL_CLASS2_Q).toDouble();
		}
		else if(COIL_CLASS_NO3 == class_no)
		{
			st->para_A = settings.value(STR_ECOIL_CLASS3_A).toDouble();
			st->para_B = settings.value(STR_ECOIL_CLASS3_B).toDouble();
			st->para_C = settings.value(STR_ECOIL_CLASS3_C).toDouble();
			st->para_D = settings.value(STR_ECOIL_CLASS3_D).toDouble();
			st->para_R = settings.value(STR_ECOIL_CLASS3_R).toDouble();
			st->para_Q = settings.value(STR_ECOIL_CLASS3_Q).toDouble();
		}
		else if(COIL_CLASS_NO4 == class_no)
		{
			st->para_A = settings.value(STR_ECOIL_CLASS4_A).toDouble();
			st->para_B = settings.value(STR_ECOIL_CLASS4_B).toDouble();
			st->para_C = settings.value(STR_ECOIL_CLASS4_C).toDouble();
			st->para_D = settings.value(STR_ECOIL_CLASS4_D).toDouble();
			st->para_R = settings.value(STR_ECOIL_CLASS4_R).toDouble();
			st->para_Q = settings.value(STR_ECOIL_CLASS4_Q).toDouble();
		}
	}

	settings.endGroup();
#endif
}

/*********************************************************************************
* 名称：MasterOverTimeCheck()
* 功能：超时检测
* 入口参数：无
* 出口参数：0：无超时 1：有瓶超时
*********************************************************************************/
bool testing_thread::Check_OverTime(void)
{
	int i=0;
//	int flag=0;
//	flag = 0;

	ST_PARA_SETTING st_temp;
	//int class_no;

	qDebug()<<QString("Check_OverTime ===== @@@ \r\n");
#if 1
	//循环扫描瓶位状态
	for(i=0;i < MAX_TEST_NUM; i++)
	{
		//qDebug()<<QString("i =====%1 @@@ \r\n").arg(i);
		//如果瓶位正在进行检测
		if(TEST_ST_TESTING == gst_Test_Info[i].Test_st)
		{
			if(i < TPCCOLI_START_NUM) //总大肠  耐热
			{
//				qDebug()<<QString("i =<2>====%1 @@@ \r\n").arg(i);
				//RefReadTab();  // 获取参数 后续完成
				if(i < FECCOLI_START_NUM)//0-7总大肠
				{
					memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
					Get_Param(TOL_COLI_ID, &st_temp);
				}
				else if(i < TPCCOLI_START_NUM)//8-15 耐热
				{
					memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
					Get_Param(FEC_COLI_ID, &st_temp);
				}

//				qDebug()<<QString("para_A === [%1] ##############\r\n").arg(st_temp.para_A);
//				qDebug()<<QString("para_B === [%1] ##############\r\n").arg(st_temp.para_B);
//				qDebug()<<QString("para_C === [%1] ##############\r\n").arg(st_temp.para_C);
//				qDebug()<<QString("para_D === [%1] ##############\r\n").arg(st_temp.para_D);


				//如果该瓶位消光度达到消光度上限值
				qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur <*****>= %2 END_STANDARD= %3").arg(i).arg(gst_Test_Info[i].ThouData_Cur).arg(END_STANDARD);
#if 0
				// 菌落总数
				if((i >= TPCCOLI_START_NUM) && (i < MAX_TEST_NUM))
				{

					//qDebug()<<QString("***********num === %1 \r\n").arg(i);
					qDebug()<<QString("gst_Test_Info[%1].sThouData_Cnt = %2").arg(i).arg(gst_Test_Info[i].sThouData_Cnt);
					if(st_temp.para_A != 0.0)  // 根据参数计算出 时间
					{
						gst_Test_Info[i].threshold_time = (long)((st_temp.para_B - 1.00)/st_temp.para_A);
						qDebug()<<QString("pSt_TestInfo->threshold_time == %1 \r\n").arg(gst_Test_Info[i].threshold_time);
					}

					int time_len;
					if(gst_Test_Info[i].threshold_time > g_Test_Over_Time)
					{
						time_len = g_Test_Over_Time;
					}
					else
					{
						time_len = gst_Test_Info[i].threshold_time;
					}

					if(gst_Test_Info[i].sThouData_Cnt > time_len)
					{
						if((gst_Test_Info[i].Ad_LightData_Cur - gst_Test_Info[i].Ad_LightData_Begin) < AD_TEST_LIMIT)  // AD 变化范围太小 说明无变化
						{
							Save_Data(i, 0.0, &gst_Test_Info[i], &st_temp);
						}
						else{
							Time_Calculate(&gst_Test_Info[i] ,&st_temp);
							Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
						}
					}
				}
				else  // 总大肠  耐热
#endif
				{
					qDebug()<<QString("<22>gst_Test_Info[%1].ThouData_Cur <*****>= %2 END_STANDARD= %3 g_Test_Over_Time = %4").arg(i).arg(gst_Test_Info[i].ThouData_Cur).arg(END_STANDARD).arg(g_Test_Over_Time);
					if(gst_Test_Info[i].ThouData_Cur >= END_STANDARD)  // 此处应是 透过率 小于30%  及吸光度 70%时判定
					{
						Time_Calculate(&gst_Test_Info[i] ,&st_temp);
						Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
					}
					else
					{
						//如果该瓶位检测超时，但认为未到消光度上限值
						qDebug()<<QString("gst_Test_Info[%1].sThouData_Cnt==111 = %2 g_Test_Over_Time = %3").arg(i).arg(gst_Test_Info[i].sThouData_Cnt).arg(g_Test_Over_Time);
						if(gst_Test_Info[i].sThouData_Cnt > g_Test_Over_Time)
						{
							qDebug()<<QString("gst_Test_Info[%1].sThouData_Cnt =222== %2 g_Test_Over_Time = %3").arg(i).arg(gst_Test_Info[i].sThouData_Cnt).arg(g_Test_Over_Time);
							//如果该瓶位消光度达到了下限值，认为超时变色
							if(gst_Test_Info[i].ThouData_Cur >= END_MAX_STANDARD)  // 此处应是 透过率 小于20%  及吸光度 80%时判定
							{
								Time_Calculate(&gst_Test_Info[i] ,&st_temp);
								Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
								//break;
							}
                            else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_60)  // 吸光度 60%
							{
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                }
                                else{
									int num = 0;

									while(num < 80)
									{
										num = qrand()%100;
									}

									Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                }
							}
							else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_50)// 吸光度 50%
							{
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                }
                                else{
									int num = 0;

									while((num < 60) || (num > 80))
									{
										num = qrand()%80;
									}

									Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                }
							}
							else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_40) // 吸光度 40%
							{
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                }
                                else{
									int num = 0;

									while((num < 40) || (num > 60))
									{
										num = qrand()%60;
									}
                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                }
							}
							else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_30)  // 吸光度 30%
							{
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                }
                                else{
									int num = 0;

									while((num < 20) || (num > 40))
									{
										num = qrand()%40;
									}
                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                }
							}
							else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_20)  // // 吸光度 20%
							{
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                }
                                else{

									int num = 0;

									while((num < 10) || (num > 20))
									{
										num = qrand()%20;
									}
                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                }
							}
							else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_10) // // 吸光度 10%
							{
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                }
                                else{
									int num = 0;

									while((num < 1) || (num > 10))
									{
										num = qrand()%10;
									}

                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                }
							}
                        //检测超时，且未变色。
							else  // 注意 此处需要完善
							{
                                gst_Test_Info[i].Ad_Max_Move_Time = g_Test_Over_Time;
                                Save_Data(i, 1, &gst_Test_Info[i], &st_temp);
    //							MasterSampInf[i].TiterOnlineResult.titer = 0;
	//							flag = 1;
								//break;
							}
						}
					}
				}
			}
			else   //24-31 埃希氏
			{
				if(i < ECOLI_START_NUM)  // 16-23 菌落总数
				{
					memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
					Get_Param(TPC_COLI_ID, &st_temp);
				}
				else{
					memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
					Get_Param(E_COLI_ID, &st_temp);
				}
				// 菌落总数和埃希氏
#if 0
                qDebug()<<QString("**<E>*********num === %1 ").arg(i);
				if(st_temp.para_A != 0.0)  // 根据参数计算出 时间
				{
					gst_Test_Info[i].threshold_time = (long)((st_temp.para_B - 1.00)/st_temp.para_A);
					qDebug()<<QString("pSt_TestInfo->threshold_time == %1 \r\n").arg(gst_Test_Info[i].threshold_time);
				}

				int time_len;
				if(gst_Test_Info[i].threshold_time >= g_Test_Over_Time)
				{
					time_len = g_Test_Over_Time;
				}
				else
				{
					time_len = gst_Test_Info[i].threshold_time;
				}
#else
				unsigned int  time_len = g_Test_Over_Time ;
#endif
//				qDebug()<<QString("gst_Test_Info[%1].sThouData_Cnt <*****>= %2 time_len= %3").arg(i).arg(gst_Test_Info[i].sThouData_Cnt).arg(time_len);
 //               printf("\n[2] g_mpn_test_st ==== %d \r\n", g_mpn_test_st);
                if((gst_Test_Info[i].sThouData_Cnt > time_len) && (g_mpn_test_st == MPN_TEST_IDLE))
				{

#if 1
                  qDebug()<<QString("[###]gst_Test_Info[%1].Ad_LightData_Cur <*****>= %2 gst_Test_Info[%3].Ad_LightData_Begin= %4").arg(i).arg(gst_Test_Info[i].Ad_LightData_Cur).arg(i).arg(gst_Test_Info[i].Ad_LightData_Begin);
					if((gst_Test_Info[i].Ad_LightData_Cur - gst_Test_Info[i].Ad_LightData_Begin) < AD_TEST_LIMIT)  // AD 变化范围太小 说明无变化
					{
                        //gst_Test_Info[i].sThouData_Cnt = g_Test_Over_Time;
                        gst_Test_Info[i].Ad_Max_Move_Time = time_len;
                        Save_Data(i, 1, &gst_Test_Info[i], &st_temp);  // 最小值是1
					}
					else{
						Time_Calculate(&gst_Test_Info[i] ,&st_temp);
						Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
					}
#else
					Time_Calculate(&gst_Test_Info[i] ,&st_temp);
					Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
#endif
				}
                else if(g_mpn_test_st == MPN_TEST_FEC || g_mpn_test_st == MPN_TEST_TOL){  // 国标测试
                    int move_data = gst_Test_Info[i].Ad_LightData_Cur - gst_Test_Info[i].Ad_LightData_Begin;
                    printf("\r\n move_data == %d AD_TEST_LIMIT === %d\r\n",move_data ,AD_TEST_LIMIT);
                    if((gst_Test_Info[i].sThouData_Cnt > TB_FAST_TIME_LIMIT) || (move_data > AD_TEST_LIMIT)) // 24小时
                    {
                        gst_Test_Info[i].Test_st = TEST_ST_IDEL;  // 清楚测试标志

                        printf("\r\n <1111 >gst_Test_Info[%d].sThouData_Cnt === %d \r\n",i, gst_Test_Info[i].sThouData_Cnt);
                        printf("\n i ==== %d TPCCOLI_START_NUM == %d  \r\n ", i, TPCCOLI_START_NUM);
                        g_mpn_test_val[i - TPCCOLI_START_NUM] = gst_Test_Info[i].sThouData_Cnt;

                        int rtn = true;
                        int a= 0, b= 0, c = 0;
                        a = 0;
                        b = 0;
                        c = 0;
                        rtn = true;
                        int j = 0;

                        for(j = 0; j < 15 /*MPN_MAX_NUM*/; j++)
                        {
                            printf("<11111111>g_mpn_test_val[%d] ==== %d MPN_MAX_NUM === %d \r\n", j, g_mpn_test_val[j], MPN_MAX_NUM);
                            if(g_mpn_test_val[j] == 0){
                                rtn = false;
                                printf("\r\n<22222222> MPN_MAX_NUM == %d j === %d \r\n", MPN_MAX_NUM, j);
                               // break;
                            }
                            else {
                                 printf("<333333>g_mpn_test_val[%d] ==== %d \r\n", j, g_mpn_test_val[j]);
                                if((j < 5) /*&&(g_mpn_test_val[j] < TB_FAST_TIME_LIMIT)*/){
                                    a++;
                                }
                                else if((j < 10) /*&& (g_mpn_test_val[j] < TB_FAST_TIME_LIMIT)*/){
                                    b++;
                                }
                                else/* if(g_mpn_test_val[i] < TB_FAST_TIME_LIMIT)*/{
                                    c ++;
                                }
                            }
                        }
                        printf("[ ]rtn ==== %d a == %d b == %d c== %d \r\n", rtn, a, b, c );
                        if(true == rtn){
                            long temp_val = GetMpnVal(a,b, c);
                            if(g_mpn_test_st == MPN_TEST_FEC){  // 测试的耐热
                                 Save_Data(FECCOLI_START_NUM, temp_val, &gst_Test_Info[TPCCOLI_START_NUM], &st_temp);  // 最小值是1
                            }
                            else if(g_mpn_test_st == MPN_TEST_TOL){   // 测试的TOL

                                 Save_Data(TOLCOLI_START_NUM, temp_val, &gst_Test_Info[TPCCOLI_START_NUM], &st_temp);  // 最小值是1
                            }
                            g_mpn_test_st = MPN_TEST_IDLE;
                             printf("\r\n[1]g_mpn_test_st ==== %d\r\n", g_mpn_test_st);
                        }

                    }
                }
			}
		}
	}

#endif

	return true;
}

long testing_thread::GetMpnVal(int a, int b, int c)
{
    if(0 == a)
    {
        return 10*g_mpn_test_mui*mpn_0[b][c];
    }
    else if(1 == a){
        return 10*g_mpn_test_mui*mpn_1[b][c];
    }
    else if(2 == a){
        return 10*g_mpn_test_mui*mpn_2[b][c];
    }
    else if(3 == a){
        return 10*g_mpn_test_mui*mpn_3[b][c];
    }
    else if(4 == a){
        return 10*g_mpn_test_mui*mpn_4[b][c];
    }
    else if(5 == a){
        return 10*g_mpn_test_mui*mpn_5[b][c];
    }
}

/*********************************************************************************
* 名称：时间校正
* 功能：
* 入口参数：
* 出口参数：无
*********************************************************************************/
void testing_thread::Time_Calculate(ST_TEST_INFO *pSt_TestInfo, ST_PARA_SETTING *pst_ParaSetting)
{
	return ;
	//log10(10)=1.00
	if(pst_ParaSetting->para_A != 0.0)  // 根据参数计算出 时间
	{
		pSt_TestInfo->threshold_time = (long)((pst_ParaSetting->para_B - 1.00)/pst_ParaSetting->para_A);
		qDebug()<<QString("pSt_TestInfo->threshold_time == %1 \r\n").arg(pSt_TestInfo->threshold_time);
	}

	if(g_Test_Over_Time > pSt_TestInfo->threshold_time) // 最大时间 大于 计算时间
	{
		pst_ParaSetting->para_A = 1.00/(g_Test_Over_Time - pSt_TestInfo->threshold_time);
		pst_ParaSetting->para_B = pst_ParaSetting->para_A * g_Test_Over_Time;
		//RefUse.ref_table1.curve_ref_c =	RefUse.ref_table.curve_ref_c;
		//RefUse.ref_table1.curve_ref_d =	RefUse.ref_table.curve_ref_d;
	}
}

/*********************************************************************************
* 名称：MasterTiterCal()
* 功能：高精度浓度计算
* 入口参数：瓶号
* 出口参数：无
*********************************************************************************/
void testing_thread::Titer_Cal(int num, ST_TEST_INFO *pSt_TestInfo, ST_PARA_SETTING *pst_Parasetting)
{
	double    consis_data=0;
	double    xxx=0;
	float    yyy=0;
	double    zzz=0;
	unsigned short  con_data1=0;
	unsigned short  con_data2=0;
	unsigned short  con_data3=0;
	float unit_temp=0;
	//REF_TABLE_BASE *cal_ref_tab;
#if 0
	//判断时间是否超过检出限时间限值
	if(pSt_TestInfo->sThouData_Cnt >= pSt_TestInfo->threshold_time)
	{
		cal_ref_tab = &(refuse->ref_table1);
	}
	else
	{
		cal_ref_tab = &(refuse->ref_table);
	}
#endif
	//对检测量进行手动校正
	if(0 == pSt_TestInfo->Ad_Max_Move_Time)
	{
		pSt_TestInfo->Ad_Max_Move_Time = 1;
	}
	if(pSt_TestInfo->Ad_Max_Move_Time >= g_Test_Over_Time)
	{
		pSt_TestInfo->Ad_Max_Move_Time = g_Test_Over_Time - 2;
	}

	qDebug()<<QString("para_A === [%1] **************\r\n").arg(pst_Parasetting->para_A);
	qDebug()<<QString("para_B === [%1] **************\r\n").arg(pst_Parasetting->para_B);
	qDebug()<<QString("para_C === [%1] **************\r\n").arg(pst_Parasetting->para_C);
	qDebug()<<QString("para_D === [%1] **************\r\n").arg(pst_Parasetting->para_D);
	qDebug()<<QString("pSt_TestInfo->Ad_Max_Move_Time === [%1] **************\r\n").arg(pSt_TestInfo->Ad_Max_Move_Time);
	//
	yyy = pst_Parasetting->para_A * pSt_TestInfo->Ad_Max_Move_Time - pst_Parasetting->para_B;//公式可以改变
	if(yyy < (-11.0))//超出量程上限
	{
		con_data1 = 10;
		con_data2 = 11|0x10;
	}
	else
	{
		zzz = exp(-1*log(10)*yyy);//计数浓度

		xxx = zzz*pst_Parasetting->para_C + pst_Parasetting->para_D;//c,d值默认为1,0，可以微调
		qDebug()<<QString("xxx === [%1] **************\r\n").arg(xxx);
		if((pst_Parasetting->Unit == UNIT_MPN_100ML)||(pst_Parasetting->Unit == UNIT_CFU_ML))
		{
			unit_temp = 1.0;
		}
		else if(pst_Parasetting->Unit == UNIT_MPN_L)
		{
			unit_temp = 10.0;
		}
		else
		{
			unit_temp = 1.0;
		}
		consis_data = xxx*unit_temp;
        if(consis_data < 1.0)
        {
            consis_data = 1.0;
        }
		Save_Data(num,consis_data, pSt_TestInfo, pst_Parasetting);

		//sampinf->TiterOnlineResult.titer = consis_data;//浓度(for print)
		qDebug()<<QString("nongdu = [%1] *************88").arg(consis_data);
		//浓度结果转换成科学计数方法
		con_data2 = 0;//存幂数
		if(consis_data <= 1.0)
		{
			con_data1 = 10;
		}
		else if(consis_data < 10)
		{
			con_data3 = (unsigned short)(consis_data*10);
			if((con_data3%10) > 0)
			{
				con_data3 = con_data3/10;
				con_data3 = con_data3*10;
				con_data3 += 10;
			}
			con_data1 = con_data3;
			if(con_data1 == 100)
			{
				con_data1 = 10;
				con_data2 = 1;
			}
		}
		else//>=10
		{
			while(consis_data >= 100.0)
			{
				con_data2 ++;
				consis_data = consis_data/10.0;
			}//while
			con_data2 = con_data2+1;//得到幂数
			con_data1 = (unsigned short)consis_data;//得到系数
		}
	}
	qDebug()<<QString("con_data1 === [%1] *********con_data2 ===[%2]*****\r\n").arg(con_data1).arg(con_data2);
	//Save_Data(num,consis_data, pSt_TestInfo, pst_Parasetting);

	//高精度测量结果
	//gp_sql->Add_One_Data();
}
#if HAVE_PRINTF
// 打印汉字
void testing_thread::setHz()
{
    char data[6];

    data[0]=0x1b;
    data[1]=0x38;
    data[2]=0x1b;
    data[3]=0x56;
    data[4]=0x01;
    pTranData->com3_send(data, 5);
}

// 打印字串
void testing_thread::setchar()
{
    char data[6];

    data[0]=0x1b;
    data[1]=0x36;
    data[2]=0x1b;
    data[3]=0x56;
    data[4]=0x02;
    pTranData->com3_send(data, 5);
}
#endif

ST_HISTORY_DATA gst_His_Info_temp[MAX_ONE_GROUP_NUM], gst_His_Info_temp_2;

ST_HISTORY_DATA st_his_data;

// 保存结果
void testing_thread::Save_Data(int num , double dat, ST_TEST_INFO *pSt_TestInfo, ST_PARA_SETTING *pst_Parasetting)
{
    int id = 0, i = 0;
    int test_len = 0;
    double temp_nongdu = 0.0;

	QDateTime time= QDateTime::currentDateTime();
	QString strDate= time.toString("yyyy-MM-dd hh:mm:ss");

    //printf("\nid = 0x%x i == 0x%x test_len = 0x%x st_his_data = 0x%x\r\n", &id, &i, &test_len, &st_his_data);
    //printf("\n dat = 0x%x pSt_TestInfo == 0x%x pst_Parasetting = 0x%x \r\n", &dat, pSt_TestInfo, pst_Parasetting);
#if 0
	if(true == pSt_TestInfo->Save_st)
	{
		return ;
	}
#endif

	st_his_data.year = time.toString("yyyy").toInt();
	st_his_data.mon = time.toString("MM").toInt();
	st_his_data.day = time.toString("dd").toInt();
	st_his_data.hour = time.toString("hh").toInt();
	st_his_data.min = time.toString("mm").toInt();
	st_his_data.sec = time.toString("ss").toInt();

	qDebug()<<QString(" year = %1 mon = %2 day = %3\r\n").arg(st_his_data.year).arg(st_his_data.mon).arg(st_his_data.day);
	qDebug()<<QString(" pSt_TestInfo->Ad_Max_Move_Time = %1 \r\n").arg(pSt_TestInfo->Ad_Max_Move_Time);

    if(0 == pSt_TestInfo->Ad_Max_Move_Time)
    {
        pSt_TestInfo->Ad_Max_Move_Time = FAST_TIME_LIMIT;
    }
	qDebug()<<QString(" hour = %1 min = %2 sec = %3\r\n").arg(st_his_data.hour).arg(st_his_data.min).arg(st_his_data.sec);
	st_his_data.NongD = dat;
	st_his_data.bt_id = pSt_TestInfo->BottleId;
	st_his_data.test_len = pSt_TestInfo->Ad_Max_Move_Time;
	st_his_data.unit_n = pst_Parasetting->Unit;

	qDebug()<<QString("########## num = %1 #### \r\n").arg(num);
	st_his_data.BottleId = QString::number( pSt_TestInfo->BottleId);
	st_his_data.NongDu = QString::number(dat);
	st_his_data.SampleID = QString::number(pSt_TestInfo->SampleID);
	st_his_data.Time = strDate;
	st_his_data.Param_time = QString::number(pSt_TestInfo->Ad_Max_Move_Time);;

	if(pst_Parasetting->Unit == UNIT_CFU_ML)
	{
		st_his_data.Unit = STR_UNIT_CFU_ML;
	}
	else if(pst_Parasetting->Unit == UNIT_MPN_100ML)
	{
		st_his_data.Unit = STR_UNIT_MPN_100ML;
	}
	else if(pst_Parasetting->Unit == UNIT_MPN_L)
	{
		st_his_data.Unit = STR_UNIT_MPN_L;
	}
#if HAVE_PRINTF
// 打印浓度
    char  PrintHeadStr[]="=============JMA-SZS-MP III=============\n";
    char  PrintBreakStr1[]="=======================================\n";
    char temp[100];
    QString tm;
    QString str_tmp ;

    setchar();
    pTranData->com3_send(PrintBreakStr1, strlen(PrintBreakStr1));

 //   memset(temp, 0, sizeof(temp));
 //   memcpy(temp, "完成时间:", 9);
 //   pTranData->com3_send(temp, strlen(temp));
    setHz();
    str_tmp = "完成时间：";

    //tm = utf82gbk(str_tmp);
   // tm = UTF82GBK(str_tmp);
    //QTextCodec *codec = QTextCodec::codecForName("GBK");
    //QTextCodec *codec = QTextCodec::codecForName("Big5");
    //QByteArray strText = codec->fromUnicode("完成时间：");

    //memset(temp, 0, sizeof(temp));
    //str2char(strText.data(), temp);
    //"完成时间：";
    temp[0] = 0xcd;
    temp[1] = 0xEa;

    temp[2] = 0xB3;
    temp[3] = 0xc9;

    temp[4] = 0xca;
    temp[5] = 0xb1;

    temp[6] = 0xbc;
    temp[7] = 0xe4;

    temp[8] = 0xa3;
    temp[9] = 0xba;

    pTranData->com3_send( temp, 10);
    memset(temp, 0, sizeof(temp));
    //strDate.toWCharArray(temp);
    sprintf(temp, "%d-%d-%d %d:%d:%d\n", st_his_data.year, st_his_data.mon, st_his_data.day, st_his_data.hour,st_his_data.min,st_his_data.sec);
    setchar();
    pTranData->com3_send(temp, strlen(temp));

    //"瓶位号：";
    temp[0] = 0xc6;
    temp[1] = 0xBF;

    temp[2] = 0xce;
    temp[3] = 0xbb;

    temp[4] = 0xba;
    temp[5] = 0xc5;

    temp[6] = 0xa3;
    temp[7] = 0xba;

    setHz();
    pTranData->com3_send(temp, 8);
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%d\n", pSt_TestInfo->BottleId);
    setchar();
    pTranData->com3_send(temp, strlen(temp));

    //"培养时间：";
    temp[0] = 0xc5;
    temp[1] = 0xE0;

    temp[2] = 0xD1;
    temp[3] = 0xf8;

    temp[4] = 0xca;
    temp[5] = 0xb1;

    temp[6] = 0xbc;
    temp[7] = 0xe4;

    temp[8] = 0xa3;
    temp[9] = 0xba;
    setHz();
    pTranData->com3_send(temp, 10);
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%d\n", pSt_TestInfo->Ad_Max_Move_Time);
    setchar();
    pTranData->com3_send(temp, strlen(temp));

    //"浓度：";
    temp[0] = 0xc5;
    temp[1] = 0xA8;

    temp[2] = 0xb6;
    temp[3] = 0xc8;

    temp[4] = 0xa3;
    temp[5] = 0xba;

    setHz();
    pTranData->com3_send(temp, 6);
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%f\n", dat);
    setchar();
    pTranData->com3_send(temp, strlen(temp));

    //"样本号：";
    temp[0] = 0xd1;
    temp[1] = 0xF9;

    temp[2] = 0xb1;
    temp[3] = 0xbe;

    temp[4] = 0xbA;
    temp[5] = 0xC5;

    temp[6] = 0xa3;
    temp[7] = 0xba;
    setHz();
    pTranData->com3_send(temp, 8);
    memset(temp, 0, sizeof(temp));
    sprintf(temp, "%d\n", pSt_TestInfo->SampleID);
    setchar();
    pTranData->com3_send(temp, strlen(temp));
    setchar();
    pTranData->com3_send(PrintHeadStr, strlen(PrintHeadStr));
#endif
    //mempcpy(&(gst_His_Info[num]),&st_his_data, sizeof(ST_HISTORY_DATA));  // 保存数据;
    memcpy(&(gst_His_Info[num]),&st_his_data, sizeof(ST_HISTORY_DATA));  // 保存数据
    printf("\n[&&&&&&&&&&&&&****]gst_His_Info[num].test_len == %d , st_his_data.test_len = %d \r\rn", gst_His_Info[num].test_len, st_his_data.test_len);
	int bottleid = (num%MAX_ONE_TEST_NUM)/MAX_ONE_GROUP_NUM;
    qDebug()<<QString("########## bottleid = %1 #### \r\n").arg(bottleid);


    //printf("\nbottleid === 0x%x &gst_His_Info_to_plc[bottleid] = 0x%x\r\n", &bottleid, &gst_His_Info_to_plc[bottleid]);
    qDebug()<<QString("########## bottleid = %1 #### num = %2 \r\n").arg(bottleid).arg(num);
	if(TOL_COLI_ID == g_work_Test_type[bottleid])  // 总大肠
    {
        id  =  TOLCOLI_START_NUM + MAX_ONE_GROUP_NUM*bottleid;
    }
    else if(FEC_COLI_ID == g_work_Test_type[bottleid])  // 耐热大肠
    {
        id  =  FECCOLI_START_NUM + MAX_ONE_GROUP_NUM*bottleid;
    }
    else if(E_COLI_ID == g_work_Test_type[bottleid]) // 埃希氏
    {
        id  =  ECOLI_START_NUM + MAX_ONE_GROUP_NUM*bottleid;
    }
    else if(TPC_COLI_ID == g_work_Test_type[bottleid])  // 菌落总数
    {
        id  =  TPCCOLI_START_NUM + MAX_ONE_GROUP_NUM*bottleid;
    }
    printf("\r\n id ==== %d  \r\n", id);
    test_len = -1;
    for(i = 0; i < MAX_ONE_GROUP_NUM; i++)
    {
        qDebug()<<QString("########## bottleid = %1 #### test_len = %2 \r\n").arg(id +1).arg(gst_His_Info[id + i].test_len);
        if(0 == gst_His_Info[id + i].test_len )
        {
            test_len = -1;
            break;
        }
        else
        {
            test_len += gst_His_Info[id + i].test_len;
        }
    }

     if(test_len > 0)  //  有数据
     {

        //printf("\n temp_nongdu = 0x%x  gst_His_Info_temp = 0x%x gst_His_Info_temp_2 = 0x%x \r\n", &temp_nongdu, &gst_His_Info_temp, &gst_His_Info_temp_2);

        memset(&(gst_His_Info_temp[0]), 0, sizeof(ST_HISTORY_DATA)*MAX_ONE_GROUP_NUM);
        memset(&gst_His_Info_temp_2, 0, sizeof(ST_HISTORY_DATA));
    // 获取数据
        mempcpy(&gst_His_Info_temp, &(gst_His_Info[id]), sizeof(ST_HISTORY_DATA)*MAX_ONE_GROUP_NUM);

        for(i = 0; i < MAX_ONE_GROUP_NUM; i++)
        {
            if(gst_His_Info_temp[0].test_len  > gst_His_Info_temp[i + 1].test_len)
            {
                memcpy(&gst_His_Info_temp_2, &gst_His_Info_temp[0], sizeof(ST_HISTORY_DATA));
                memcpy(&gst_His_Info_temp[0], &gst_His_Info_temp[i + 1], sizeof(ST_HISTORY_DATA));
                memcpy(&gst_His_Info_temp[i+1], &gst_His_Info_temp_2, sizeof(ST_HISTORY_DATA));
            }
        }
        for(i = 1; i < MAX_ONE_GROUP_NUM; i++)
        {
            if(gst_His_Info_temp[1].test_len  < gst_His_Info_temp[i + 1].test_len)
            {
                memcpy(&gst_His_Info_temp_2, &gst_His_Info_temp[1], sizeof(ST_HISTORY_DATA));
                memcpy(&gst_His_Info_temp[1], &gst_His_Info_temp[i + 1], sizeof(ST_HISTORY_DATA));
                memcpy(&gst_His_Info_temp[i+1], &gst_His_Info_temp_2, sizeof(ST_HISTORY_DATA));
            }
        }

        temp_nongdu = 0.0;
        test_len = 0;
        for(i = 2; i < MAX_ONE_GROUP_NUM ; i++)
        {
            qDebug()<<QString("<1>%% bottleid = [%1].MAX_ONE_GROUP_NUM = %2 ").arg(bottleid).arg(MAX_ONE_GROUP_NUM);
            qDebug()<<QString("<2>%% gst_His_Info[%1].test_len = %2 ").arg( i ).arg(gst_His_Info_temp[i].test_len);

            temp_nongdu += gst_His_Info_temp[i].NongD;
            qDebug()<<"temp_nongdu = "<<temp_nongdu;
            test_len += gst_His_Info_temp[i].test_len;
            qDebug()<<"test_len = "<<test_len;
        }

        gst_His_Info_to_plc[bottleid].test_len = test_len/(MAX_ONE_GROUP_NUM -2);  // 检测因子
        gst_His_Info_to_plc[bottleid].NongD = (float)temp_nongdu/(MAX_ONE_GROUP_NUM-2);		// 浓度
        gst_His_Info_to_plc[bottleid].unit_n = gst_His_Info_temp[0].unit_n;	//单位
        gst_His_Info_to_plc[bottleid].bt_id = bottleid + 1; // 上传瓶号 从1开始
        gst_His_Info_to_plc[bottleid].year = st_his_data.year; // 年
        gst_His_Info_to_plc[bottleid].mon = st_his_data.mon; // 月
        gst_His_Info_to_plc[bottleid].day = st_his_data.day; // 日
        gst_His_Info_to_plc[bottleid].hour = st_his_data.hour; // 时
        gst_His_Info_to_plc[bottleid].min = st_his_data.min; // 分
        gst_His_Info_to_plc[bottleid].sec = st_his_data.sec; // 秒

        qDebug()<<"test_len = 11111"<<test_len;
    }



    if(num < FECCOLI_START_NUM)//0-25总大肠
	{
        qDebug()<<"test_len = 3333 = "<<test_len;
		gp_sql->Add_One_Data(TOL_COLI_ID, &st_his_data);
	}
    else if(num < TPCCOLI_START_NUM)//26-50 耐热
	{
		gp_sql->Add_One_Data(FEC_COLI_ID, &st_his_data);
	}
    else if(num < ECOLI_START_NUM)  // 51-75 菌落总数
	{
		gp_sql->Add_One_Data(TPC_COLI_ID, &st_his_data);
	}
    else   //76-100 埃希氏
	{
		gp_sql->Add_One_Data(E_COLI_ID, &st_his_data);
	}

#if 1 //!TEST  //测试
    memset(pSt_TestInfo, 0, sizeof(ST_TEST_INFO));
	pSt_TestInfo->Test_st = TEST_ST_IDEL;  // 清除标志
#else
	pSt_TestInfo->Save_st = true;
#endif

    qDebug()<<"***********************8"<<test_len;

}
