#include "sql_control.h"
#include "common.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

//sql caozuo
sql_control *gp_sql;//  定义全局变量
//QSqlQuery g_query;  //sql 数据库操作

static char bottle_no[][10]={
	"一",		"二",		"三",		"四",		"五",		"六",		"七",		"八",		"九",		"十",
	"十一",		"十二",		"十三",		"十四",		"十五",		"十六",		"十七",		"十八",		"十九",		"二十",
	"二十一",	"二十二",	"二十三",	"二十四",	"二十五",	"二十六",	"二十七",	"二十八",	"二十九",	"三十",
	"三十一",	"三十二",	"三十三",	"三十四",	"三十五",	"三十六",	"三十七",	"三十八",	"三十九",	"四十",
	"四十一",	"四十二",	"四十三",	"四十四",	"四十五",	"四十六",	"四十七",	"四十八",	"四十九",	"五十",
	"五十一",	"五十二",	"五十三",	"五十四",	"五十五",	"五十六",	"五十七",	"五十八",	"五十九",	"六十",
	"六十一",	"六十二",	"六十三",	"六十四",	"六十五",	"六十六",	"六十七",	"六十八",	"六十九",	"七十",
	"七十一",	"七十二",	"七十三",	"七十四",	"七十五",	"七十六",	"七十七",	"七十八",	"七十九",	"八十",
	"八十一",	"八十二",	"八十三",	"八十四",	"八十五",	"八十六",	"八十七",	"八十八",	"八十九",	"九十",
	"九十一",	"九十二",	"九十三",	"九十四",	"九十五",	"九十六",	"九十七",	"九十八",	"九十九",	"一百"
};
sql_control::sql_control()
{
	bool flag = false;
	bool success;
   // 简立数据库
	QFile f;
	if(f.exists(FILE_HIS_DATA))
	{
		flag = true;
	}
	else
	{
		flag = false;
	}

	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setHostName("JM");          //设置数据库主机名
	db.setDatabaseName(FILE_HIS_DATA);              //设置数据库名
	db.setUserName("zbc");                //设置数据库用户名
	db.setPassword("123456");                   //设置数据库密码
	//db.open();                             		//打开连接

	//打开数据库
	if(!db.open())
	{
	   qDebug()<<db.lastError();
	   qFatal("failed to connect.") ;
	   printf(" open error \r\n");
	   //return -1;
	}
	else
	{
		printf(" open ok \r\n");
	}
//	QSqlQuery g_query;  //sql 数据库操作
	g_query = new QSqlQuery(db);
	// 建立表
	//创建数据库表
	if(false == flag)
	{
		// 建立 埃希氏表
		success = g_query->exec(QObject::tr(CMD_CREAT_COLI_TABLE).arg(STR_TEST_ECOLI)); // 建立
		if(success){
			qDebug()<<QObject::tr("数据库表创建成功！\n");
		}
		else{
			qDebug()<<QObject::tr("数据库表创建失败！\n");
		}

		// 建立 总大肠菌表
		success = g_query->exec(QObject::tr(CMD_CREAT_COLI_TABLE).arg(STR_TEST_TOLCOLI)); // 建立
		if(success){
			qDebug()<<QObject::tr("数据库表创建成功！\n");
		}
		else{
			qDebug()<<QObject::tr("数据库表创建失败！\n");
		}
		// 建立 菌落总数
		success = g_query->exec(QObject::tr(CMD_CREAT_COLI_TABLE).arg(STR_TEST_TPCCOLI)); // 建立
		if(success){
			qDebug()<<QObject::tr("数据库表创建成功！\n");
		}
		else{
			qDebug()<<QObject::tr("数据库表创建失败！\n");
		}
		// 建立 耐热菌
		success = g_query->exec(QObject::tr(CMD_CREAT_COLI_TABLE).arg(STR_TEST_FECCOLI)); // 建立
		if(success){
			qDebug()<<QObject::tr("数据库表创建成功！%1\n").arg(STR_TEST_FECCOLI);
		}
		else{
			qDebug()<<QObject::tr("数据库表创建失败！%1\n").arg(STR_TEST_FECCOLI);
		}

		// 建立 温区1
		success = g_query->exec(QObject::tr(CMD_CREAT_TEM_TABLE).arg(STR_TEMP1)); // 建立
		if(success){
			qDebug()<<QObject::tr("\n success！%1\n").arg(STR_TEMP1);
		}
		else{
			qDebug()<<QObject::tr("error %1\n").arg(STR_TEMP1);
		}
		// 建立 温区2
		success = g_query->exec(QObject::tr(CMD_CREAT_TEM_TABLE).arg(STR_TEMP2)); // 建立
		if(success){
			qDebug()<<QObject::tr("success！%1\n").arg(STR_TEMP2);
		}
		else{
			qDebug()<<QObject::tr("error%1 \n").arg(STR_TEMP2);
		}

		// 建立 温区3
		success = g_query->exec(QObject::tr(CMD_CREAT_TEM_TABLE).arg(STR_TEMP3)); // 建立
		if(success){
			qDebug()<<QObject::tr("success！%1\n").arg(STR_TEMP3);
		}
		else{
			qDebug()<<QObject::tr("error%1 \n").arg(STR_TEMP3);
		}

		// 建立 温区4
		success = g_query->exec(QObject::tr(CMD_CREAT_TEM_TABLE).arg(STR_TEMP4)); // 建立
		if(success){
			qDebug()<<QObject::tr("success！%1\n").arg(STR_TEMP4);
		}
		else{
			qDebug()<<QObject::tr("error%1 \n").arg(STR_TEMP4);
		}

		// 建立 温区5
		success = g_query->exec(QObject::tr(CMD_CREAT_TEM_TABLE).arg(STR_TEMP5)); // 建立
		if(success){
			qDebug()<<QObject::tr("success！%1\n").arg(STR_TEMP5);
		}
		else{
			qDebug()<<QObject::tr("error%1 \n").arg(STR_TEMP5);
		}

		// 建立 温区6
		success = g_query->exec(QObject::tr(CMD_CREAT_TEM_TABLE).arg(STR_TEMP6)); // 建立
		if(success){
			qDebug()<<QObject::tr("success！%1\n").arg(STR_TEMP6);
		}
		else{
			qDebug()<<QObject::tr("error%1 \n").arg(STR_TEMP6);
		}
		// 建立 记录
		for(int i = 0; i < MAX_TEST_NUM; i++)
		{

			success = g_query->exec(QObject::tr(CMD_CREAT_ID_TABLE).arg(bottle_no[i])); // 建立
			if(success){
				qDebug()<<QObject::tr("success！%1\n").arg(bottle_no[i]);
			}
			else{
				qDebug()<<QObject::tr("error%1 \n").arg(bottle_no[i]);
			}
		}

	}
}
#if 0
// 析构函数
sql_control::~sql_control()
{

}
#endif

