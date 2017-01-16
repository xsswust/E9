#ifndef TESTMODE_H
#define TESTMODE_H

#include <QWidget>



namespace Ui {
class testmode;
}

class testmode : public QWidget
{
    Q_OBJECT
    
public:
    explicit testmode(QWidget *parent = 0);
    ~testmode();
    int testMode_flag;
	void go_test(int mode); // 进入测试界面

	bool exit_flag;

private slots:
	void on_pbExit_clicked();   // 界面退出
    void on_pbTolColi_clicked();
    void on_pbFecColi_clicked();
    void on_pbTPC_clicked();
    void on_pbEColi_clicked();

private:
    Ui::testmode *ui;
};
extern testmode *ptestMode;

#endif // TESTMODE_H
