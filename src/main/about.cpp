#include <QPushButton>
#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, [&](){delete this;} );
}

About::~About()
{
    delete ui;
}
