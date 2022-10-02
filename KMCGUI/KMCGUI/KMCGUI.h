#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KMCGUI.h"

class KMCGUI : public QMainWindow
{
    Q_OBJECT

public:
    KMCGUI(QWidget *parent = nullptr);
    ~KMCGUI();

public slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_kmerLengthSlider_valueChanged();
    void on_threadsSlider_valueChanged();
    void on_GBSlider_valueChanged();
    void on_horizontalSlider_4_valueChanged();
private:
    Ui::KMCGUIClass ui;
    QStringList fileNames; 
};
