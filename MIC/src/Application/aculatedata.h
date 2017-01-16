#ifndef ACULATEDATA_H
#define ACULATEDATA_H

#include <QWidget>
#include <QTimer>
#include <QString>

namespace Ui {
class AculateData;
}

class AculateData : public QWidget
{
    Q_OBJECT
    
public:
    explicit AculateData(QWidget *parent = 0);
    ~AculateData();
    void WriteIniPara();
    void ReadCurrentAcurate();
    void ReadCurrentStatus();
    void saveCurrentAcurate();
	void UnInit();
	void Init();


	void ShowStat(void);
    void showTransmission();  //
	bool exit_flag;
    bool st_flag;         // 显示状态 标志

public slots:
    void ReademitAcurate();
	void timeUpdate();		 // 时间更新显


private slots:
    void on_pbExit_clicked();
    void pb_change_st();		 // 更新状态

private:
    Ui::AculateData *ui;

	QTimer *ptimerUpdateStat;
};

#endif // ACULATEDATA_H
