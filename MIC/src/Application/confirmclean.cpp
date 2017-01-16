#include "confirmclean.h"
#include "ui_confirmclean.h"

ConfirmClean::ConfirmClean(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmClean)
{
    ui->setupUi(this);
}

ConfirmClean::~ConfirmClean()
{
    delete ui;
}
