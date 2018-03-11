#include "projectmanagerwindow.h"
#include "ui_projectmanagerwindow.h"

ProjectManagerWindow::ProjectManagerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectManagerWindow)
{
    ui->setupUi(this);
}

ProjectManagerWindow::~ProjectManagerWindow()
{
    delete ui;
}
