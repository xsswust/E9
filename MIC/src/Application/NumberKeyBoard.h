#ifndef NUMBERKEYBOARD_H
#define NUMBERKEYBOARD_H

#include <QWidget>
#include <QSignalMapper>
#include <QPushButton>
#include <QLineEdit>
#include "ui_NumberKeyBoard.h"

class NumberKeyBoard : public QWidget
{
    Q_OBJECT
public:
    QString Nustr;
    NumberKeyBoard(QWidget *parent = 0);
    ~NumberKeyBoard();
    QSignalMapper *mapper;
    QString text;
    QPushButton* pushBtn;
public slots:
     void pbEnter();
     void pbBackspace();
     void pbReleased();
     //void keyBoardReleased();
signals:
    void setValue(const QString &);
    void del();
    void clearFocusFlag();
    void recoverDataLineEdit();

    void emitNustr(QString);
private:
    bool capsLockStatus;
    int LineEditNumberCode;
    Ui::NumberKeyBoardClass ui;
};
extern NumberKeyBoard *pNumberKeyBoard;
#endif // NUMBERKEYBOARD_H
