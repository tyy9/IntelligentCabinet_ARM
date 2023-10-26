#include "storedialog.h"
#include "ui_StoreDialog.h"

StoreDialog::StoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StoreDialog)
{
    ui->setupUi(this);
}

StoreDialog::~StoreDialog()
{
    delete ui;
}

void StoreDialog::on_pushButton_pressed()
{
    this->close();
}

