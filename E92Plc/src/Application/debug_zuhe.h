#ifndef DEBUG_ZUHE_H
#define DEBUG_ZUHE_H

#include <QWidget>
#include "keyboard_dependence.h"

// 默认检测号
#define TESTID_DEFAULT		1

// 默认瓶号
#define BOTTOMID_DEFAULT		1


#define MODE_SETUP_1		1
#define MODE_SETUP_2		2
#define MODE_SETUP_3		3
#define MODE_SETUP_4		4
#define MODE_SETUP_5		5
#define MODE_SETUP_6		6
#define MODE_SETUP_7		7
#define MODE_SETUP_8		8
#define MODE_SETUP_9		9
#define MODE_SETUP_10		10
#define MODE_SETUP_11		11
#define MODE_SETUP_12		12
#define MODE_SETUP_13		13
#define MODE_SETUP_14		14
#define MODE_SETUP_15		15
#define MODE_SETUP_16		16
#define MODE_SETUP_17		17
#define MODE_SETUP_18		18
#define MODE_SETUP_19		19
#define MODE_SETUP_20		20
#define MODE_SETUP_21		21
#define MODE_SETUP_22		22
#define MODE_SETUP_23		23
#define MODE_SETUP_24		24
#define MODE_SETUP_25		25
#define MODE_SETUP_26		26
#define MODE_SETUP_27		27
#define MODE_SETUP_28		28
#define MODE_SETUP_29		29

#define MODE_SETUP_30		30
#define MODE_SETUP_31		31
#define MODE_SETUP_32		32
#define MODE_SETUP_33		33
#define MODE_SETUP_34		34
#define MODE_SETUP_35		35
#define MODE_SETUP_36		36
#define MODE_SETUP_37		37
#define MODE_SETUP_38		38
#define MODE_SETUP_39		39

#define MODE_SETUP_40		40
#define MODE_SETUP_41		41
#define MODE_SETUP_42		42
#define MODE_SETUP_43		43
#define MODE_SETUP_44		44
#define MODE_SETUP_45		45
#define MODE_SETUP_46		46
#define MODE_SETUP_47		47
#define MODE_SETUP_48		48
#define MODE_SETUP_49		49

#define MODE_SETUP_50		50
#define MODE_SETUP_51		51
#define MODE_SETUP_52		52
#define MODE_SETUP_53		53
#define MODE_SETUP_54		54
#define MODE_SETUP_55		55
#define MODE_SETUP_56		56
#define MODE_SETUP_57		57
#define MODE_SETUP_58		58
#define MODE_SETUP_59		59

#define MODE_SETUP_60		60
#define MODE_SETUP_61		61
#define MODE_SETUP_62		62
#define MODE_SETUP_63		63
#define MODE_SETUP_64		64
#define MODE_SETUP_65		65
#define MODE_SETUP_66		66
#define MODE_SETUP_67		67
#define MODE_SETUP_68		68




#define TIME_CLEAN_ZAILIUHUAN		25  // 消毒液进载流环的时间
#define TIME_CLEAN_10		10  // 混合瓶到 载流环

#define TIME_CLEAN_JIANCECHI		100  // 消毒液进检测池的时间


#define TIME_XIAODU		(10*60)   // 10分钟  消毒液浸泡时间



namespace Ui {
class debug_zuhe;
}

class debug_zuhe : public QWidget
{
    Q_OBJECT
    
public:
	explicit debug_zuhe(QWidget *parent = 0);
	~debug_zuhe();
    int testMode_flag;
	// 初始化界面需要的一些值
	void initPainter();
	void dlg2fy();
	void time_end();
	void wjs2zlh();
	void zlh2dlg();
	void xdy2zlh();
	//void xdy2dlg();
	//void zlh2dlg();

	void clean_guanlu();  // 管路清洗
	void kq2zlh();

	void Jcc2Fy(int id);  // 检测池到废液
	void JianceshiClean();  //检测池清洗
	void Wjs2Jcc(int id);	//  无菌水到检测池
	void Xdy2Jcc(int id);	// 消毒液到检测室
	void AllClean();
	void Hhp2Zlh();		// 混合瓶 到 载流环

	bool exit_flag;

private slots:
	void timeOut();		 // 时间更新显
	void on_pbCybjy_released();

	void on_pbSydjcc_released();

	void on_pbJccdsy_released();

	void on_pbStopAll_released();

	void on_pbDlgdjcc_released();

	void on_pbZlhdsy_released();

	void on_pbKqdxg_released();

	void on_pbZlhddlg_released();

	void on_pbXdydjcc_released();

	void on_pbWjsdjcc_released();

	void on_pbWjsdzlh_released();

	void on_pbXgddlg_released();

	void on_pbJccdfy_released();

	void on_pbXdydzlh_released();

	void on_pbGlqx_released();

	void on_pbJccqx_released();

	void on_pbQbqx_released();

	void on_pbExit_released();

public slots:
   //void loginAndcancel();
   void InsertValue(QString nustr);
   void NumberBack();


private:
	Ui::debug_zuhe *ui;
	// 定义定时器
	QTimer *time_100ms;
	// 键盘输入
	Keyboard_dependence* im;
};
extern debug_zuhe *pdebug_zuhe;

#endif // TESTMODE_H