//查询最大id
unsigned int sql_control::Get_Max_Id(QSqlQuery *sql_query, int mode)
{
	int max_id = 0;

	switch(mode)
	{
		case TOL_COLI_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEST_TOLCOLI));
			break;

		case FEC_COLI_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEST_FECCOLI));
			break;

		case TPC_COLI_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEST_TPCCOLI));
			break;

		case E_COLI_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEST_ECOLI));
			break;

		case TEMP1_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEMP1));
			break;

		case TEMP2_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEMP2));
			break;

		case TEMP3_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEMP3));
			break;

		case TEMP4_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEMP4));
			break;

		case TEMP5_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEMP5));
			break;

		case TEMP6_ID:
			sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(STR_TEMP6));
			break;

		default:
			break;
	}

	if(!sql_query->exec())
	{
		qDebug()<<sql_query->lastError();
	}
	else
	{
		while(sql_query->next())
		{
			max_id = sql_query->value(0).toInt();
			//qDebug()<<QString("max id:%1").arg(max_id);
		}
	}
	return max_id;
}

// 增加一条记录
bool sql_control::Add_One_Data(int mode, ST_HISTORY_DATA *pst_his_data)
{
	unsigned int max_id;

    qDebug()<<"pst_his_data->SampleID="<<pst_his_data->SampleID;
    qDebug()<<"pst_his_data->NongDu="<<pst_his_data->NongDu;
    qDebug()<<"pst_his_data->BottleId="<<pst_his_data->BottleId;
    qDebug()<<"pst_his_data->Param_time="<<pst_his_data->Param_time;
	switch(mode)
	{
		case TOL_COLI_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			qDebug()<<QString(CMD_INSERT_ONE_DATA).arg(STR_TEST_TOLCOLI);
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA).arg(STR_TEST_TOLCOLI));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);
			g_query->addBindValue(pst_his_data->Unit);
			g_query->addBindValue(pst_his_data->Time);
			g_query->addBindValue(pst_his_data->BottleId);
			g_query->addBindValue(pst_his_data->Param_time);
            qDebug()<<"历史数据输入  CalibrateFlag="<<CalibrateFlag;
            if(true == CalibrateFlag)
             {    g_query->addBindValue("修正后"); }
            else
             {   g_query->addBindValue("修正前"); }
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted TOL_COLI_ID!";
			}

			break;

		case FEC_COLI_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA).arg(STR_TEST_FECCOLI));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);
			g_query->addBindValue(pst_his_data->Unit);
			g_query->addBindValue(pst_his_data->Time);
			g_query->addBindValue(pst_his_data->BottleId);
			g_query->addBindValue(pst_his_data->Param_time);
            qDebug()<<"历史数据输入  CalibrateFlag="<<CalibrateFlag;
            if(true == CalibrateFlag)
             {    g_query->addBindValue("修正后"); }
            else
             {   g_query->addBindValue("修正前"); }
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted! FEC_COLI_ID";
			}
			break;

		case TPC_COLI_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA).arg(STR_TEST_TPCCOLI));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);
			g_query->addBindValue(pst_his_data->Unit);
			g_query->addBindValue(pst_his_data->Time);
			g_query->addBindValue(pst_his_data->BottleId);
			g_query->addBindValue(pst_his_data->Param_time);
            qDebug()<<"历史数据输入  CalibrateFlag="<<CalibrateFlag;
            if(true == CalibrateFlag)
             {    g_query->addBindValue("修正后"); }
            else
             {   g_query->addBindValue("修正前"); }
            if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted TPC_COLI_ID!";
			}
			break;

		case E_COLI_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA).arg(STR_TEST_ECOLI));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);
			g_query->addBindValue(pst_his_data->Unit);
			g_query->addBindValue(pst_his_data->Time);
			g_query->addBindValue(pst_his_data->BottleId);
			g_query->addBindValue(pst_his_data->Param_time);
            qDebug()<<"历史数据输入  CalibrateFlag="<<CalibrateFlag;
            if(true == CalibrateFlag)
             {    g_query->addBindValue("修正后"); }
            else
             {   g_query->addBindValue("修正前"); }
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted E_COLI_ID!";
			}
			break;

		case TEMP1_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA_TEMP).arg(STR_TEMP1));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"inserted TEMP1_ID! \r\n";
			}
			break;

		case TEMP2_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA_TEMP).arg(STR_TEMP2));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"inserted TEMP2_ID! \r\n";
			}
			break;

		case TEMP3_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA_TEMP).arg(STR_TEMP3));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"inserted TEMP3_ID! \r\n";
			}
			break;
		case TEMP4_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA_TEMP).arg(STR_TEMP4));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"inserted TEMP4_ID!\r\n";
			}
			break;

		case TEMP5_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA_TEMP).arg(STR_TEMP5));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"inserted TEMP4_ID!\r\n";
			}
			break;

		case TEMP6_ID:
			max_id = Get_Max_Id(g_query, mode);
			if(max_id > MAX_HIS_ID)
			{
				Del_All_Data(mode);
				max_id = 0;
			}
			g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA_TEMP).arg(STR_TEMP6));
			g_query->addBindValue(max_id+1);
			g_query->addBindValue(pst_his_data->SampleID);
			g_query->addBindValue(pst_his_data->NongDu);

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"inserted TEMP4_ID!\r\n";
			}
			break;

	default:
			break;
	}

	return true;
}
// 增加一条记录
bool sql_control::Add_One_Data1(int mode, ST_HISTORY_DATA *pst_his_data,bool CalibrateFlag)
{
    unsigned int max_id;

    qDebug()<<"pst_his_data->SampleID="<<pst_his_data->SampleID;
    qDebug()<<"pst_his_data->NongDu="<<pst_his_data->NongDu;
    qDebug()<<"pst_his_data->BottleId="<<pst_his_data->BottleId;
    qDebug()<<"pst_his_data->Param_time="<<pst_his_data->Param_time;
    switch(mode)
    {
        case TOL_COLI_ID:
            max_id = Get_Max_Id(g_query, mode);
            if(max_id > MAX_HIS_ID)
            {
                Del_All_Data(mode);
                max_id = 0;
            }
            qDebug()<<QString(CMD_INSERT_ONE_DATA).arg(STR_TEST_TOLCOLI);
            g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA).arg(STR_TEST_TOLCOLI));
            g_query->addBindValue(max_id+1);
            g_query->addBindValue(pst_his_data->SampleID);
            g_query->addBindValue(pst_his_data->NongDu);
            g_query->addBindValue(pst_his_data->Unit);
            g_query->addBindValue(pst_his_data->Time);
            g_query->addBindValue(pst_his_data->BottleId);
            g_query->addBindValue(pst_his_data->Param_time);
            qDebug()<<"历史数据输入  CalibrateFlag="<<CalibrateFlag;
            if(true == CalibrateFlag)
             {    g_query->addBindValue("修正后"); }
            else
             {   g_query->addBindValue("修正前"); }
            if(!g_query->exec()){
                qDebug()<<g_query->lastError();
                return false;
            }
            else{
                qDebug()<<"inserted TOL_COLI_ID!";
            }

            break;

        case FEC_COLI_ID:
            max_id = Get_Max_Id(g_query, mode);
            if(max_id > MAX_HIS_ID)
            {
                Del_All_Data(mode);
                max_id = 0;
            }
            g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA).arg(STR_TEST_FECCOLI));
            g_query->addBindValue(max_id+1);
            g_query->addBindValue(pst_his_data->SampleID);
            g_query->addBindValue(pst_his_data->NongDu);
            g_query->addBindValue(pst_his_data->Unit);
            g_query->addBindValue(pst_his_data->Time);
            g_query->addBindValue(pst_his_data->BottleId);
            g_query->addBindValue(pst_his_data->Param_time);
            qDebug()<<"历史数据输入  CalibrateFlag="<<CalibrateFlag;
            if(true == CalibrateFlag)
             {    g_query->addBindValue("修正后"); }
            else
             {   g_query->addBindValue("修正前"); }
            if(!g_query->exec()){
                qDebug()<<g_query->lastError();
                return false;
            }
            else{
                qDebug()<<"inserted! FEC_COLI_ID";
            }
            break;

        case TPC_COLI_ID:
            max_id = Get_Max_Id(g_query, mode);
            if(max_id > MAX_HIS_ID)
            {
                Del_All_Data(mode);
                max_id = 0;
            }
            g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA).arg(STR_TEST_TPCCOLI));
            g_query->addBindValue(max_id+1);
            g_query->addBindValue(pst_his_data->SampleID);
            g_query->addBindValue(pst_his_data->NongDu);
            g_query->addBindValue(pst_his_data->Unit);
            g_query->addBindValue(pst_his_data->Time);
            g_query->addBindValue(pst_his_data->BottleId);
            g_query->addBindValue(pst_his_data->Param_time);
            qDebug()<<"历史数据输入  CalibrateFlag="<<CalibrateFlag;
            if(true == CalibrateFlag)
             {    g_query->addBindValue("修正后"); }
            else
             {   g_query->addBindValue("修正前"); }
            if(!g_query->exec()){
                qDebug()<<g_query->lastError();
                return false;
            }
            else{
                qDebug()<<"inserted TPC_COLI_ID!";
            }
            break;

        case E_COLI_ID:
            max_id = Get_Max_Id(g_query, mode);
            if(max_id > MAX_HIS_ID)
            {
                Del_All_Data(mode);
                max_id = 0;
            }
            g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA).arg(STR_TEST_ECOLI));
            g_query->addBindValue(max_id+1);
            g_query->addBindValue(pst_his_data->SampleID);
            g_query->addBindValue(pst_his_data->NongDu);
            g_query->addBindValue(pst_his_data->Unit);
            g_query->addBindValue(pst_his_data->Time);
            g_query->addBindValue(pst_his_data->BottleId);
            g_query->addBindValue(pst_his_data->Param_time);
            qDebug()<<"历史数据输入  CalibrateFlag="<<CalibrateFlag;
            if(true == CalibrateFlag)
             {    g_query->addBindValue("修正后"); }
            else
             {   g_query->addBindValue("修正前"); }
            if(!g_query->exec()){
                qDebug()<<g_query->lastError();
                return false;
            }
            else{
                qDebug()<<"inserted E_COLI_ID!";
            }
            break;
    default:
            break;
    }

    return true;
}
// 删除一个表的记录
bool sql_control::Del_All_Data(int mode)
{
	switch(mode)
	{
		case TOL_COLI_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEST_TOLCOLI));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear TOL_COLI_ID!";
			}

			break;

		case FEC_COLI_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEST_FECCOLI));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear FEC_COLI_ID!";
			}
			break;

		case TPC_COLI_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEST_TPCCOLI));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear TPC_COLI_ID!";
			}
			break;

		case E_COLI_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEST_ECOLI));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear E_COLI_ID!";
			}
			break;

		case TEMP1_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEMP1));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear TEMP1_ID!";
			}
			break;

		case TEMP2_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEMP2));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case TEMP3_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEMP3));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear TEMP3_ID!";
			}
			break;

		case TEMP4_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEMP4));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear TEMP4_ID!";
			}
			break;

		case TEMP5_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEMP5));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear TEMP5_ID!";
			}
			break;

		case TEMP6_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(STR_TEMP6));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"clear TEMP6_ID!";
			}
			break;
		default:
			break;
	}
	return true;
}


