#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "findcontour.h"
#include "imageprocessing.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNova_triggered();

    void on_pushButton_clicked();

    void on_actionOpen_File_triggered();

    void on_shrinkage_limit_2_textChanged(const QString &arg1);

    void on_shrinkage_limit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
