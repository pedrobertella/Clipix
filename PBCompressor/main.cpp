#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings settings("Pedro Bertella", "PBCompressor");
    QVariant check = settings.value("tema");
    if(check.isNull()){
        settings.setValue("tema", 0);
    }
    if(settings.value("tema").toInt()==1){
        a.setStyle(QStyleFactory::create("Fusion"));
        QPalette p = a.palette();
        p.setColor(QPalette::Window,QColor(53,53,53));
        p.setColor(QPalette::WindowText,Qt::white);
        p.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
        p.setColor(QPalette::Base,QColor(42,42,42));
        p.setColor(QPalette::AlternateBase,QColor(66,66,66));
        p.setColor(QPalette::ToolTipBase,Qt::white);
        p.setColor(QPalette::ToolTipText,Qt::white);
        p.setColor(QPalette::Text,Qt::white);
        p.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
        p.setColor(QPalette::Dark,QColor(35,35,35));
        p.setColor(QPalette::Shadow,QColor(20,20,20));
        p.setColor(QPalette::Button,QColor(53,53,53));
        p.setColor(QPalette::ButtonText,Qt::white);
        p.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
        p.setColor(QPalette::BrightText,Qt::red);
        p.setColor(QPalette::Link,QColor(42,130,218));
        p.setColor(QPalette::Highlight,QColor(42,130,218));
        p.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
        p.setColor(QPalette::HighlightedText,Qt::white);
        p.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));
        a.setPalette(p);
    }else{
        settings.setValue("tema", 0);
        a.setStyle(QStyleFactory::create("Fusion"));
    }
    MainWindow w(nullptr, &a);
    w.show();

    return a.exec();
}
