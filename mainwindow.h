#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_drawVertex_toggled(bool checked);
    void on_drawEdge_toggled(bool checked);

    void on_randGenerate_clicked();

    void on_start_toggled(bool checked);

    void on_step_clicked();

    void selection_finished();

    void on_clear_clicked();

    void on_showPath_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
};
#endif // MAINWINDOW_H
