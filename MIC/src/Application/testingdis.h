#ifndef TESTINGDIS_H
#define TESTINGDIS_H

#include <QWidget>

namespace Ui {
class TestingDis;
}

class TestingDis : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestingDis(QWidget *parent = 0);
    ~TestingDis();
    int testNo;
	void TestDis_Init();   // 初始化 开始
	void TestDis_Uninit();   // 初始化销毁 开始
	bool exit_flag;

private slots:
    void receiveIniVal();
    void receiveOverVal();

private slots:
    void on_pbExit_clicked();

    void on_pbStop_clicked();

    void on_pb1_clicked();

    void on_pb2_clicked();

    void on_pb3_clicked();

    void on_pb4_clicked();

    void on_pbIniVal_clicked();

    void on_pbOverVal_clicked();

    void on_pbFinished_clicked();

private:
    Ui::TestingDis *ui;
};

#endif // TESTINGDIS_H
