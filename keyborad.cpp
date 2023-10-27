#include "keyborad.h"
#include "ui_keyborad.h"

KeyBorad::KeyBorad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyBorad)
{
    ui->setupUi(this);
}

KeyBorad::~KeyBorad()
{
    delete ui;
}
