#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QImage>
#include <QFileDialog>
#include <QColorDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pbar = new QProgressBar();
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(empty);
    ui->mainToolBar->addWidget(new QLabel("Progresso: ", this));
    ui->mainToolBar->addWidget(pbar);
    ui->radioPorcentagem->setChecked(true);
    ui->lineCaminho->setText(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
       e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        QString extension = fileName.split(".",QString::SkipEmptyParts).at(1);
        if(extension=="jpg" || extension=="JPG" || extension=="jpeg" || extension=="JPEG" || extension=="png" ||extension=="PNG" ||extension=="GIF" ||extension=="gif" || extension=="bmp" || extension=="BMP"){
            ui->listImagens->addItem(new QListWidgetItem(QIcon(fileName), fileName));
        }
    }
    QImage temp(e->mimeData()->urls().first().toLocalFile());
}

void MainWindow::on_spinQualidade_valueChanged(int arg1)
{
    ui->sliderQualidade->setValue(arg1);
}

void MainWindow::on_radioPorcentagem_toggled(bool checked)
{
        ui->spinAltPor->setEnabled(checked);
        ui->spinLarPor->setEnabled(checked);
        ui->spinAltPix->setEnabled(!checked);
        ui->spinLarPix->setEnabled(!checked);
}

void MainWindow::on_sliderQualidade_valueChanged(int value)
{
    ui->spinQualidade->setValue(value);
}

void MainWindow::on_actionSAir_triggered()
{
    QApplication::quit();
}

void MainWindow::on_btnCaminho_clicked()
{
    QString caminho = QFileDialog::getExistingDirectory(this, "Selecione a pasta de Saída", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    ui->lineCaminho->setText(caminho);
}

void MainWindow::on_actionLimpar_Arquivos_triggered()
{
    QList<QListWidgetItem*> itens = ui->listImagens->selectedItems();
    foreach(QListWidgetItem *i, itens){
        delete i;
   }
}

void MainWindow::on_actionAbrir_Imagens_triggered()
{
    QList<QUrl> imagens = QFileDialog::getOpenFileUrls(this, "Selecione as imagens para compressão", QUrl(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)), "Imagens (*.png *.bmp *.jpg *.jpeg);;JPEG (*.jpg *.jpeg);; PNG (*.png);; BMP (*.bmp)");
    if(!imagens.isEmpty()){
        foreach (const QUrl &url, imagens) {
            QString fileName = url.toLocalFile();
            ui->listImagens->addItem(new QListWidgetItem(QIcon(fileName), fileName));
        }
        QImage temp(imagens.first().toLocalFile());
    }
}

bool MainWindow::goodPath()
{
    if(QDir(ui->lineCaminho->text()).exists()){
        QFile *file = new QFile(ui->lineCaminho->text()+"/test.tmp");
        if (file->open(QIODevice::WriteOnly)) {
            file->remove();
            delete file;
            return true;
        }else{
            delete file;
            return false;
        }
    }else{
        return false;
    }
}

bool MainWindow::compressImages(QString path, QString finalName)
{
    int q = ui->spinQualidade->value();
    QString formato = ui->comboFormato->currentText();
    QImage img(path);
    QImage i2(img.size(), QImage::Format_RGB32);
    QColor co;
    co = co.fromRgb(255,255,255,255);
    i2.fill(co);
    QPainter painter(&i2);
    painter.drawImage(0,0,img);
    painter.end();
    i2 = i2.scaled(getNewWitdh(img.width()),getNewHeight(img.height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return i2.save(finalName, formato.toLocal8Bit().constData(), q);
}

QString MainWindow::getSavePath(QString fullPath)
{
    QString filePath = fullPath;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    QString fileName = filePathList.at(filePathList.count() - 1);
    fileName = fileName.split(".",QString::SkipEmptyParts).at(0);
    saveFilePath = QString(ui->lineCaminho->text() + "/" + fileName + "." + ui->comboFormato->currentText());
    return saveFilePath;
}

int MainWindow::getNewWitdh(int size)
{
   int newSize;
   if(ui->radioPorcentagem->isChecked()){
       newSize = (ui->spinLarPor->value()*size)/100;
   }else{
       newSize = ui->spinLarPix->value();
   }
   return newSize;
}

int MainWindow::getNewHeight(int size)
{
    int newSize;
    if(ui->radioPorcentagem->isChecked()){
        newSize = (ui->spinAltPor->value()*size)/100;
    }else{
        newSize = ui->spinAltPix->value();
    }
    return newSize;
}

void MainWindow::on_spinAltPor_valueChanged(int arg1)
{
    ui->spinLarPor->setValue(arg1);
}

void MainWindow::on_spinLarPor_valueChanged(int arg1)
{
    ui->spinAltPor->setValue(arg1);
}

void MainWindow::on_actionVoltar_aos_Padr_es_triggered()
{
    ui->spinQualidade->setValue(80);
    ui->radioPorcentagem->setChecked(true);
    ui->spinAltPor->setValue(100);
    ui->spinAltPix->setValue(3000);
    ui->spinLarPix->setValue(2000);
    ui->lineCaminho->setText("");
    ui->comboFormato->setCurrentText("JPG");
}

void MainWindow::on_actionIniciar_triggered()
{
    if(ui->listImagens->count()==0){
        QMessageBox::warning(this, "Atenção!", "Selecione imagens para a operação.", QMessageBox::Ok);
        return;
    }
    if(goodPath()){
        QApplication::processEvents();
        int t = ui->listImagens->count();
        pbar->setMinimum(0);
        pbar->setMaximum(t);
        bool res = true, success = true;
        for(int i = 0; i < t; ++i)
        {
            QListWidgetItem* item = ui->listImagens->item(i);
            res = compressImages(item->text(), getSavePath(item->text()));
            pbar->setValue(i);
            if(!res){
                QMessageBox::critical(this, "Erro ao processar a Operação", "Não foi possível comprimir as imagens.", QMessageBox::Ok);
                success=false;
                break;
            }
        }
        if(success){
            QMessageBox::information(this, "Processamento completo", "Todas as imagens foram processadas.", QMessageBox::Ok);
        }
        pbar->setMaximum(100);
        pbar->reset();
    }else{
        QMessageBox::critical(this, "Erro ao iniciar o processamento", "O caminho especificado não é válido.\nVerifique se a pasta existe ou se você possui permissão para gravar.", QMessageBox::Ok);
    }
}

void MainWindow::on_actionSobre_triggered()
{
    QMessageBox *msg = new QMessageBox();
    msg->setText("Sobre PBCompressor v1.0");
    msg->setIcon(QMessageBox::Information);
    msg->setInformativeText("Desenvolvido por Pedro Bertella, 2019.");
    msg->setStandardButtons(QMessageBox::Ok);
    msg->exec();
}

void MainWindow::on_actionSobre_Qt_triggered()
{
    QApplication::aboutQt();
}
