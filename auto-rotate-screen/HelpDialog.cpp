#include <QMessageBox>
#include "HelpDialog.h"
#include "ui_HelpDialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    //To remove the default help button beside the top close button
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    flags = flags & (~helpFlag);
    this->setWindowFlags(flags);

    ui->setupUi(this);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::on_pushButton_clicked()
{
    QMessageBox::aboutQt(this, "About QT");
}
