#include "testing_thread.h"
#include "common.h"
#include "sql_control.h"

testing_thread *ptesting_thread;

testing_thread::testing_thread()
{
    timer_flag = false;  //timer 没有启动
    time_Testing = new QTimer();
    timer_out_flag = false;  // 时间到
 //   CalibrateFlag=false;
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
    {  /*  for(int i=0;i<MAX_TEST_NUM;i++)
            {
             qDebug()<<"测试之前状态 gst_Test_Info"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<gst_Test_Info[i].Ad_Max_Move_Time;
        }*/
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
            CalibrateFlag=false;

        }
        else if(idle_num == MAX_TEST_NUM)
        {
            if(true == timer_flag){
                time_Testing->stop();  //定时取消
                timer_flag = false;
                g_test_st = timer_flag;
                CalibrateFlag=true;
                qDebug()<<"定时取消**************";
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
        ST_PARA_SETTING st_temp;

      if(true==TOLParaCalibratedFlag || true==FECParaCalibratedFlag || true==TPCParaCalibratedFlag || true==EParaCalibratedFlag )//输入校正参数后才能计算  否则进入校正函数时  还未输校正参数就已经设为0  不能再进入函数了
      {
           if(true == CalibrateFlag)//重新计算校准之后的数据
          {
             for(int i=0;i<MAX_TEST_NUM;i++)
             {
                 if(i < TPCCOLI_START_NUM) //总大肠  耐热
                {
                 if(i < FECCOLI_START_NUM)//总大肠
                 {    qDebug()<<"输入总大肠  pSt_CalibrateInfo"<<"["<<i<<"]"<<".TestFlag="<<pSt_CalibrateInfo[i].TestFlag;
                     memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
                    if(true==TOLParaCalibratedFlag && true==pSt_CalibrateInfo[i].TestFlag) //设立一个测试标志 说明之前已经测试过 再设立一个计算方法选择 //输入了修正后的参数
                    {   qDebug()<<"总大肠i="<<i;
                         Get_Param1(TOL_COLI_ID, &st_temp,9,9);
                     //   TOLParaCalibratedFlag=false;
                       qDebug()<<"输出校正后参数 总大肠-st_temp.para_A="<<st_temp.para_A;
                       qDebug()<<"输出校正后参数 总大肠-st_temp.para_B="<<st_temp.para_B;
                 //      CalcMethodChoose(&pSt_CalibrateInfo[i],i,st_temp);
                       if(pSt_CalibrateInfo[i].Ad_Max_Move_Time != 0)
                       {   qDebug()<<"输入总大肠 pSt_CalibrateInfo"<<"["<<i<<"]"<<".BottleId="<<pSt_CalibrateInfo[i].BottleId;
                           qDebug()<<"输入总大肠 pSt_CalibrateInfo"<<"["<<i<<"]"<<".SampleID="<<pSt_CalibrateInfo[i].SampleID;
                           qDebug()<<"输入总大肠 pSt_CalibrateInfo"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<pSt_CalibrateInfo[i].Ad_Max_Move_Time;
                           qDebug()<<"输入  CalibrateFlag="<<CalibrateFlag;
							Time_Calculate(&pSt_CalibrateInfo[i] ,&st_temp);
							Titer_Cal(i, &pSt_CalibrateInfo[i] ,&st_temp);//浓度计算
					   }
					   else{
                           Save_Data(i, 1, &pSt_CalibrateInfo[i], &st_temp);
					   }
                       memset(&pSt_CalibrateInfo[i], 0, sizeof(ST_TEST_INFO)); //校准完之后清除
                       qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".BottleId="<<pSt_CalibrateInfo[i].BottleId;
                       qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".SampleID="<<pSt_CalibrateInfo[i].SampleID;
                       qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<pSt_CalibrateInfo[i].Ad_Max_Move_Time;
                       qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".TestFlag="<<pSt_CalibrateInfo[i].TestFlag;
                    }//if
                }
                 else if(i < TPCCOLI_START_NUM) //耐热
                 {  qDebug()<<"输入耐热  pSt_CalibrateInfo"<<"["<<i<<"]"<<".TestFlag="<<pSt_CalibrateInfo[i].TestFlag;
                     memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
                     if(true==FECParaCalibratedFlag && true==pSt_CalibrateInfo[i].TestFlag) //设立一个测试标志 说明之前已经测试过 再设立一个计算方法选择
                     {   qDebug()<<"耐热i="<<i;
                         Get_Param1(FEC_COLI_ID, &st_temp,9,9);
                      //   FECParaCalibratedFlag=false;
                         qDebug()<<"输出校正后参数 耐热大肠-st_temp.para_A="<<st_temp.para_A;
                         qDebug()<<"输出校正后参数 耐热大肠-st_temp.para_B="<<st_temp.para_B;
                    //     CalcMethodChoose(&pSt_CalibrateInfo[i],i,st_temp);
                         if(pSt_CalibrateInfo[i].Ad_Max_Move_Time != 0)
                         {   qDebug()<<"输入耐热大肠 pSt_CalibrateInfo"<<"["<<i<<"]"<<".BottleId="<<pSt_CalibrateInfo[i].BottleId;
                             qDebug()<<"输入耐热大肠 pSt_CalibrateInfo"<<"["<<i<<"]"<<".SampleID="<<pSt_CalibrateInfo[i].SampleID;
                             qDebug()<<"输入耐热大肠 pSt_CalibrateInfo"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<pSt_CalibrateInfo[i].Ad_Max_Move_Time;
                             qDebug()<<"输入  CalibrateFlag="<<CalibrateFlag;
                          Time_Calculate(&pSt_CalibrateInfo[i] ,&st_temp);
                          Titer_Cal(i, &pSt_CalibrateInfo[i] ,&st_temp);//浓度计算
                          }
						 else{
                             Save_Data(i, 1, &pSt_CalibrateInfo[i], &st_temp);
						 }
                         memset(&pSt_CalibrateInfo[i], 0, sizeof(ST_TEST_INFO)); //校准完之后清除
                         qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".BottleId="<<pSt_CalibrateInfo[i].BottleId;
                         qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".SampleID="<<pSt_CalibrateInfo[i].SampleID;
                         qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<pSt_CalibrateInfo[i].Ad_Max_Move_Time;
                         qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".TestFlag="<<pSt_CalibrateInfo[i].TestFlag;
                     }
                 }
               }
                 else
                {
                   if(i < ECOLI_START_NUM)  // 菌落总数
                   {  qDebug()<<"输入菌落总数  pSt_CalibrateInfo"<<"["<<i<<"]"<<".TestFlag="<<pSt_CalibrateInfo[i].TestFlag;
                       memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
                       if(true==TPCParaCalibratedFlag && true==pSt_CalibrateInfo[i].TestFlag) //设立一个测试标志 说明之前已经测试过 再设立一个计算方法选择
                       {   qDebug()<<"菌落总数i="<<i;
                           Get_Param1(TPC_COLI_ID, &st_temp,9,9);
                           qDebug()<<"输出校正后参数 菌落总数-st_temp.para_A="<<st_temp.para_A;
                           qDebug()<<"输出校正后参数 菌落总数-st_temp.para_B="<<st_temp.para_B;
                       //    TPCParaCalibratedFlag=false;
                      //     CalcMethodChoose(&pSt_CalibrateInfo[i],i,st_temp);
                           if(pSt_CalibrateInfo[i].Ad_Max_Move_Time != 0)
                           {   qDebug()<<"输入菌落总数 pSt_CalibrateInfo"<<"["<<i<<"]"<<".BottleId="<<pSt_CalibrateInfo[i].BottleId;
                               qDebug()<<"输入菌落总数 pSt_CalibrateInfo"<<"["<<i<<"]"<<".SampleID="<<pSt_CalibrateInfo[i].SampleID;
                               qDebug()<<"输入菌落总数 pSt_CalibrateInfo"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<pSt_CalibrateInfo[i].Ad_Max_Move_Time;
                               qDebug()<<"输入  CalibrateFlag="<<CalibrateFlag;
								Time_Calculate(&pSt_CalibrateInfo[i] ,&st_temp);
								Titer_Cal(i, &pSt_CalibrateInfo[i] ,&st_temp);//浓度计算
                            }
						   else{
                               Save_Data(i, 1, &pSt_CalibrateInfo[i], &st_temp);
						   }
                           memset(&pSt_CalibrateInfo[i], 0, sizeof(ST_TEST_INFO)); //校准完之后清除
                           qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".BottleId="<<pSt_CalibrateInfo[i].BottleId;
                           qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".SampleID="<<pSt_CalibrateInfo[i].SampleID;
                           qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<pSt_CalibrateInfo[i].Ad_Max_Move_Time;
                           qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".TestFlag="<<pSt_CalibrateInfo[i].TestFlag;
                       }
                   }
                   else
                   {   qDebug()<<"输入埃希氏  pSt_CalibrateInfo"<<"["<<i<<"]"<<".TestFlag="<<pSt_CalibrateInfo[i].TestFlag;
                       memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
                      if(true==EParaCalibratedFlag && true==pSt_CalibrateInfo[i].TestFlag) //设立一个测试标志 说明之前已经测试过 再设立一个计算方法选择
					   {
							qDebug()<<"埃希氏i="<<i;
							Get_Param1(E_COLI_ID, &st_temp,9,9);
							qDebug()<<"输出校正后参数 埃希氏-st_temp.para_A="<<st_temp.para_A;
							qDebug()<<"输出校正后参数 埃希氏-st_temp.para_B="<<st_temp.para_B;
                      //    EParaCalibratedFlag=false;
                       //    CalcMethodChoose(&pSt_CalibrateInfo[i],i,st_temp);
                          if(pSt_CalibrateInfo[i].Ad_Max_Move_Time != 0)
                          {   qDebug()<<"输入埃希氏 pSt_CalibrateInfo"<<"["<<i<<"]"<<".BottleId="<<pSt_CalibrateInfo[i].BottleId;
                              qDebug()<<"输入埃希氏 pSt_CalibrateInfo"<<"["<<i<<"]"<<".SampleID="<<pSt_CalibrateInfo[i].SampleID;
                              qDebug()<<"输入埃希氏 pSt_CalibrateInfo"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<pSt_CalibrateInfo[i].Ad_Max_Move_Time;
                              qDebug()<<"输入  CalibrateFlag="<<CalibrateFlag;
								Time_Calculate(&pSt_CalibrateInfo[i] ,&st_temp);
								Titer_Cal(i, &pSt_CalibrateInfo[i] ,&st_temp);//浓度计算
                           }
						  else{
                              Save_Data(i, 1, &pSt_CalibrateInfo[i], &st_temp);
						  }
                           memset(&pSt_CalibrateInfo[i], 0, sizeof(ST_TEST_INFO)); //校准完之后清除
                           qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".BottleId="<<pSt_CalibrateInfo[i].BottleId;
                           qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".SampleID="<<pSt_CalibrateInfo[i].SampleID;
                           qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<pSt_CalibrateInfo[i].Ad_Max_Move_Time;
                           qDebug()<<"清除----pSt_CalibrateInfo"<<"["<<i<<"]"<<".TestFlag="<<pSt_CalibrateInfo[i].TestFlag;
                       }
                   }
                }
             }//for
              TOLParaCalibratedFlag=false; //for循环结束后 较标结束
              FECParaCalibratedFlag=false;
              TPCParaCalibratedFlag=false;
              EParaCalibratedFlag=false;
         } //true == CalibrateFlag  if
       }//或if
     }//while
}
//
void testing_thread::CalcMethodChoose(ST_TEST_INFO *gst_Test_Info,int i,ST_PARA_SETTING st_temp)
{         qDebug()<<"gst_Test_Info"<<"["<<i<<"]"<<".CalcMethod="<<gst_Test_Info[i].CalcMethod;
          qDebug()<<"输入 gst_Test_Info"<<"["<<i<<"]"<<".BottleId="<<gst_Test_Info[i].BottleId;
          qDebug()<<"输入 gst_Test_Info"<<"["<<i<<"]"<<".SampleID="<<gst_Test_Info[i].SampleID;
          qDebug()<<"输入 gst_Test_Info"<<"["<<i<<"]"<<".Ad_Max_Move_Time="<<gst_Test_Info[i].Ad_Max_Move_Time;
          qDebug()<<"输入-st_temp.para_A="<<st_temp.para_A;
          qDebug()<<"输入-st_temp.para_B="<<st_temp.para_B;

                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                {
                 Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                 Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                 }
                else
                    Save_Data(i, 1, &gst_Test_Info[i], &st_temp);
   /*       int num = 0;
         switch (gst_Test_Info[i].CalcMethod)
          {
           case 1:case 2:case 17:
            Time_Calculate(&gst_Test_Info[i] ,&st_temp);
            Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
            break;
            case 3:case 5:case 7:case 9:case 11:case 13:
            if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
            {
             Time_Calculate(&gst_Test_Info[i] ,&st_temp);
             Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
             }
             break;
            case 4:
            while(num < 80)
            {
             num = qrand()%100;
            }
             Save_Data(i, num, &gst_Test_Info[i], &st_temp);
             break;
           case 6:
             while((num < 60) || (num > 80))
             {
              num = qrand()%80;
             }
             Save_Data(i, num, &gst_Test_Info[i], &st_temp);
             break;
             case 8:
             while((num < 40) || (num > 60))
             {
              num = qrand()%60;
             }
             Save_Data(i, num, &gst_Test_Info[i], &st_temp);
             break;
            case 10:
             while((num < 20) || (num > 40))
             {
              num = qrand()%40;
             }
            Save_Data(i, num, &gst_Test_Info[i], &st_temp);
             break;
            case 12:
            while((num < 10) || (num > 20))
             {
               num = qrand()%80;
              }
              Save_Data(i, num, &gst_Test_Info[i], &st_temp);
             break;
            case 14:
              while((num < 1) || (num > 10))
              {
                 num = qrand()%10;
              }
               Save_Data(i, num, &gst_Test_Info[i], &st_temp);
               case 15:case 16:
               gst_Test_Info[i].Ad_Max_Move_Time = g_Test_Over_Time;
                Save_Data(i, 1, &gst_Test_Info[i], &st_temp);
             break;
              default:
                 break;
        }//switch*/
}
// 测试
void testing_thread::Testing()
{
    int i, id = 0;

   // ST_HISTORY_DATA st_his_data;
     ST_HISTORY_DATA1 st_his_data;

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

                 //   st_his_data.NongDu = QString::number(gst_Test_Info[id].ThouData_dis);
                //    st_his_data.Unit = QString::number(gst_Test_Info[id].Ad_LightData_Cur);
                      st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id].Ad_DarkData_Begin);
                      st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id].Ad_DarkData_Cur);
                      st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id].Ad_LightData_Begin);
                      st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id].Ad_LightData_Cur);
                      st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id].Ad_LightData_Last);
                      st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id].Ad_Max_Move_Data);
                      st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id].Ad_Max_Move_Time);
                      st_his_data.Last_ThouData= QString::number(gst_Test_Info[id].Last_ThouData);
                      st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id].ThouData_Cur);
                      st_his_data.ThouData_dis= QString::number(gst_Test_Info[id].ThouData_dis);
                      st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id].sThouData_Cnt);
                      gp_sql->Add_One_Data_forTest(id, &st_his_data);
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 ******** \r\n").arg(id).arg(gst_Test_Info[id].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+ MAX_ONE_GROUP_NUM].Test_st) //第二组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM);

                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
                    ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);

               //     st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
              //      st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
                    ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);

              //      st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
             //       st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*2, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*2).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
                    ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);

                //    st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
              //      st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*3, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*3).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
                    ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);

              //      st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
             //       st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].sThouData_Cnt);
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

                //    st_his_data.NongDu = QString::number(gst_Test_Info[id].ThouData_dis);
                //    st_his_data.Unit = QString::number(gst_Test_Info[id].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id, &st_his_data);
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 ******** \r\n").arg(id).arg(gst_Test_Info[id].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+ MAX_ONE_GROUP_NUM].Test_st) //第二组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM);

                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
                    ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);

               //     st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
               //     st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
                    ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);

                //    st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
               //     st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*2, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*2).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
                    ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);

               //     st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
               //     st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*3, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*3).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
                    ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);

              //      st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
              //      st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].sThouData_Cnt);
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

               //     st_his_data.NongDu = QString::number(gst_Test_Info[id].ThouData_dis);
               //     st_his_data.Unit = QString::number(gst_Test_Info[id].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id, &st_his_data);
                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 ******** \r\n").arg(id).arg(gst_Test_Info[id].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+ MAX_ONE_GROUP_NUM].Test_st) //第二组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM);

                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM]);

              //      st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
               //     st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Test_st) //第三组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*2);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*2]);

                 //   st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
                 //   st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*2, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*2).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*2].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Test_st) //第四组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*3);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*3]);

               //     st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
              //      st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].sThouData_Cnt);
                    gp_sql->Add_One_Data_forTest(id + MAX_ONE_GROUP_NUM*3, &st_his_data);

                    //qDebug()<<QString("gst_Test_Info[%1].ThouData_Cur == %2 **********").arg(id+MAX_ONE_GROUP_NUM*3).arg(gst_Test_Info[id+MAX_ONE_GROUP_NUM*3].ThouData_Cur);
                }
                if(TEST_ST_TESTING == gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Test_st) //第四组
                {
                    gst_Test_Info[id+MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur = Get_ad(id+MAX_ONE_GROUP_NUM*4);
                    ThruRate_Calculate(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
                    //ThouData_Smooth(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);
                    E_Test_Cal(&gst_Test_Info[id + MAX_ONE_GROUP_NUM*4]);

                 //   st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
                //    st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].sThouData_Cnt);
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

               //     st_his_data.NongDu = QString::number(gst_Test_Info[id].ThouData_dis);
                //    st_his_data.Unit = QString::number(gst_Test_Info[id].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id].sThouData_Cnt);
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

                //    st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
               //     st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM].sThouData_Cnt);
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

                 //   st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
                //    st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*2].sThouData_Cnt);
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

              //      st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
              //      st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*3].sThouData_Cnt);
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

             //       st_his_data.NongDu = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
             //       st_his_data.Unit = QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
                    st_his_data.Ad_DarkData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_DarkData_Begin);
                    st_his_data.Ad_DarkData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_DarkData_Cur);
                    st_his_data.Ad_LightData_Begin= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Begin);
                    st_his_data.Ad_LightData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Cur);
                    st_his_data.Ad_LightData_Last= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_LightData_Last);
                    st_his_data.Ad_Max_Move_Data= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_Max_Move_Data);
                    st_his_data.Ad_Max_Move_Time= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Ad_Max_Move_Time);
                    st_his_data.Last_ThouData= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].Last_ThouData);
                    st_his_data.ThouData_Cur= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_Cur);
                    st_his_data.ThouData_dis= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].ThouData_dis);
                    st_his_data.sThouData_Cnt= QString::number(gst_Test_Info[id + MAX_ONE_GROUP_NUM*4].sThouData_Cnt);
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
        pSt_TestInfo->ThouData_Cur = (100 - pSt_TestInfo->ThouData_dis)*STANDARD_PARAM; //吸光度
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
void testing_thread::Get_Param1(int mode, ST_PARA_SETTING *st,int BottleClassNo,int BottleClassNo_1)
{
    qDebug()<<"每个水样曲线的输出****";
    qDebug()<<"输入----BottleClassNo="<<BottleClassNo;
    qDebug()<<"输入----BottleClassNo_1="<<BottleClassNo_1;
    QFile file(FILE_QX_PARA_SETTING);  //
    file.open(QIODevice::ReadOnly);  // 打开文件 只读

//	printf("mode = %d \r\n", mode);
    if(TOL_COLI_ID == mode)  // 总大肠
    {
        file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1));
        //memset(st, 0,PRAR_ST_DATA_LEN);
        file.read((char*)st, PRAR_ST_DATA_LEN);
        printf("PRAR_TOL_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1) = %d  \r\n", PRAR_TOL_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1));
    }
    else if(FEC_COLI_ID == mode)  // 耐热大肠
    {
        file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1));
