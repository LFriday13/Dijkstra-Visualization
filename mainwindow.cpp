#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0, 400, 400);
    connect( scene, SIGNAL( selectionChanged() ), this, SLOT( selection_finished() ));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scale(qreal(1), qreal(1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_drawVertex_toggled(bool checked) {
    if(checked){ui->drawEdge->setChecked(false);}
    if(checked){ui->start->setChecked(false);}
    ui->graphicsView->set_drawing_vertices(checked);
}

void MainWindow::on_drawEdge_toggled(bool checked) {
    if(checked){ui->drawVertex->setChecked(false);}
    if(checked){ui->start->setChecked(false);}
    ui->graphicsView->set_drawing_edges(checked);
}

void MainWindow::on_randGenerate_clicked() {
    ui->drawVertex->setChecked(false);
    ui->drawEdge->setChecked(false);
    ui->start->setChecked(false);
    ui->graphicsView->generate();
}

void MainWindow::on_start_toggled(bool checked) {
    if(checked){ui->drawVertex->setChecked(false);}
    if(checked){ui->drawEdge->setChecked(false);}
    ui->graphicsView->set_started(checked);
}

void MainWindow::on_step_clicked() {
    ui->graphicsView->step();
}

void MainWindow::selection_finished() {
    ui->start->setChecked(false);
}

void MainWindow::on_clear_clicked()
{
    ui->graphicsView->clear_paths();
    ui->showPath->setChecked(false);
}

void MainWindow::on_showPath_toggled(bool checked)
{
    ui->graphicsView->set_showing(checked);
}
