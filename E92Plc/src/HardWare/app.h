#ifndef APP_H
#define APP_H
#include <QString>
#include <QTableWidget>


/*********************文件保存路径*****************************/
#define WARNING_MESSAGE_FILE "/home/JMS_SpecHMAnaly_Config.ini" //报警信息文件
#define HISTORY_DATA_FILE    "/opt/PDA/app/datas/HisDatalist.data" //历史数据文件
#define USER_INFORMATION_FILE "" //用户信息文件
#define SYSTERM_CONFIG_INFORMATION_FILE "" //系统配置信息文件
/*************************************************************/


/***********************系统配置信息文件中配置信息名称****************************/
#define KEY_PARAMETER "KeyParameter"//主要参数
#define CATHODE_CURRENT_LAMP "CathodeCurrentLamp"                   //主阴极灯电流
#define AUX_CATHODE_CURRENT_LAMP "AuxCathodeCurrentLamp"            //副阴极灯电流
#define PMT_VOLTAGE "PMTVoltage"                                    //倍增管副高压
#define DBD_VOLTAGE "DBDVoltage"                                    //原子化器高压

#define FLOW_CONTROL_PARAMETER "FlowControlParamer"                 //流速控制参数
#define PUMP_SPEED "PumpSpeed"                                      //蠕动泵流速
#define PREPARE_1ST_TIME "Prepare1stTime"                           //准备1st时间
#define PREPARE_2ND_TIME "Prepare2ndTime"                           //准备2nd时间
#define MEASURE_1ST_TIME "Measure1stTime"                           //测量1st时间
#define MEASURE_2ND_TIME "Measure2ndTime"                           //测量2nd时间
#define MEASURE_3RD_TIME "Measure3rdTime"                           //测量3rd时间
#define CARRIER_GAS_VELOCITY "CarrierGasVelocity"                   //载气流速

#define SIGNAL_DIAGRAM_PARAMETER "SignalDialogParameter"            //信号谱图参数
#define END_TIME "EndTime"                                          //结束时间
#define SAMPLE_FREQUENCY "SampleFrequency"                          //频率
#define ABSCISSA_MIN "AbscissaMin"                                  //横轴最小值
#define ABSCISSA_MAX "AbscissaMax"                                  //横轴最大值
#define VERTICAL_MIN "VerticalMin"                                  //纵轴最小值
#define VERTICAL_MAX "VerticalMax"                                  //纵轴最大值

#define CALIBRATE_PARAMETER "calibrateParameter"                    //校准参数
#define SLOPE "Slope"                                                //斜率
#define INTERCEPT "Intercept"                                       //截距
#define COEFFICIENT "Coefficient"                                   //修正因数
#define CORRELATION "Correlation"                                   //线性相关度


#define OTHER_PARAMETER "OtherParameter"                            //其他参数


#define AUTO_MEASURE_INTERMISSION "autoMeasureIntermission"         //自动测量间隔时间
#define AUTO_MEASURE_INTERMISSION_HOUR "autoMeasureIntermissionHour"
#define AUTO_MEASURE_INTERMISSION_MINUTE "autoMeasureIntermissionMinute"

/*************************************************************/


/*******************************用户信息文件信息名称***********************************/
#define USERNAME_AND_PASSWORD "userNameAndPassword"
#define USERNAME "userName"
#define PASSWORD "password"
#define COMMUNICATION_SETTINGS "communicationSettings"
#define SERVER_IP "serverIP"
#define SERVER_PORT_ID "serverPortID"
/***********************************************************************************/

#define BACKGROUND_IMAGES "/opt/PDA/app/images/background.jpg"


extern bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
extern bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);

extern void saveWarningMessage(QString FileNameErr,QString object, QString waringType, QString warningEvent, QString currentValue, QString marginValue);
extern void showWarningMessage(QString FileNameErr,QTableWidget *tableWarningandLog);

extern void saveCODWarningMessage(QString FileNameErr,QString warningEvent);

extern void saveHistoryData(QString FileNameErr,QString DateTime,QString CODValue,QString detectValue,QString referVale,QString blankValue,QString absorbValue,QString dataStatus,QString initialT,QString degestinT,QString detectT);
extern void showHistoryData( QString FileNameErr,QTableWidget *tableWarningandLog);
//extern void saveCurveDataForDis(QString FileNameErr,QString Density);

extern void saveColiHistoryData(QString FileNameErr,QString SampleNo,QString Density,QString Unit,QString DateTime,QString testQual,QString bottleNo);
extern void saveTempHistoryData(QString FileNameErr,QString Temp);

extern void clearHistoryData(QString FileNameErr);

extern int setSysTime(char *t_string);

extern char* QstringTostring(QString qs);
extern QString stringToQstring(char* str);

extern float Coefficient0Caculate(float detectedValue,float referedValue);
extern float Coefficient1Caculate(float detectedValue,float referedValue,float density);
extern float AbsorbanceValue(float detectedValue,float referedValue);
extern float DensityCaculate(float detectedValue,float referedValue,float K1,float b,float medium);

extern float ParaCaculate(float x1,float x2,float x3,float x4, float y1, float y2, float y3, float y4);

#endif // APP_H
