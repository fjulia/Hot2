#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "config.h"


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
     m_config.setDataPath("/dades/hotStage_data/B0");
    //ui->dir_value->setText(Config::dataPath);
}

void MainWindow::on_actionNova_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),m_config.getDataPath(),QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if ( dir.isNull() == false )
    {
        m_config.setDataPath(dir.toLatin1().data());
        ui->dir_value->setText(m_config.getDataPath());
    }
}

void MainWindow::on_pushButton_clicked()
{
    startProcessing(m_config.getDataPath());
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QString(QLatin1String(m_config.getDataPath())),tr("Images (*.png *.xpm *.jpg *.tif *.PNG *.XPM *.JPG *.TIF)"));
    if ( fileName.isNull() == false )
    {
        findContour(fileName.toLatin1().data());
    }
}
