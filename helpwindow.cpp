#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    setWindowTitle("Help");
    ui->stackedWidget->setCurrentIndex(rand()%3);
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
