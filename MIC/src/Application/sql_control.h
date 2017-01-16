#ifndef SQL_CONTROL_H
#define SQL_CONTROL_H
#include <QSqlQuery>
#include "common.h"
// 建立 菌表
// %1 是表名
#define CMD_CREAT_COLI_TABLE "create table %1(id int primary key,\
										样本号 varchar,\
										浓度 varchar,\
										单位 varchar,\
										时间 varchar,\
										瓶号 varchar,\
                                        检测因子 varchar,\
                                        修正 varchar\
										)"

// %1 是表名
#define CMD_CREAT_TEM_TABLE "create table %1(id int primary key,\
										时间 varchar,\
										温度 varchar\
										)"
// 建立 温度表

/*#define CMD_CREAT_ID_TABLE "create table %1(id int primary key,\
										时间 varchar,\
										透过率 varchar,\
										ad varchar\
                                        )"*/

#define CMD_CREAT_ID_TABLE "create table %1(id int primary key,\
                                        时间 varchar,\
                                        开始暗值 varchar,\
                                        当前暗值 varchar,\
                                        开始亮值 varchar,\
                                        当前亮值AD值 varchar,\
                                        上一次亮值 varchar,\
                                        最大变化值 varchar,\
                                        最大变化时间 varchar,\
                                        上一次变化率 varchar,\
                                        吸光度 varchar,\
                                        透过率显示 varchar,\
                                        检测时间 varchar\
                                        )"
// %1 是表名
#define CMD_GET_MAX_ID "select max(id) from %1"   // %1 是表名

// 插入一条信息
#define CMD_INSERT_ONE_DATA			"insert into %1 values(?,?,?,?,?,?,?,?)"   // %1 是表名
#define CMD_INSERT_ONE_DATA_TEMP	"insert into %1 values(?,?,?)"   // %1 是表名
#define CMD_INSERT_ONE_DATA_Test	"insert into %1 values(?,?,?,?,?,?,?,?,?,?,?,?,?)"   // %1 是表名
// 删除 数据
#define CMD_DELETE_ONE_DATA		"delete from %1 where id = %2"  // 删除一条数据
#define CMD_DELETE_ALL_DATA		"delete from %1"          // 删除所有数据

#define CMD_GET_ONE_DATA		"select *from %1 where id=%2"  // 获取表中的一条数据

class sql_control
{
public:
	sql_control();		// 构造函数
//	~sql_control();
	QSqlQuery *g_query;  //sql 数据库操作
	QSqlDatabase db;

	unsigned int Get_Max_Id(QSqlQuery *sql_query, int mode);  // 获取最大ID
    bool Add_One_Data(int mode, ST_HISTORY_DATA *pst_his_data);	// 增加数据
    bool Add_One_Data1(int mode, ST_HISTORY_DATA *pst_his_data,bool CalibrateFlag);
	bool Del_One_Data(int mode,int id);	// 删除一条数据
	bool Del_All_Data(int mode);	// 清空数据
	bool Get_One_Data(int mode, ST_HISTORY_DATA *pst_his_data, int id);	// 获取一条数据
	//bool

	unsigned int Get_Max_Id_for_test(QSqlQuery *sql_query, int id);
//	bool Add_One_Data_forTest(int id, ST_HISTORY_DATA *pst_his_data);
    bool Add_One_Data_forTest(int id, ST_HISTORY_DATA1 *pst_his_data);
	bool Del_All_Data_for_ad(int num);
};

extern sql_control *gp_sql;//  定义全局变量
#endif // SQL_CONTROL_H
