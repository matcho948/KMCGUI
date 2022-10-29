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
    void on_chooseButton_clicked();
    void on_runButton_clicked();
    void on_kmerLengthSlider_valueChanged();
    void on_threadsSlider_valueChanged();
    void on_GBSlider_valueChanged();
    void on_signatureLengthSlider_valueChanged();
    void on_nBinsSlider_valueChanged();
    void on_threadsSliderStage2_valueChanged();
    void on_GBSliderStage2_valueChanged();
    void on_runButton_progressBar();
private:
    Ui::KMCGUIClass ui;
    QStringList fileNames; 
};
