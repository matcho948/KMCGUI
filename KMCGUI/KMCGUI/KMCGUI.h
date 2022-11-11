#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KMCGUI.h"
#include <kmc_runner.h>


class KMCGUI : public QMainWindow, public KMC::IPercentProgressObserver
{
    Q_OBJECT

public:
    KMCGUI(QWidget *parent = nullptr);
    ~KMCGUI();
    void SetLabel(const std::string& label) override;
    void ProgressChanged(int newValue) override;
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
signals:
    void computationProgress(int progress);
    void repaintProgressBar();
private:	
    std::string label;
    Ui::KMCGUIClass ui;
    QStringList fileNames; 
};