// 删除一个表的记录
bool sql_control::Del_All_Data_for_ad(int num)
{

#if 1
	g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg(bottle_no[num]));
	if(!g_query->exec()){
		qDebug()<<g_query->lastError();
		return false;
	}
	else{
		//qDebug()<<"clear TOL_COLI_ID!";
	}
#else
	num = num +1;
	switch(num)
	{
		case 1:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("一"));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TOL_COLI_ID!";
			}

			break;

		case 2:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear FEC_COLI_ID!";
			}
			break;

		case 3:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("三"));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TPC_COLI_ID!";
			}
			break;

		case 4:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("四"));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear E_COLI_ID!";
			}
			break;

		case 5:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("五"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP1_ID!";
			}
			break;

		case 6:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("六"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 7:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("七"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 8:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("八"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 9:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("九"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 10:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;
		case 11:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十一"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;
		case 12:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十二"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 13:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十三"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 14:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十四"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 15:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十五"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 16:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十六"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 17:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十七"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 18:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十八"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 19:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("十九"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 20:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 21:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十一"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 22:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十二"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 23:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十三"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 24:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十四"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 25:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十五"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 26:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十六"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 27:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十七"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 28:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十八"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 29:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("二十九"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 30:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("三十"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 31:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("三十一"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;

		case 32:
			g_query->prepare(QObject::tr(CMD_DELETE_ALL_DATA).arg("三十二"));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				//qDebug()<<"clear TEMP2_ID!";
			}
			break;



		default:
			break;
	}
#endif
	return true;
}

// 删除表的一条记录
bool sql_control::Del_One_Data(int mode, int id)
{
	switch(mode)
	{
		case TOL_COLI_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ONE_DATA).arg(STR_TEST_TOLCOLI).arg(id));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"del one TOL_COLI_ID!";
			}

			break;

		case FEC_COLI_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ONE_DATA).arg(STR_TEST_FECCOLI).arg(id));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"del one FEC_COLI_ID!";
			}
			break;

		case TPC_COLI_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ONE_DATA).arg(STR_TEST_TPCCOLI).arg(id));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"del one TPC_COLI_ID!";
			}
			break;

		case E_COLI_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ONE_DATA).arg(STR_TEST_ECOLI).arg(id));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"del one E_COLI_ID!";
			}
			break;

		case TEMP1_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ONE_DATA).arg(STR_TEMP1).arg(id));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"del one TEMP1_ID!";
			}
			break;

		case TEMP2_ID:
			g_query->prepare(QObject::tr(CMD_DELETE_ONE_DATA).arg(STR_TEMP2).arg(id));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"del one TEMP2_ID!";
			}
			break;
		default:
			break;
	}
	return true;
}

