#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "config.h"
#include "utils.h"


bool started;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     m_config.setDataPath("/dades/hotStage_data/B0");
}

MainWindow::~MainWindow()
{
    delete ui;
     m_config.setDataPath((char *)"/dades/hotStage_data/B0");
    ui->dir_value->setText(QString::fromStdString(m_config.getDataPath()));
    ui->shrinkage_limit->setText(QString::number(m_config.getShrinkage()));
    ui->shrinkage_limit_2->setText(QString::number(m_config.getShrinkage2()));
}

void MainWindow::on_actionNova_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),m_config.getDataPath().c_str(),QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if ( dir.isNull() == false )
    {
        m_config.setDataPath(dir.toLatin1().data());
        ui->dir_value->setText(QString::fromStdString(m_config.getDataPath()));
    }
}

void MainWindow::on_pushButton_clicked()
{
    startProcessing(m_config.getDataPath());
}

void MainWindow::on_actionOpen_File_triggered()
{
    started = false;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QString::fromStdString(m_config.getDataPath()),tr("Images (*.png *.xpm *.jpg *.tif *.PNG *.XPM *.JPG *.TIF)"));
    string path = SplitFilename(fileName.toLatin1().data());
    m_config.setDataPath(path);
    ui->dir_value->setText(QString::fromStdString(path));
    if ( fileName.isNull() == false )
    {
        findContour(fileName.toLatin1().data());
        ui->bottom_value->setText(QString::number(m_config.noise_down));
        ui->rotation_value->setText(QString::number(m_config.rotation));

    }
}

void MainWindow::on_shrinkage_limit_2_textChanged(const QString &arg1)
{
     m_config.setShrinkage2(arg1.toInt());
}

void MainWindow::on_shrinkage_limit_textChanged(const QString &arg1)
{
    m_config.setShrinkage(arg1.toInt());
}

