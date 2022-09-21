#include "KMCGUI.h"
#include <QFileDialog>
#include <kmc_runner.h>
#include <iostream>
#include <algorithm>

KMCGUI::KMCGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.horizontalSlider->setMinimum(1);
    ui.horizontalSlider->setMaximum(100);
    ui.horizontalSlider->setValue(1);
    connect(ui.chooseButton, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked()));
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui.horizontalSlider, SIGNAL(ui.horizontalSlider->valueChanged()), this,
        SLOT(on_horizontalSlider_valueChanged()()));
}

KMCGUI::~KMCGUI()
{}

void KMCGUI::on_horizontalSlider_valueChanged()
{
    ui.label_10->setText(QString::number(ui.horizontalSlider->value()));
}

void KMCGUI::on_pushButton_2_clicked()
{
    QFileDialog* fileDialog = new QFileDialog();
    fileNames = fileDialog->getOpenFileNames(this, tr("Open Files"),
        "",
        tr("Fasta files (*.fq *.fastq)"));
}

void KMCGUI::on_pushButton_clicked()
{
    if (fileNames.count() == 0)
    {
        ui.label_4->setText("No files choosed");
    }
    else 
    {
        std::vector<std::string> stringFileNames; 
        for (int i = 0; i < fileNames.size(); i++)
        {
            std::string fileName = fileNames[i].toLocal8Bit().constData();
            std::replace(fileName.begin(), fileName.end(), '/', '\\');
            stringFileNames.push_back(fileName);
        }
        try
        {
            KMC::Runner runner;

            KMC::Stage1Params stage1Params;
            stage1Params
                .SetKmerLen(ui.horizontalSlider->value())
                .SetInputFiles(stringFileNames);

            auto stage1Result = runner.RunStage1(stage1Params);

            KMC::Stage2Params stage2Params;

            stage2Params
                .SetOutputFileName("31mers");

            auto stage2Result = runner.RunStage2(stage2Params);
        

            std::string totalKmers = std::to_string(stage2Result.nTotalKmers);
            std::string uniqueKmers = std::to_string(stage2Result.nUniqueKmers);

            ui.label_4->setText(QString::fromUtf8(totalKmers.c_str()));
            ui.label_5->setText(QString::fromUtf8(uniqueKmers.c_str()));

        }
        catch (const std::exception& e)
        {
            ui.label->setText(QString::fromUtf8(e.what()));
        }
    }
}