// 通过id获取一条数据
bool sql_control::Get_One_Data(int mode, ST_HISTORY_DATA *pst_his_data, int id)	// 获取一条数据
{
	switch(mode)
	{
		case TOL_COLI_ID:
			g_query->prepare(QObject::tr(CMD_GET_ONE_DATA).arg(STR_TEST_TOLCOLI).arg(id));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
//			else{
//				qDebug()<<"get ok!";
//			}
			while(g_query->next())
			{
				pst_his_data->SampleID = g_query->value(1).toString();//我想把表里的第二个值转换成字符串赋给text
				pst_his_data->NongDu = g_query->value(2).toString();//我想把表里的第二个值转换成字符串赋给text
				pst_his_data->Unit = g_query->value(3).toString();//我想把表里的第二个值转换成字符串赋给text
				pst_his_data->Time = g_query->value(4).toString();//我想把表里的第二个值转换成字符串赋给text
				pst_his_data->BottleId = g_query->value(5).toString();//我想把表里的第二个值转换成字符串赋给text
			}
			break;

		case FEC_COLI_ID:
			g_query->prepare(QObject::tr(CMD_GET_ONE_DATA).arg(STR_TEST_TOLCOLI).arg(id));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted!";
			}
			break;

		case TPC_COLI_ID:
			g_query->prepare(QObject::tr(CMD_GET_ONE_DATA).arg(STR_TEST_TOLCOLI).arg(id));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted!";
			}
			break;

		case E_COLI_ID:
			g_query->prepare(QObject::tr(CMD_GET_ONE_DATA).arg(STR_TEST_ECOLI).arg(id));
			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted!";
			}
			break;

		case TEMP1_ID:
			g_query->prepare(QObject::tr(CMD_GET_ONE_DATA).arg(STR_TEMP1).arg(id));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted!";
			}
			break;

		case TEMP2_ID:
			g_query->prepare(QObject::tr(CMD_GET_ONE_DATA).arg(STR_TEMP2).arg(id));

			if(!g_query->exec()){
				qDebug()<<g_query->lastError();
				return false;
			}
			else{
				qDebug()<<"inserted!";
			}
			break;

		default:
			break;
	}
	return true;

}