//      file.seek(PRAR_FEC_START+PRAR_ST_DATA_LEN*ClassNo);
        //memset(st, 0,PRAR_ST_DATA_LEN);
        file.read((char*)st, PRAR_ST_DATA_LEN);
        printf("PRAR_FEC_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1) = %d  \r\n", PRAR_FEC_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1));
    }
    else if(TPC_COLI_ID == mode)  // 菌落总数
    {
        file.seek(PRAR_TPC_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1));
        //file.seek(PRAR_TPC_START+PRAR_ST_DATA_LEN*ClassNo);
//		memset(&st, 0,PRAR_ST_DATA_LEN);
        file.read((char*)st, PRAR_ST_DATA_LEN);
        printf("PRAR_TPC_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1) = %d  \r\n", PRAR_TPC_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1));
    }
    else if(E_COLI_ID == mode)  // 埃希氏
    {
        file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1));
       // file.seek(PRAR_E_START+PRAR_ST_DATA_LEN*ClassNo);
//		memset(&st, 0,PRAR_ST_DATA_LEN);
        file.read((char*)st, PRAR_ST_DATA_LEN);
        printf("PRAR_E_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1) = %d  \r\n", PRAR_E_START + PRAR_ST_DATA_LEN*(BottleClassNo*PRAR_MAX_CLASS_NUM + BottleClassNo_1));
    }
    file.close();

