#ifndef HISDATADIS_H
#define HISDATADIS_H

#include <QWidget>

namespace Ui {
class HisDataDis;
}

class HisDataDis : public QWidget
{
    Q_OBJECT
    
public:
    explicit HisDataDis(QWidget *parent = 0);
    ~HisDataDis();
	void DisPlayDensity();  // 显示数据
    int hisDataMode_flag;
	void HisDataDis_init();    // 界面初始化
	bool exit_flag;
private slots:
	void on_pushCleanButton_clicked();
	void on_pushExitButton_clicked();

private:
    Ui::HisDataDis *ui;
};

#endif // HISDATADIS_H
