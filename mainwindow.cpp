#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "triggeritem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->scene = new TimelineScene();
    this->scene->addTriggerItem(0, 0, 0, "Annonce");
    this->scene->addTriggerItem(5, 1, 1, "L1");
    this->scene->addTriggerItem(20, 2, 1, "L2");
    this->scene->addTriggerItem(35, 3, 2, "F1");

    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    ui->graphicsView->setScene(this->scene);
    ui->graphicsView->setSceneRect(-100.0, -100.0,
                                   this->scene->maxLength() * (1/this->scene->ratio()), 200.0);
    ui->graphicsView->centerOn(0.0, 0.0);
    //ui->graphicsView->scale(2.0, 2.0);
    //ui->graphicsView->setMaximumHeight(200);
    ui->graphicsView->show();
    //setCentralWidget(ui->graphicsView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
