#ifndef SYSHELP_H
#define SYSHELP_H

#include <QWidget>

namespace Ui {
class SysHelp;
}

class SysHelp : public QWidget
{
    Q_OBJECT
    
public:
    explicit SysHelp(QWidget *parent = 0);
    ~SysHelp();
	bool exit_flag;
	bool sysinfo_flag;
    
private slots:
    void on_pushButton_clicked();
	void on_pb_sysInfo_released();

private:
    Ui::SysHelp *ui;
};

#endif // SYSHELP_H
