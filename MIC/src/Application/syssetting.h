#ifndef SYSSETTING_H
#define SYSSETTING_H

#include <QWidget>

namespace Ui {
class SysSetting;
}

class SysSetting : public QWidget
{
    Q_OBJECT
    
public:
    explicit SysSetting(QWidget *parent = 0);
    ~SysSetting();
	bool exit_flag;
    
private slots:

    void on_pbParaSet_clicked();

    void on_pbParaCali_clicked();

    void on_pbTimeCali_clicked();

    void on_pbExit_clicked();

    void on_pbUpDate_clicked();

	void on_pbNetSet_clicked();

	void goto_TempSet();

private:
    Ui::SysSetting *ui;
};

#endif // SYSSETTING_H