//	printf("st.para_A = %f st.para_B= %f, st.para_C= %f\r\n", st->para_A, st->para_B, st->para_C);
//	printf("st.para_D = %f st.para_Q = %f, st.para_R= %f\r\n", st->para_D, st->para_Q, st->para_R);

}
/*********************************************************************************
* 名称：MasterOverTimeCheck()
* 功能：超时检测
* 入口参数：无
* 出口参数：0：无超时 1：有瓶超时
*********************************************************************************/
bool testing_thread::
Check_OverTime(void)
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
            {   gst_Test_Info[i].TestFlag=true;
                 qDebug()<<"输入总大肠  gst_Test_Info"<<"["<<i<<"]"<<".TestFlag="<<gst_Test_Info[i].TestFlag;
//				qDebug()<<QString("i =<2>====%1 @@@ \r\n").arg(i);
                //RefReadTab();  // 获取参数 后续完成
                if(i < FECCOLI_START_NUM)//0-7总大肠
                {  /* if(false==TOLTestNumberVector.contains(gst_Test_Info[i].BottleId))
                        TOLTestNumberVector.prepend(gst_Test_Info[i].BottleId);
                     for(int j=0;j<TOLTestNumberVector.size();j++)
                      qDebug()<<"TOLTestNumberVector"<<"["<<j<<"]="<<TOLTestNumberVector.at(j);*/
                    memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
                 //   Get_Param(TOL_COLI_ID, &st_temp);
                    switch (i%MAX_ONE_TEST_NUM) {   //定位到瓶号
                    case 0:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle1_ClassNo,Bottle1_ClassNo_1);
                        qDebug()<<"Bottle1_ClassNo-总大肠"<<Bottle1_ClassNo;
                        qDebug()<<"Bottle1_ClassNo_1-总大肠"<<Bottle1_ClassNo_1;
                        qDebug()<<"1号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"1号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"1号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"1号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 1:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle2_ClassNo,Bottle2_ClassNo_1);
                        qDebug()<<"Bottle2_ClassNo-总大肠"<<Bottle2_ClassNo;
                        qDebug()<<"Bottle2_ClassNo_1-总大肠"<<Bottle2_ClassNo_1;
                        qDebug()<<"2号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"2号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"2号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"2号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 2:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle3_ClassNo,Bottle3_ClassNo_1);
                        qDebug()<<"Bottle3_ClassNo-总大肠"<<Bottle3_ClassNo;
                        qDebug()<<"Bottle3_ClassNo_1-总大肠"<<Bottle3_ClassNo_1;
                        qDebug()<<"3号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"3号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"3号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"3号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 3:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle4_ClassNo,Bottle4_ClassNo_1);
                        qDebug()<<"Bottle4_ClassNo-总大肠"<<Bottle4_ClassNo;
                        qDebug()<<"Bottle4_ClassNo_1-总大肠"<<Bottle4_ClassNo_1;
                        qDebug()<<"4号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"4号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"4号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"4号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 4:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle5_ClassNo,Bottle5_ClassNo_1);
                        qDebug()<<"Bottle5_ClassNo-总大肠"<<Bottle5_ClassNo;
                        qDebug()<<"Bottle5_ClassNo_1-总大肠"<<Bottle5_ClassNo_1;
                        qDebug()<<"5号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"5号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"5号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"5号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 5:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle6_ClassNo,Bottle6_ClassNo_1);
                        qDebug()<<"Bottle6_ClassNo-总大肠"<<Bottle6_ClassNo;
                        qDebug()<<"Bottle6_ClassNo_1-总大肠"<<Bottle6_ClassNo_1;
                        qDebug()<<"6号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"6号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"6号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"6号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 6:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle7_ClassNo,Bottle7_ClassNo_1);
                        qDebug()<<"Bottle7_ClassNo-总大肠"<<Bottle7_ClassNo;
                        qDebug()<<"Bottle7_ClassNo_1-总大肠"<<Bottle7_ClassNo_1;
                        qDebug()<<"7号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"7号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"7号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"7号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 7:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle8_ClassNo,Bottle8_ClassNo_1);
                        qDebug()<<"Bottle8_ClassNo-总大肠"<<Bottle8_ClassNo;
                        qDebug()<<"Bottle8_ClassNo_1-总大肠"<<Bottle8_ClassNo_1;
                        qDebug()<<"8号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"8号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"8号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"8号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 8:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle9_ClassNo,Bottle9_ClassNo_1);
                        qDebug()<<"Bottle9_ClassNo-总大肠"<<Bottle9_ClassNo;
                        qDebug()<<"Bottle9_ClassNo_1-总大肠"<<Bottle9_ClassNo_1;
                        qDebug()<<"9号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"9号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"9号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"9号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 9:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle10_ClassNo,Bottle10_ClassNo_1);
                        qDebug()<<"Bottle10_ClassNo-总大肠"<<Bottle10_ClassNo;
                        qDebug()<<"Bottle10_ClassNo_1-总大肠"<<Bottle10_ClassNo_1;
                        qDebug()<<"10号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"10号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"10号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"10号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 10:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle11_ClassNo,Bottle11_ClassNo_1);
                        qDebug()<<"Bottle11_ClassNo-总大肠"<<Bottle11_ClassNo;
                        qDebug()<<"Bottle11_ClassNo_1-总大肠"<<Bottle11_ClassNo_1;
                        qDebug()<<"11号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"11号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"11号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"11号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 11:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle12_ClassNo,Bottle12_ClassNo_1);
                        qDebug()<<"Bottle12_ClassNo-总大肠"<<Bottle12_ClassNo;
                        qDebug()<<"Bottle12_ClassNo_1-总大肠"<<Bottle12_ClassNo_1;
                        qDebug()<<"12号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"12号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"12号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"12号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 12:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle13_ClassNo,Bottle13_ClassNo_1);
                        qDebug()<<"Bottle13_ClassNo-总大肠"<<Bottle13_ClassNo;
                        qDebug()<<"Bottle13_ClassNo_1-总大肠"<<Bottle13_ClassNo_1;
                        qDebug()<<"13号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"13号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"13号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"13号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 13:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle14_ClassNo,Bottle14_ClassNo_1);
                        qDebug()<<"Bottle14_ClassNo-总大肠"<<Bottle14_ClassNo;
                        qDebug()<<"Bottle14_ClassNo_1-总大肠"<<Bottle14_ClassNo_1;
                        qDebug()<<"14号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"14号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"14号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"14号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 14:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle15_ClassNo,Bottle15_ClassNo_1);
                        qDebug()<<"Bottle15_ClassNo-总大肠"<<Bottle15_ClassNo;
                        qDebug()<<"Bottle15_ClassNo_1-总大肠"<<Bottle15_ClassNo_1;
                        qDebug()<<"15号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"15号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"15号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"15号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 15:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle16_ClassNo,Bottle16_ClassNo_1);
                        qDebug()<<"Bottle16_ClassNo-总大肠"<<Bottle16_ClassNo;
                        qDebug()<<"Bottle16_ClassNo_1-总大肠"<<Bottle16_ClassNo_1;
                        qDebug()<<"16号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"16号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"16号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"16号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 16:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle17_ClassNo,Bottle17_ClassNo_1);
                        qDebug()<<"Bottle17_ClassNo-总大肠"<<Bottle17_ClassNo;
                        qDebug()<<"Bottle17_ClassNo_1-总大肠"<<Bottle17_ClassNo_1;
                        qDebug()<<"17号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"17号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"17号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"17号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 17:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle18_ClassNo,Bottle18_ClassNo_1);
                        qDebug()<<"Bottle18_ClassNo-总大肠"<<Bottle18_ClassNo;
                        qDebug()<<"Bottle18_ClassNo_1-总大肠"<<Bottle18_ClassNo_1;
                        qDebug()<<"18号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"18号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"18号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"18号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 18:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle19_ClassNo,Bottle19_ClassNo_1);
                        qDebug()<<"Bottle19_ClassNo-总大肠"<<Bottle19_ClassNo;
                        qDebug()<<"Bottle19_ClassNo_1-总大肠"<<Bottle19_ClassNo_1;
                        qDebug()<<"19号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"19号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"19号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"19号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 19:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle20_ClassNo,Bottle20_ClassNo_1);
                        qDebug()<<"Bottle20_ClassNo-总大肠"<<Bottle20_ClassNo;
                        qDebug()<<"Bottle20_ClassNo_1-总大肠"<<Bottle20_ClassNo_1;
                        qDebug()<<"20号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"20号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"20号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"20号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 20:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle21_ClassNo,Bottle21_ClassNo_1);
                        qDebug()<<"Bottle21_ClassNo-总大肠"<<Bottle21_ClassNo;
                        qDebug()<<"Bottle21_ClassNo_1-总大肠"<<Bottle21_ClassNo_1;
                        qDebug()<<"21号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"21号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"21号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"21号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 21:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle22_ClassNo,Bottle22_ClassNo_1);
                        qDebug()<<"Bottle22_ClassNo-总大肠"<<Bottle22_ClassNo;
                        qDebug()<<"Bottle22_ClassNo_1-总大肠"<<Bottle22_ClassNo_1;
                        qDebug()<<"22号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"22号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"22号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"22号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 22:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle23_ClassNo,Bottle23_ClassNo_1);
                        qDebug()<<"Bottle23_ClassNo-总大肠"<<Bottle23_ClassNo;
                        qDebug()<<"Bottle23_ClassNo_1-总大肠"<<Bottle23_ClassNo_1;
                        qDebug()<<"23号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"23号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"23号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"23号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 23:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle24_ClassNo,Bottle24_ClassNo_1);
                        qDebug()<<"Bottle24_ClassNo-总大肠"<<Bottle24_ClassNo;
                        qDebug()<<"Bottle24_ClassNo_1-总大肠"<<Bottle24_ClassNo_1;
                        qDebug()<<"24号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"24号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"24号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"24号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    case 24:
                        Get_Param1(TOL_COLI_ID, &st_temp,Bottle25_ClassNo,Bottle25_ClassNo_1);
                        qDebug()<<"Bottle25_ClassNo-总大肠"<<Bottle25_ClassNo;
                        qDebug()<<"Bottle25_ClassNo_1-总大肠"<<Bottle25_ClassNo_1;
                        qDebug()<<"25号瓶 st_temp.para_A-总大肠"<<st_temp.para_A;
                        qDebug()<<"25号瓶 st_temp.para_B-总大肠"<<st_temp.para_B;
                        qDebug()<<"25号瓶 st_temp.para_C-总大肠"<<st_temp.para_C;
                        qDebug()<<"25号瓶 st_temp.para_D-总大肠"<<st_temp.para_D;
                        break;
                    default:
                        break;
                    }
                }
                else if(i < TPCCOLI_START_NUM)//8-15 耐热
                { /*   if(false==FECTestNumberVector.contains(gst_Test_Info[i].BottleId))
                        FECTestNumberVector.prepend(gst_Test_Info[i].BottleId);
                     for(int k=0;k<FECTestNumberVector.size();k++)
                     qDebug()<<"FECTestNumberVector"<<"["<<k<<"]="<<FECTestNumberVector.at(k);*/
                     gst_Test_Info[i].TestFlag=true;
                      qDebug()<<"输入耐热  gst_Test_Info"<<"["<<i<<"]"<<".TestFlag="<<gst_Test_Info[i].TestFlag;
                    memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
                   // Get_Param(FEC_COLI_ID, &st_temp);
                    switch (i%MAX_ONE_TEST_NUM) {   //定位到瓶号
                    case 0:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle1_ClassNo,Bottle1_ClassNo_1);
                        qDebug()<<"Bottle1_ClassNo-耐热大肠"<<Bottle1_ClassNo;
                        qDebug()<<"Bottle1_ClassNo_1-耐热大肠"<<Bottle1_ClassNo_1;
                        qDebug()<<"1号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"1号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"1号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"1号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 1:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle2_ClassNo,Bottle2_ClassNo_1);
                        qDebug()<<"Bottle2_ClassNo-耐热大肠"<<Bottle2_ClassNo;
                        qDebug()<<"Bottle2_ClassNo_1-耐热大肠"<<Bottle2_ClassNo_1;
                        qDebug()<<"2号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"2号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"2号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"2号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 2:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle3_ClassNo,Bottle3_ClassNo_1);
                        qDebug()<<"Bottle3_ClassNo-耐热大肠"<<Bottle3_ClassNo;
                        qDebug()<<"Bottle3_ClassNo_1-耐热大肠"<<Bottle3_ClassNo_1;
                        qDebug()<<"3号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"3号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"3号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"3号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 3:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle4_ClassNo,Bottle4_ClassNo_1);
                        qDebug()<<"Bottle4_ClassNo-耐热大肠"<<Bottle4_ClassNo;
                        qDebug()<<"Bottle4_ClassNo_1-耐热大肠"<<Bottle4_ClassNo_1;
                        qDebug()<<"4号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"4号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"4号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"4号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 4:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle5_ClassNo,Bottle5_ClassNo_1);
                        qDebug()<<"Bottle5_ClassNo-耐热大肠"<<Bottle5_ClassNo;
                        qDebug()<<"Bottle5_ClassNo_1-耐热大肠"<<Bottle5_ClassNo_1;
                        qDebug()<<"5号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"5号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"5号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"5号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 5:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle6_ClassNo,Bottle6_ClassNo_1);
                        qDebug()<<"Bottle6_ClassNo-耐热大肠"<<Bottle6_ClassNo;
                        qDebug()<<"Bottle6_ClassNo_1-耐热大肠"<<Bottle6_ClassNo_1;
                        qDebug()<<"6号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"6号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"6号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"6号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 6:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle7_ClassNo,Bottle7_ClassNo_1);
                        qDebug()<<"Bottle7_ClassNo-耐热大肠"<<Bottle7_ClassNo;
                        qDebug()<<"Bottle7_ClassNo_1-耐热大肠"<<Bottle7_ClassNo_1;
                        qDebug()<<"7号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"7号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"7号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"7号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 7:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle8_ClassNo,Bottle8_ClassNo_1);
                        qDebug()<<"Bottle8_ClassNo-耐热大肠"<<Bottle8_ClassNo;
                        qDebug()<<"Bottle8_ClassNo_1-耐热大肠"<<Bottle8_ClassNo_1;
                        qDebug()<<"8号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"8号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"8号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"8号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 8:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle9_ClassNo,Bottle9_ClassNo_1);
                        qDebug()<<"Bottle9_ClassNo-耐热大肠"<<Bottle9_ClassNo;
                        qDebug()<<"Bottle9_ClassNo_1-耐热大肠"<<Bottle9_ClassNo_1;
                        qDebug()<<"9号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"9号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"9号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"9号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 9:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle10_ClassNo,Bottle10_ClassNo_1);
                        qDebug()<<"Bottle10_ClassNo-耐热大肠"<<Bottle10_ClassNo;
                        qDebug()<<"Bottle10_ClassNo_1-耐热大肠"<<Bottle10_ClassNo_1;
                        qDebug()<<"10号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"10号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"10号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"10号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 10:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle11_ClassNo,Bottle11_ClassNo_1);
                        qDebug()<<"Bottle11_ClassNo-耐热大肠"<<Bottle11_ClassNo;
                        qDebug()<<"Bottle11_ClassNo_1-耐热大肠"<<Bottle11_ClassNo_1;
                        qDebug()<<"11号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"11号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"11号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"11号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 11:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle12_ClassNo,Bottle12_ClassNo_1);
                        qDebug()<<"Bottle12_ClassNo-耐热大肠"<<Bottle12_ClassNo;
                        qDebug()<<"Bottle12_ClassNo_1-耐热大肠"<<Bottle12_ClassNo_1;
                        qDebug()<<"12号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"12号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"12号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"12号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 12:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle13_ClassNo,Bottle13_ClassNo_1);
                        qDebug()<<"Bottle13_ClassNo-耐热大肠"<<Bottle13_ClassNo;
                        qDebug()<<"Bottle13_ClassNo_1-耐热大肠"<<Bottle13_ClassNo_1;
                        qDebug()<<"13号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"13号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"13号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"13号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 13:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle14_ClassNo,Bottle14_ClassNo_1);
                        qDebug()<<"Bottle14_ClassNo-耐热大肠"<<Bottle14_ClassNo;
                        qDebug()<<"Bottle14_ClassNo_1-耐热大肠"<<Bottle14_ClassNo_1;
                        qDebug()<<"14号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"14号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"14号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"14号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 14:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle15_ClassNo,Bottle15_ClassNo_1);
                        qDebug()<<"Bottle15_ClassNo-耐热大肠"<<Bottle15_ClassNo;
                        qDebug()<<"Bottle15_ClassNo_1-耐热大肠"<<Bottle15_ClassNo_1;
                        qDebug()<<"15号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"15号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"15号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"15号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 15:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle16_ClassNo,Bottle16_ClassNo_1);
                        qDebug()<<"Bottle16_ClassNo-耐热大肠"<<Bottle16_ClassNo;
                        qDebug()<<"Bottle16_ClassNo_1-耐热大肠"<<Bottle16_ClassNo_1;
                        qDebug()<<"16号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"16号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"16号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"16号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 16:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle17_ClassNo,Bottle17_ClassNo_1);
                        qDebug()<<"Bottle17_ClassNo-耐热大肠"<<Bottle17_ClassNo;
                        qDebug()<<"Bottle17_ClassNo_1-耐热大肠"<<Bottle17_ClassNo_1;
                        qDebug()<<"17号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"17号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"17号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"17号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 17:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle18_ClassNo,Bottle18_ClassNo_1);
                        qDebug()<<"Bottle18_ClassNo-耐热大肠"<<Bottle18_ClassNo;
                        qDebug()<<"Bottle18_ClassNo_1-耐热大肠"<<Bottle18_ClassNo_1;
                        qDebug()<<"18号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"18号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"18号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"18号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 18:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle19_ClassNo,Bottle19_ClassNo_1);
                        qDebug()<<"Bottle19_ClassNo-耐热大肠"<<Bottle19_ClassNo;
                        qDebug()<<"Bottle19_ClassNo_1-耐热大肠"<<Bottle19_ClassNo_1;
                        qDebug()<<"19号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"19号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"19号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"19号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 19:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle20_ClassNo,Bottle20_ClassNo_1);
                        qDebug()<<"Bottle20_ClassNo-耐热大肠"<<Bottle20_ClassNo;
                        qDebug()<<"Bottle20_ClassNo_1-耐热大肠"<<Bottle20_ClassNo_1;
                        qDebug()<<"20号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"20号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"20号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"20号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 20:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle21_ClassNo,Bottle21_ClassNo_1);
                        qDebug()<<"Bottle21_ClassNo-耐热大肠"<<Bottle21_ClassNo;
                        qDebug()<<"Bottle21_ClassNo_1-耐热大肠"<<Bottle21_ClassNo_1;
                        qDebug()<<"21号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"21号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"21号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"21号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 21:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle22_ClassNo,Bottle22_ClassNo_1);
                        qDebug()<<"Bottle22_ClassNo-耐热大肠"<<Bottle22_ClassNo;
                        qDebug()<<"Bottle22_ClassNo_1-耐热大肠"<<Bottle22_ClassNo_1;
                        qDebug()<<"22号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"22号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"22号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"22号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 22:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle23_ClassNo,Bottle23_ClassNo_1);
                        qDebug()<<"Bottle23_ClassNo-耐热大肠"<<Bottle23_ClassNo;
                        qDebug()<<"Bottle23_ClassNo_1-耐热大肠"<<Bottle23_ClassNo_1;
                        qDebug()<<"23号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"23号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"23号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"23号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 23:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle24_ClassNo,Bottle24_ClassNo_1);
                        qDebug()<<"Bottle24_ClassNo-耐热大肠"<<Bottle24_ClassNo;
                        qDebug()<<"Bottle24_ClassNo_1-耐热大肠"<<Bottle24_ClassNo_1;
                        qDebug()<<"24号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"24号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"24号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"24号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    case 24:
                        Get_Param1(FEC_COLI_ID, &st_temp,Bottle25_ClassNo,Bottle25_ClassNo_1);
                        qDebug()<<"Bottle25_ClassNo-耐热大肠"<<Bottle25_ClassNo;
                        qDebug()<<"Bottle25_ClassNo_1-耐热大肠"<<Bottle25_ClassNo_1;
                        qDebug()<<"25号瓶 st_temp.para_A-耐热大肠"<<st_temp.para_A;
                        qDebug()<<"25号瓶 st_temp.para_B-耐热大肠"<<st_temp.para_B;
                        qDebug()<<"25号瓶 st_temp.para_C-耐热大肠"<<st_temp.para_C;
                        qDebug()<<"25号瓶 st_temp.para_D-耐热大肠"<<st_temp.para_D;
                        break;
                    default:
                        break;
                    }
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
                        CalcMethod[i]=1;
                        gst_Test_Info[i].CalcMethod=1;
                        qDebug()<<"结果计算方式1***";
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
                                CalcMethod[i]=2;
                                 gst_Test_Info[i].CalcMethod=2;
                                 qDebug()<<"结果计算方式2***";
                            }
                            else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_60)  // 吸光度 60%
                            {
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                    CalcMethod[i]=3;
                                     gst_Test_Info[i].CalcMethod=3;
                                     qDebug()<<"结果计算方式3***";
                                }
                                else{
                                    int num = 0;

                                    while(num < 80)
                                    {
                                        num = qrand()%100;
                                    }

                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                    CalcMethod[i]=4;
                                     gst_Test_Info[i].CalcMethod=4;
                                     qDebug()<<"结果计算方式4***";
                                }
                            }
                            else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_50)// 吸光度 50%
                            {
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                    CalcMethod[i]=5;
                                     gst_Test_Info[i].CalcMethod=5;
                                     qDebug()<<"结果计算方式5***";
                                }
                                else{
                                    int num = 0;

                                    while((num < 60) || (num > 80))
                                    {
                                        num = qrand()%80;
                                    }

                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                    CalcMethod[i]=6;
                                     gst_Test_Info[i].CalcMethod=6;
                                     qDebug()<<"结果计算方式6***";
                                }
                            }
                            else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_40) // 吸光度 40%
                            {
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                    CalcMethod[i]=7;
                                    gst_Test_Info[i].CalcMethod=7;
                                    qDebug()<<"结果计算方式7***";
                                }
                                else{
                                    int num = 0;

                                    while((num < 40) || (num > 60))
                                    {
                                        num = qrand()%60;
                                    }
                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                    CalcMethod[i]=8;
                                    gst_Test_Info[i].CalcMethod=8;
                                    qDebug()<<"结果计算方式8***";
                                }
                            }
                            else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_30)  // 吸光度 30%
                            {
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                    CalcMethod[i]=9;
                                    gst_Test_Info[i].CalcMethod=9;
                                    qDebug()<<"结果计算方式9***";
                                }
                                else{
                                    int num = 0;

                                    while((num < 20) || (num > 40))
                                    {
                                        num = qrand()%40;
                                    }
                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                    CalcMethod[i]=10;
                                    gst_Test_Info[i].CalcMethod=10;
                                    qDebug()<<"结果计算方式10***";
                                }
                            }
                            else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_20)  // // 吸光度 20%
                            {
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                    CalcMethod[i]=11;
                                    gst_Test_Info[i].CalcMethod=11;
                                    qDebug()<<"结果计算方式11***";
                                }
                                else{

                                    int num = 0;

                                    while((num < 10) || (num > 20))
                                    {
                                        num = qrand()%20;
                                    }
                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                    CalcMethod[i]=12;
                                    gst_Test_Info[i].CalcMethod=12;
                                    qDebug()<<"结果计算方式12***";
                                }
                            }
                            else if(gst_Test_Info[i].ThouData_Cur > END_MAX_STANDARD_10) // // 吸光度 10%
                            {
                                if(gst_Test_Info[i].Ad_Max_Move_Time != 0)
                                {
                                    Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                                    Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                                    CalcMethod[i]=13;
                                    gst_Test_Info[i].CalcMethod=13;
                                    qDebug()<<"结果计算方式13***";
                                }
                                else{
                                    int num = 0;

                                    while((num < 1) || (num > 10))
                                    {
                                        num = qrand()%10;
                                    }

                                    Save_Data(i, num, &gst_Test_Info[i], &st_temp);
                                    CalcMethod[i]=14;
                                    gst_Test_Info[i].CalcMethod=14;
                                    qDebug()<<"结果计算方式14***";
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
                                CalcMethod[i]=15;
                                gst_Test_Info[i].CalcMethod=15;
                                qDebug()<<"结果计算方式15***";
                            }
                        }
                    }
                }
            }
            else   //24-31 埃希氏
            {
                if(i < ECOLI_START_NUM)  // 16-23 菌落总数
                { /*  if(false==TPCTestNumberVector.contains(gst_Test_Info[i].BottleId))
                        TPCTestNumberVector.prepend(gst_Test_Info[i].BottleId);
                     for(int z=0;z<TPCTestNumberVector.size();z++)
                     qDebug()<<"TPCTestNumberVector"<<"["<<z<<"]="<<TPCTestNumberVector.at(z);*/
                     gst_Test_Info[i].TestFlag=true;
                     qDebug()<<"输入菌落总数  gst_Test_Info"<<"["<<i<<"]"<<".TestFlag="<<gst_Test_Info[i].TestFlag;
                    memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
                  //  Get_Param(TPC_COLI_ID, &st_temp);
                    switch (i%MAX_ONE_TEST_NUM) {   //定位到瓶号
                    case 0:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle1_ClassNo,Bottle1_ClassNo_1);
                        qDebug()<<"Bottle1_ClassNo-菌落总数"<<Bottle1_ClassNo;
                        qDebug()<<"Bottle1_ClassNo_1-菌落总数"<<Bottle1_ClassNo_1;
                        qDebug()<<"1号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"1号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"1号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"1号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 1:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle2_ClassNo,Bottle2_ClassNo_1);
                        qDebug()<<"Bottle2_ClassNo-菌落总数"<<Bottle2_ClassNo;
                        qDebug()<<"Bottle2_ClassNo_1-菌落总数"<<Bottle2_ClassNo_1;
                        qDebug()<<"2号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"2号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"2号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"2号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 2:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle3_ClassNo,Bottle3_ClassNo_1);
                        qDebug()<<"Bottle3_ClassNo-菌落总数"<<Bottle3_ClassNo;
                        qDebug()<<"Bottle3_ClassNo_1-菌落总数"<<Bottle3_ClassNo_1;
                        qDebug()<<"3号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"3号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"3号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"3号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 3:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle4_ClassNo,Bottle4_ClassNo_1);
                        qDebug()<<"Bottle4_ClassNo-菌落总数"<<Bottle4_ClassNo;
                        qDebug()<<"Bottle4_ClassNo_1-菌落总数"<<Bottle4_ClassNo_1;
                        qDebug()<<"4号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"4号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"4号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"4号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 4:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle5_ClassNo,Bottle5_ClassNo_1);
                        qDebug()<<"Bottle5_ClassNo-菌落总数"<<Bottle5_ClassNo;
                        qDebug()<<"Bottle5_ClassNo_1-菌落总数"<<Bottle5_ClassNo_1;
                        qDebug()<<"5号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"5号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"5号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"5号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 5:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle6_ClassNo,Bottle6_ClassNo_1);
                        qDebug()<<"Bottle6_ClassNo-菌落总数"<<Bottle6_ClassNo;
                        qDebug()<<"Bottle6_ClassNo_1-菌落总数"<<Bottle6_ClassNo_1;
                        qDebug()<<"6号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"6号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"6号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"6号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 6:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle7_ClassNo,Bottle7_ClassNo_1);
                        qDebug()<<"Bottle7_ClassNo-菌落总数"<<Bottle7_ClassNo;
                        qDebug()<<"Bottle7_ClassNo_1-菌落总数"<<Bottle7_ClassNo_1;
                        qDebug()<<"7号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"7号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"7号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"7号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 7:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle8_ClassNo,Bottle8_ClassNo_1);
                        qDebug()<<"Bottle8_ClassNo-菌落总数"<<Bottle8_ClassNo;
                        qDebug()<<"Bottle8_ClassNo_1-菌落总数"<<Bottle8_ClassNo_1;
                        qDebug()<<"8号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"8号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"8号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"8号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 8:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle9_ClassNo,Bottle9_ClassNo_1);
                        qDebug()<<"Bottle9_ClassNo-菌落总数"<<Bottle9_ClassNo;
                        qDebug()<<"Bottle9_ClassNo_1-菌落总数"<<Bottle9_ClassNo_1;
                        qDebug()<<"9号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"9号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"9号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"9号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 9:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle10_ClassNo,Bottle10_ClassNo_1);
                        qDebug()<<"Bottle10_ClassNo-菌落总数"<<Bottle10_ClassNo;
                        qDebug()<<"Bottle10_ClassNo_1-菌落总数"<<Bottle10_ClassNo_1;
                        qDebug()<<"10号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"10号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"10号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"10号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 10:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle11_ClassNo,Bottle11_ClassNo_1);
                        qDebug()<<"Bottle11_ClassNo-菌落总数"<<Bottle11_ClassNo;
                        qDebug()<<"Bottle11_ClassNo_1-菌落总数"<<Bottle11_ClassNo_1;
                        qDebug()<<"11号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"11号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"11号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"11号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 11:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle12_ClassNo,Bottle12_ClassNo_1);
                        qDebug()<<"Bottle12_ClassNo-菌落总数"<<Bottle12_ClassNo;
                        qDebug()<<"Bottle12_ClassNo_1-菌落总数"<<Bottle12_ClassNo_1;
                        qDebug()<<"12号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"12号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"12号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"12号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 12:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle13_ClassNo,Bottle13_ClassNo_1);
                        qDebug()<<"Bottle13_ClassNo-菌落总数"<<Bottle13_ClassNo;
                        qDebug()<<"Bottle13_ClassNo_1-菌落总数"<<Bottle13_ClassNo_1;
                        qDebug()<<"13号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"13号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"13号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"13号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 13:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle14_ClassNo,Bottle14_ClassNo_1);
                        qDebug()<<"Bottle14_ClassNo-菌落总数"<<Bottle14_ClassNo;
                        qDebug()<<"Bottle14_ClassNo_1-菌落总数"<<Bottle14_ClassNo_1;
                        qDebug()<<"14号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"14号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"14号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"14号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 14:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle15_ClassNo,Bottle15_ClassNo_1);
                        qDebug()<<"Bottle15_ClassNo-菌落总数"<<Bottle15_ClassNo;
                        qDebug()<<"Bottle15_ClassNo_1-菌落总数"<<Bottle15_ClassNo_1;
                        qDebug()<<"15号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"15号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"15号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"15号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 15:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle16_ClassNo,Bottle16_ClassNo_1);
                        qDebug()<<"Bottle16_ClassNo-菌落总数"<<Bottle16_ClassNo;
                        qDebug()<<"Bottle16_ClassNo_1-菌落总数"<<Bottle16_ClassNo_1;
                        qDebug()<<"16号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"16号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"16号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"16号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 16:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle17_ClassNo,Bottle17_ClassNo_1);
                        qDebug()<<"Bottle17_ClassNo-菌落总数"<<Bottle17_ClassNo;
                        qDebug()<<"Bottle17_ClassNo_1-菌落总数"<<Bottle17_ClassNo_1;
                        qDebug()<<"17号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"17号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"17号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"17号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 17:
                        Get_Param1(TPC_COLI_ID,&st_temp,Bottle18_ClassNo,Bottle18_ClassNo_1);
                        qDebug()<<"Bottle18_ClassNo-菌落总数"<<Bottle18_ClassNo;
                        qDebug()<<"Bottle18_ClassNo_1-菌落总数"<<Bottle18_ClassNo_1;
                        qDebug()<<"18号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"18号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"18号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"18号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 18:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle19_ClassNo,Bottle19_ClassNo_1);
                        qDebug()<<"Bottle19_ClassNo-菌落总数"<<Bottle19_ClassNo;
                        qDebug()<<"Bottle19_ClassNo_1-菌落总数"<<Bottle19_ClassNo_1;
                        qDebug()<<"19号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"19号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"19号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"19号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 19:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle20_ClassNo,Bottle20_ClassNo_1);
                        qDebug()<<"Bottle20_ClassNo-菌落总数"<<Bottle20_ClassNo;
                        qDebug()<<"Bottle20_ClassNo_1-菌落总数"<<Bottle20_ClassNo_1;
                        qDebug()<<"20号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"20号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"20号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"20号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 20:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle21_ClassNo,Bottle21_ClassNo_1);
                        qDebug()<<"Bottle21_ClassNo-菌落总数"<<Bottle21_ClassNo;
                        qDebug()<<"Bottle21_ClassNo_1-菌落总数"<<Bottle21_ClassNo_1;
                        qDebug()<<"21号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"21号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"21号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"21号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 21:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle22_ClassNo,Bottle22_ClassNo_1);
                        qDebug()<<"Bottle22_ClassNo-菌落总数"<<Bottle22_ClassNo;
                        qDebug()<<"Bottle22_ClassNo_1-菌落总数"<<Bottle22_ClassNo_1;
                        qDebug()<<"22号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"22号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"22号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"22号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 22:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle23_ClassNo,Bottle23_ClassNo_1);
                        qDebug()<<"Bottle23_ClassNo-菌落总数"<<Bottle23_ClassNo;
                        qDebug()<<"Bottle23_ClassNo_1-菌落总数"<<Bottle23_ClassNo_1;
                        qDebug()<<"23号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"23号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"23号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"23号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 23:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle24_ClassNo,Bottle24_ClassNo_1);
                        qDebug()<<"Bottle24_ClassNo-菌落总数"<<Bottle24_ClassNo;
                        qDebug()<<"Bottle24_ClassNo_1-菌落总数"<<Bottle24_ClassNo_1;
                        qDebug()<<"24号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"24号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"24号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"24号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    case 24:
                        Get_Param1(TPC_COLI_ID, &st_temp,Bottle25_ClassNo,Bottle25_ClassNo_1);
                        qDebug()<<"Bottle25_ClassNo-菌落总数"<<Bottle25_ClassNo;
                        qDebug()<<"Bottle25_ClassNo_1-菌落总数"<<Bottle25_ClassNo_1;
                        qDebug()<<"25号瓶 st_temp.para_A-菌落总数"<<st_temp.para_A;
                        qDebug()<<"25号瓶 st_temp.para_B-菌落总数"<<st_temp.para_B;
                        qDebug()<<"25号瓶 st_temp.para_C-菌落总数"<<st_temp.para_C;
                        qDebug()<<"25号瓶 st_temp.para_D-菌落总数"<<st_temp.para_D;
                        break;
                    default:
                        break;
                    }
                }
                else{
                  /*  if(false==ETestNumberVector.contains(gst_Test_Info[i].BottleId))
                    ETestNumberVector.prepend(gst_Test_Info[i].BottleId);
                    for(int n=0;n<ETestNumberVector.size();n++)
                    qDebug()<<"ETestNumberVector"<<"["<<n<<"]="<<ETestNumberVector.at(n);*/
                    gst_Test_Info[i].TestFlag=true;
                     qDebug()<<"输入埃希氏  gst_Test_Info"<<"["<<i<<"]"<<".TestFlag="<<gst_Test_Info[i].TestFlag;
                    memset(&st_temp, 0, sizeof(ST_PARA_SETTING));
                  //  Get_Param(E_COLI_ID, &st_temp);
                    switch (i%MAX_ONE_TEST_NUM) {   //定位到瓶号
                    case 0:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle1_ClassNo,Bottle1_ClassNo_1);
                        qDebug()<<"Bottle1_ClassNo-埃希氏"<<Bottle1_ClassNo;
                        qDebug()<<"Bottle1_ClassNo_1-埃希氏"<<Bottle1_ClassNo_1;
                        qDebug()<<"1号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"1号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"1号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"1号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 1:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle2_ClassNo,Bottle2_ClassNo_1);
                        qDebug()<<"Bottle2_ClassNo-埃希氏"<<Bottle2_ClassNo;
                        qDebug()<<"Bottle2_ClassNo_1-埃希氏"<<Bottle2_ClassNo_1;
                        qDebug()<<"2号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"2号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"2号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"2号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 2:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle3_ClassNo,Bottle3_ClassNo_1);
                        qDebug()<<"Bottle3_ClassNo-埃希氏"<<Bottle3_ClassNo;
                        qDebug()<<"Bottle3_ClassNo_1-埃希氏"<<Bottle3_ClassNo_1;
                        qDebug()<<"3号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"3号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"3号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"3号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 3:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle4_ClassNo,Bottle4_ClassNo_1);
                        qDebug()<<"Bottle4_ClassNo-埃希氏"<<Bottle4_ClassNo;
                        qDebug()<<"Bottle4_ClassNo_1-埃希氏"<<Bottle4_ClassNo_1;
                        qDebug()<<"4号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"4号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"4号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"4号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 4:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle5_ClassNo,Bottle5_ClassNo_1);
                        qDebug()<<"Bottle5_ClassNo-埃希氏"<<Bottle5_ClassNo;
                        qDebug()<<"Bottle5_ClassNo_1-埃希氏"<<Bottle5_ClassNo_1;
                        qDebug()<<"5号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"5号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"5号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"5号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 5:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle6_ClassNo,Bottle6_ClassNo_1);
                        qDebug()<<"Bottle6_ClassNo-埃希氏"<<Bottle6_ClassNo;
                        qDebug()<<"Bottle6_ClassNo_1-埃希氏"<<Bottle6_ClassNo_1;
                        qDebug()<<"6号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"6号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"6号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"6号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 6:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle7_ClassNo,Bottle7_ClassNo_1);
                        qDebug()<<"Bottle7_ClassNo-埃希氏"<<Bottle7_ClassNo;
                        qDebug()<<"Bottle7_ClassNo_1-埃希氏"<<Bottle7_ClassNo_1;
                        qDebug()<<"7号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"7号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"7号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"7号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 7:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle8_ClassNo,Bottle8_ClassNo_1);
                        qDebug()<<"Bottle8_ClassNo-埃希氏"<<Bottle8_ClassNo;
                        qDebug()<<"Bottle8_ClassNo_1-埃希氏"<<Bottle8_ClassNo_1;
                        qDebug()<<"8号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"8号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"8号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"8号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 8:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle9_ClassNo,Bottle9_ClassNo_1);
                        qDebug()<<"Bottle9_ClassNo-埃希氏"<<Bottle9_ClassNo;
                        qDebug()<<"Bottle9_ClassNo_1-埃希氏"<<Bottle9_ClassNo_1;
                        qDebug()<<"9号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"9号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"9号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"9号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 9:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle10_ClassNo,Bottle10_ClassNo_1);
                        qDebug()<<"Bottle10_ClassNo-埃希氏"<<Bottle10_ClassNo;
                        qDebug()<<"Bottle10_ClassNo_1-埃希氏"<<Bottle10_ClassNo_1;
                        qDebug()<<"10号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"10号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"10号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"10号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 10:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle11_ClassNo,Bottle11_ClassNo_1);
                        qDebug()<<"Bottle11_ClassNo-埃希氏"<<Bottle11_ClassNo;
                        qDebug()<<"Bottle11_ClassNo_1-埃希氏"<<Bottle11_ClassNo_1;
                        qDebug()<<"11号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"11号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"11号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"11号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 11:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle12_ClassNo,Bottle12_ClassNo_1);
                        qDebug()<<"Bottle12_ClassNo-埃希氏"<<Bottle12_ClassNo;
                        qDebug()<<"Bottle12_ClassNo_1-埃希氏"<<Bottle12_ClassNo_1;
                        qDebug()<<"12号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"12号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"12号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"12号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 12:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle13_ClassNo,Bottle13_ClassNo_1);
                        qDebug()<<"Bottle13_ClassNo-埃希氏"<<Bottle13_ClassNo;
                        qDebug()<<"Bottle13_ClassNo_1-埃希氏"<<Bottle13_ClassNo_1;
                        qDebug()<<"13号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"13号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"13号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"13号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 13:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle14_ClassNo,Bottle14_ClassNo_1);
                        qDebug()<<"Bottle14_ClassNo-埃希氏"<<Bottle14_ClassNo;
                        qDebug()<<"Bottle14_ClassNo_1-埃希氏"<<Bottle14_ClassNo_1;
                        qDebug()<<"14号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"14号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"14号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"14号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 14:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle15_ClassNo,Bottle15_ClassNo_1);
                        qDebug()<<"Bottle15_ClassNo-埃希氏"<<Bottle15_ClassNo;
                        qDebug()<<"Bottle15_ClassNo_1-埃希氏"<<Bottle15_ClassNo_1;
                        qDebug()<<"15号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"15号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"15号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"15号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 15:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle16_ClassNo,Bottle16_ClassNo_1);
                        qDebug()<<"Bottle16_ClassNo-埃希氏"<<Bottle16_ClassNo;
                        qDebug()<<"Bottle16_ClassNo_1-埃希氏"<<Bottle16_ClassNo_1;
                        qDebug()<<"16号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"16号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"16号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"16号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 16:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle17_ClassNo,Bottle17_ClassNo_1);
                        qDebug()<<"Bottle17_ClassNo-埃希氏"<<Bottle17_ClassNo;
                        qDebug()<<"Bottle17_ClassNo_1-埃希氏"<<Bottle17_ClassNo_1;
                        qDebug()<<"17号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"17号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"17号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"17号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 17:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle18_ClassNo,Bottle18_ClassNo_1);
                        qDebug()<<"Bottle18_ClassNo-埃希氏"<<Bottle18_ClassNo;
                        qDebug()<<"Bottle18_ClassNo_1-埃希氏"<<Bottle18_ClassNo_1;
                        qDebug()<<"18号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"18号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"18号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"18号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 18:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle19_ClassNo,Bottle19_ClassNo_1);
                        qDebug()<<"Bottle19_ClassNo-埃希氏"<<Bottle19_ClassNo;
                        qDebug()<<"Bottle19_ClassNo_1-埃希氏"<<Bottle19_ClassNo_1;
                        qDebug()<<"19号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"19号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"19号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"19号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 19:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle20_ClassNo,Bottle20_ClassNo_1);
                        qDebug()<<"Bottle20_ClassNo-埃希氏"<<Bottle20_ClassNo;
                        qDebug()<<"Bottle20_ClassNo_1-埃希氏"<<Bottle20_ClassNo_1;
                        qDebug()<<"20号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"20号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"20号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"20号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 20:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle21_ClassNo,Bottle21_ClassNo_1);
                        qDebug()<<"Bottle21_ClassNo-埃希氏"<<Bottle21_ClassNo;
                        qDebug()<<"Bottle21_ClassNo_1-埃希氏"<<Bottle21_ClassNo_1;
                        qDebug()<<"21号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"21号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"21号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"21号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 21:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle22_ClassNo,Bottle22_ClassNo_1);
                        qDebug()<<"Bottle22_ClassNo-埃希氏"<<Bottle22_ClassNo;
                        qDebug()<<"Bottle22_ClassNo_1-埃希氏"<<Bottle22_ClassNo_1;
                        qDebug()<<"22号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"22号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"22号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"22号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 22:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle23_ClassNo,Bottle23_ClassNo_1);
                        qDebug()<<"Bottle23_ClassNo-埃希氏"<<Bottle23_ClassNo;
                        qDebug()<<"Bottle23_ClassNo_1-埃希氏"<<Bottle23_ClassNo_1;
                        qDebug()<<"23号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"23号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"23号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"23号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 23:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle24_ClassNo,Bottle24_ClassNo_1);
                        qDebug()<<"Bottle24_ClassNo-埃希氏"<<Bottle24_ClassNo;
                        qDebug()<<"Bottle24_ClassNo_1-埃希氏"<<Bottle24_ClassNo_1;
                        qDebug()<<"24号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"24号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"24号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"24号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    case 24:
                        Get_Param1(E_COLI_ID, &st_temp,Bottle25_ClassNo,Bottle25_ClassNo_1);
                        qDebug()<<"Bottle25_ClassNo-埃希氏"<<Bottle25_ClassNo;
                        qDebug()<<"Bottle25_ClassNo_1-埃希氏"<<Bottle25_ClassNo_1;
                        qDebug()<<"25号瓶 st_temp.para_A-埃希氏"<<st_temp.para_A;
                        qDebug()<<"25号瓶 st_temp.para_B-埃希氏"<<st_temp.para_B;
                        qDebug()<<"25号瓶 st_temp.para_C-埃希氏"<<st_temp.para_C;
                        qDebug()<<"25号瓶 st_temp.para_D-埃希氏"<<st_temp.para_D;
                        break;
                    default:
                        break;
                    }
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
                        CalcMethod[i]=16;
                        gst_Test_Info[i].CalcMethod=16;
                        qDebug()<<"结果计算方式16***";
                    }
                    else{
                        Time_Calculate(&gst_Test_Info[i] ,&st_temp);
                        Titer_Cal(i, &gst_Test_Info[i] ,&st_temp);//浓度计算
                        CalcMethod[i]=17;
                        gst_Test_Info[i].CalcMethod=17;
                        qDebug()<<"结果计算方式17***";
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
                        gst_Test_Info[i].Test_st = TEST_ST_IDEL;  // 清除测试标志

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

                        for(j = 0; j < MPN_MAX_NUM; j++)
                        {
                            printf("<11111111>g_mpn_test_val[%d] ==== %d MPN_MAX_NUM === %d \r\n", j, g_mpn_test_val[j], MPN_MAX_NUM);
                            if(g_mpn_test_val[j] == 0){  // 测试结束
                                rtn = false;
                                printf("\r\n<22222222>");
                                break;
                            }
                            else {
                                 printf("<333333>g_mpn_test_val[%d] ==== %d   TB_FAST_TIME_LIMIT  == %d\r\n", j, g_mpn_test_val[j], TB_FAST_TIME_LIMIT);
                                if((j < 5) &&(g_mpn_test_val[j] < TB_FAST_TIME_LIMIT)){
                                    a++;
                                }
                                else if((j < 10) && (g_mpn_test_val[j] < TB_FAST_TIME_LIMIT)){
                                    b++;
                                }
                                else if(g_mpn_test_val[j] < TB_FAST_TIME_LIMIT){
                                    c++;
                                }
                                printf("\r\n <333333> a == %d b == %d c== %d \r\n", a, b, c);
                            }
                        }
                        printf("[ ]rtn ==== %d a == %d b == %d c== %d \r\n", rtn, a, b, c );
                        if(true == rtn){
                           long long temp_val = GetMpnVal(a,b, c);  //设成long long 否则溢出
                            printf(" get_val==== %d a == %d b == %d c== %d \r\n", rtn, a, b, c );
                            qDebug()<<"temp_val="<<temp_val;
                            if(g_mpn_test_st == MPN_TEST_FEC){  // 测试的耐热
                                st_temp.Unit = 0;
                                Save_Data(FECCOLI_START_NUM, temp_val, &gst_Test_Info[TPCCOLI_START_NUM], &st_temp);  // 最小值是1
                            }
                            else if(g_mpn_test_st == MPN_TEST_TOL){   // 测试的TOL
                                st_temp.Unit = 0;
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

long long testing_thread::GetMpnVal(int a, int b, int c)
{
    if(0 == a)
    {
        return 10*g_mpn_test_mui*(long long)mpn_0[b][c]; //强制转换成long long
    }
    else if(1 == a){
        return 10*g_mpn_test_mui*(long long)mpn_1[b][c];
    }
    else if(2 == a){
        return 10*g_mpn_test_mui*(long long)mpn_2[b][c];
    }
    else if(3 == a){
        return 10*g_mpn_test_mui*(long long)mpn_3[b][c];
    }
    else if(4 == a){
        return 10*g_mpn_test_mui*(long long)mpn_4[b][c];
    }
    else if(5 == a){
        return 10*g_mpn_test_mui*(long long)mpn_5[b][c];
    }
}

/*********************************************************************************
* 名称：时间校正
* 功能：
* 入口参数：
* 出口参数：无
*********************************************************************************/
void testing_thread::Time_Calculate(ST_TEST_INFO *pSt_TestInfo, ST_PARA_SETTING *pst_ParaSetting)
{   qDebug()<<"进入函数******";
    return ;
    qDebug()<<"判断函数是否运行******";
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
{   qDebug()<<"计算输入  CalibrateFlag="<<CalibrateFlag;
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
        con_data2 = 11|0x10; //|位或   11|0x10位或结果为27
    }
    else
    {
        zzz = exp(-1*log(10)*yyy);//计数浓度  exp()用来计算以e 为底的x 次方值，即ex 值，然后将结果返回

        //xxx = zzz*pst_Parasetting->para_C + pst_Parasetting->para_D;//c,d值默认为1,0，可以微调
        xxx = zzz;//c,d值默认为1,0，可以微调
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
    st_his_data.Param_time = QString::number(pSt_TestInfo->Ad_Max_Move_Time);

    qDebug()<<"st_his_data.NongDu="<<st_his_data.NongDu;
    qDebug()<<"st_his_data.test_len="<<st_his_data.test_len;
    qDebug()<<"st_his_data.BottleId="<<st_his_data.BottleId;
    qDebug()<<"st_his_data.SampleID ="<<st_his_data.SampleID;
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
    qDebug()<<"g_work_Test_type[bottleid]="<<g_work_Test_type[bottleid];
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
        if(0 == gst_His_Info[id + i].test_len )  //0-4为一组数据  5-9为一组  20-24为一组 依次类推
        {
            test_len = -1;
            break;  //退出循环  只要有一个无数据就退出循环
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
            if(gst_His_Info_temp[0].test_len  > gst_His_Info_temp[i + 1].test_len) //升序排序  0最小
            {
                memcpy(&gst_His_Info_temp_2, &gst_His_Info_temp[0], sizeof(ST_HISTORY_DATA));
                memcpy(&gst_His_Info_temp[0], &gst_His_Info_temp[i + 1], sizeof(ST_HISTORY_DATA));
                memcpy(&gst_His_Info_temp[i+1], &gst_His_Info_temp_2, sizeof(ST_HISTORY_DATA));
            }
        }
        for(i = 1; i < MAX_ONE_GROUP_NUM; i++) //降序排序  1最大
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
        for(i = 2; i < MAX_ONE_GROUP_NUM ; i++) //从2开始 去掉最高和最低值
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


    qDebug()<<"保存输入  CalibrateFlag="<<CalibrateFlag;
    if(num < FECCOLI_START_NUM)//0-25总大肠
    {
        qDebug()<<"test_len = 3333 = "<<test_len;
        gp_sql->Add_One_Data1(TOL_COLI_ID, &st_his_data,CalibrateFlag);
    }
    else if(num < TPCCOLI_START_NUM)//26-50 耐热
    {
        gp_sql->Add_One_Data1(FEC_COLI_ID, &st_his_data,CalibrateFlag);
    }
    else if(num < ECOLI_START_NUM)  // 51-75 菌落总数
    {
        gp_sql->Add_One_Data1(TPC_COLI_ID, &st_his_data,CalibrateFlag);
    }
    else   //76-100 埃希氏
    {
        gp_sql->Add_One_Data1(E_COLI_ID, &st_his_data,CalibrateFlag);
    }

#if 1 //!TEST  //测试
    if(CalibrateFlag == false) //未较标进行复制   较标时复制源与目的相同 故较标不进行复制
	{
		memcpy(&pSt_CalibrateInfo[num],pSt_TestInfo,sizeof(ST_TEST_INFO)); //注意指针传递
		pSt_CalibrateInfo[num].Test_st = TEST_ST_IDEL;  // 清除标志
		qDebug()<<"pSt_CalibrateInfo"<<"["<<num<<"]"<<".BottleId"<<pSt_CalibrateInfo[num].BottleId;
		qDebug()<<"pSt_CalibrateInfo"<<"["<<num<<"]"<<".SampleID"<<pSt_CalibrateInfo[num].SampleID;
		qDebug()<<"pSt_CalibrateInfo"<<"["<<num<<"]"<<".Ad_Max_Move_Time"<<pSt_CalibrateInfo[num].Ad_Max_Move_Time;
		qDebug()<<"pSt_CalibrateInfo"<<"["<<num<<"]"<<".TestFlag"<<pSt_CalibrateInfo[num].TestFlag;
	}
     memset(pSt_TestInfo, 0, sizeof(ST_TEST_INFO));
     pSt_TestInfo->Test_st = TEST_ST_IDEL;  // 清除标志

  #else
    pSt_TestInfo->Save_st = true;
#endif

    qDebug()<<"***********************8"<<test_len;

}
