#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QComboBox>
#include "NumberKeyBoard.h"
#include "src/HardWare/keyboard_dependence.h"
#include "debugging.h"
#include <stdio.h>
#include "common.h"
#include "mainwindow.h"

Login *p_login;
//int g_permission = 1;
int g_permission = 0;
Login::Login(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::Login)
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	Admin_flag = false;

	ui->comboBox->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
	ui->label_secret->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
	ui->label_user->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
	ui->p_cancel->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
	ui->p_login->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
	ui->lineEdit_secret->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));

	ui->comboBox->addItem(tr(STR_LOGIN_OP));   // 0
	ui->comboBox->addItem(tr(STR_LOGIN_ADMIN));  // 1
}

Login::~Login()
{
    delete ui;
}
/*
	函数名称：
	函数功能：界面初始化
	输入参数：
	输出参数：
	修改记录：
*/
void Login::Login_Init()
{
	exit_flag = false;
	// 键盘
	im = new Keyboard_dependence;
	ui->lineEdit_secret->setFocusPolicy (Qt::ClickFocus);
	ui->lineEdit_secret->installEventFilter(im);

	connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(InsertValue(QString)));
	//ui->comboBox->setCurrentIndex(0);
	ui->comboBox->setCurrentIndex(1);
}

/*
	函数名称：
	函数功能：界面销毁
	输入参数：
	输出参数：
	修改记录：
*/
void Login::Login_UnInit()
{
	ui->lineEdit_secret->removeEventFilter(im);
	printf("\n Login_UnInit ====11==\r\n");
	disconnect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(InsertValue(QString)));
	delete im;
}
#if 0
/*
    函数名称：
    函数功能：登陆退出
    输入参数：
    输出参数：
    修改记录：
*/
void Login::loginAndcancel()
{

   if(((QPushButton*)sender())->text () == "login")
   {
       if(Admin_flag == true && ui->lineEdit_secret->text ()== "860622")
       {
		   qDebug ()<<"login success!";
           g_permission = 1;

//           emit this->HaveLogined();
  //         emit this->emitLoginAndSecret();
           this->close();

           im->v_key->close ();
           Debugging *pDebug = new Debugging();
           pDebug->showFullScreen();
           return;
	   }
	   else
       {
           qDebug ("login failed!");
           return;
       }
   }
   else if(((QPushButton*)sender())->text () == "cancel")
   {
       this->close ();
       im->v_key->close ();
       return;
   }
}
#endif
// 显示输入
void Login::InsertValue(QString nustr)
{
	qDebug()<<nustr;
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->clear();
    p_editer->setText(nustr);
}

/*
    函数名称：
    函数功能：键盘返回
    输入参数：
    输出参数：
    修改记录：
*/
void Login::NumberBack()
{
    p_lineedite = qobject_cast<QLineEdit*>(focusWidget());
    p_lineedite->backspace ();
}

//　登陆身份选择
void Login::on_comboBox_currentIndexChanged(int index)
{
    if(index == 1)
    {
        Admin_flag = true;
        printf("Admin\n");
    }
    else
    {
        Admin_flag = false;
        printf("Oper\n");
    }
}


//login 登陆

void Login::on_p_login_clicked()
{
	if((true == Admin_flag ) && (STR_LOGIN_PASSWORD == ui->lineEdit_secret->text ()))
	{
		qDebug ()<<"login success!";
		g_permission = 1;
		im->v_key->close ();
		Login_UnInit();

		Debugging *pDebug = new Debugging();
		pDebug->showFullScreen();

		this->close();
	}
	else
	{
		qDebug ("login failed!");   // 注意此处应该加message 提示
	}
}
//取消
void Login::on_p_cancel_clicked()
{
//	getFullScree("login.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		Login_UnInit();
		this->close();
		printf("\non_p_cancel_clicked   ======11=== \r\n");
		pmainwin->MainWindow_Init();
	}
}
