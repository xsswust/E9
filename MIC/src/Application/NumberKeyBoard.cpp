#include "NumberKeyBoard.h"
#include "src/HardWare/keyboard_dependence.h"
#include "stdio.h"
#include <QTextCodec>
#include "common.h"

NumberKeyBoard *pNumberKeyBoard;
NumberKeyBoard::NumberKeyBoard(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    Nustr="";
    ui.leDisplayNumber->setText(Nustr);
    capsLockStatus = false;

    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // Chinese Display
//    QTextCodec::setCodecForTr(gbk);
	QTextCodec::setCodecForLocale(gbk);
//    QTextCodec::setCodecForCStrings(gbk);

    ui.pbBackspace->setFont(QFont(FONT_USE_WQY,16,QFont::Bold));
    ui.pbMinus->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbDot->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbEnter->setFont(QFont(FONT_USE_WQY,16,QFont::Bold));
    ui.pbNumber0->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber1->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber2->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber3->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber4->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber5->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber6->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber7->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber8->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.pbNumber9->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui.leDisplayNumber->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));

    ui.pbEnter->setText("退出");

    connect(ui.pbEnter,SIGNAL(clicked()),this,SLOT(pbEnter()));
    connect(ui.pbBackspace,SIGNAL(clicked()),this,SLOT(pbBackspace()));
    connect(ui.pbMinus,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber0,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber1,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber2,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber3,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber4,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber5,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber6,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber7,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber8,SIGNAL(released()),this,SLOT(pbReleased()));
    connect(ui.pbNumber9,SIGNAL(released()),this,SLOT(pbReleased()));
//    connect(ui.pbSprit,SIGNAL(released()),this,SLOT(pbReleased()));



    connect(ui.pbDot,SIGNAL(released()),this,SLOT(pbReleased()));
//    connect(ui.pbColon,SIGNAL(released()),this,SLOT(pbReleased()));
//    connect(ui.pbSpace,SIGNAL(released()),this,SLOT(pbReleased()));
}

NumberKeyBoard::~NumberKeyBoard()
{

}
void NumberKeyBoard::pbEnter()
{
   // emit emitNustr(Nustr);
    close();
    Nustr="";
    ui.leDisplayNumber->setText(Nustr);

    if(ui.pbEnter->text()=="确定")
    {
        emit clearFocusFlag();
		//printf("emit clearFocusFlag()\n");
    }
    else
    {
        emit recoverDataLineEdit();
		//printf("emit recoverDataLineEdit()\n");
        emit emitNustr("");
    }
    ui.pbEnter->setText("退出");

}

void NumberKeyBoard::pbBackspace()
{
    emit del();

    /*2014.8.9修改,按一下退格删除一位*/
    Nustr = Nustr.left(Nustr.length() - 1);
    ui.leDisplayNumber->backspace();
    emit emitNustr(Nustr);
}

void NumberKeyBoard::pbReleased()
{
    pushBtn = qobject_cast<QPushButton*>(QObject::sender());
    text=pushBtn->text();
    if(text == "Space")
    {
        text = " ";
    }
    Nustr=Nustr+text;
    emit setValue(text);
    ui.leDisplayNumber->setText(Nustr);

    emit emitNustr(Nustr);

    if(Nustr != "")
    {
        ui.pbEnter->setText("确定");
    }
    else
    {
        ui.pbEnter->setText("退出");
    }
}