// 增加一条记录
bool sql_control::Add_One_Data_forTest(int id, ST_HISTORY_DATA1 *pst_his_data)
{
	unsigned int max_id;


	if(id < 0 || id >= MAX_TEST_NUM )
	{
		return false;
	}
/*#if !DEBUD_TEST //HARDWARE_V101
    return  true;
#endif*/
	max_id = Get_Max_Id_for_test(g_query, id);
	//qDebug()<<QString(CMD_INSERT_ONE_DATA_TEMP).arg(bottle_no[id]);
	g_query->prepare(QObject::tr(CMD_INSERT_ONE_DATA_Test).arg(bottle_no[id]));

	g_query->addBindValue(max_id+1);
	g_query->addBindValue(pst_his_data->SampleID);
//	g_query->addBindValue(pst_his_data->NongDu);
//	g_query->addBindValue(pst_his_data->Unit);

    g_query->addBindValue(pst_his_data->Ad_DarkData_Begin);
    g_query->addBindValue(pst_his_data->Ad_DarkData_Cur);
    g_query->addBindValue(pst_his_data->Ad_LightData_Begin);
    g_query->addBindValue(pst_his_data->Ad_LightData_Cur);
    g_query->addBindValue(pst_his_data->Ad_LightData_Last);
    g_query->addBindValue(pst_his_data->Ad_Max_Move_Data);
    g_query->addBindValue(pst_his_data->Ad_Max_Move_Time);
    g_query->addBindValue(pst_his_data->Last_ThouData);
    g_query->addBindValue(pst_his_data->ThouData_Cur);
    g_query->addBindValue(pst_his_data->ThouData_dis);
    g_query->addBindValue(pst_his_data->sThouData_Cnt);
    qDebug()<<"单条历史数据插入成功";

	if(!g_query->exec()){
		qDebug()<<g_query->lastError();
		return false;
	}
	else{
		//qDebug()<<"inserted TOL_COLI_ID!";
	}
	return true;
}


//查询最大id
unsigned int sql_control::Get_Max_Id_for_test(QSqlQuery *sql_query, int id)
{
	int max_id = 0;

	sql_query->prepare(QObject::tr(CMD_GET_MAX_ID).arg(bottle_no[id]));

	if(!sql_query->exec())
	{
		qDebug()<<sql_query->lastError();
	}
	else
	{
		while(sql_query->next())
		{
			max_id = sql_query->value(0).toInt();
			//qDebug()<<QString("max id:%1").arg(max_id);
		}
	}
	return max_id;
}
