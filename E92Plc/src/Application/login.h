#ifndef LOGIN_H
#define LOGIN_H

//#include <QDialog>
#include <QWidget>
#include "src/HardWare/keyboard_dependence.h"
#include "NumberKeyBoard.h"
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT
    
public:
    bool Admin_flag;
    explicit Login(QWidget *parent = 0);
    ~Login();

	void Login_Init();   //初始化
	void Login_UnInit();	// 界面跳出处理

	bool exit_flag;

signals:
   //void emitLoginAndSecret();
   //void HaveLogined();

public slots:
   //void loginAndcancel();
   void InsertValue(QString nustr);
   void NumberBack();

private slots:
   void on_comboBox_currentIndexChanged(int index);
   void on_p_login_clicked();
   void on_p_cancel_clicked();

private:
    Ui::Login *ui;
    Keyboard_dependence* im;
    QLineEdit *p_editer;
    QLineEdit* p_lineedite;


};
extern Login* p_login;
extern int g_permission ;
#endif // LOGIN_H
