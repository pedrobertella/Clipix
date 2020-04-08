#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QProgressBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    QProgressBar *pbar;

private slots:

    void on_spinQualidade_valueChanged(int arg1);

    void on_radioPorcentagem_toggled(bool checked);

    void on_sliderQualidade_valueChanged(int value);

    void on_actionSAir_triggered();

    void on_btnCaminho_clicked();

    void on_actionLimpar_Arquivos_triggered();

    void on_actionAbrir_Imagens_triggered();

    void on_spinAltPor_valueChanged(int arg1);

    void on_spinLarPor_valueChanged(int arg1);

    void on_actionVoltar_aos_Padr_es_triggered();

    void on_actionIniciar_triggered();

    void on_actionSobre_triggered();

    void on_actionSobre_Qt_triggered();

private:
    Ui::MainWindow *ui;
    bool goodPath();
    bool compressImages(QString path, QString finalName);
    QString getSavePath(QString fullPath);
    int getNewWitdh(int size);
    int getNewHeight(int size);
};

#endif // MAINWINDOW_H
