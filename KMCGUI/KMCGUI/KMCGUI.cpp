#include "KMCGUI.h"
#include <QFileDialog>
#include <kmc_runner.h>
#include <iostream>
#include <algorithm>

KMCGUI::KMCGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.kmerLengthSlider->setMinimum(1);
    ui.kmerLengthSlider->setMaximum(100);
    ui.kmerLengthSlider->setValue(1);
    ui.threadsSlider->setMinimum(1);
    ui.threadsSlider->setMaximum(64);
    ui.threadsSlider->setValue(std::thread::hardware_concurrency());
    ui.GBSlider->setMinimum(1);
    ui.GBSlider->setMaximum(128);
    ui.GBSlider->setValue(12);
    ui.signatureLengthSlider->setMinimum(5);
    ui.signatureLengthSlider->setMaximum(11);
    ui.signatureLengthSlider->setValue(9);
    ui.canoncialKmersCheckBox->setChecked(1);
    connect(ui.chooseButton, SIGNAL(clicked()), this, SLOT(on_chooseButton_clicked()));
    connect(ui.runButton, SIGNAL(clicked()), this, SLOT(on_runButton_clicked()));
    connect(ui.kmerLengthSlider, SIGNAL(ui.horizontalSlider->valueChanged()), this,
        SLOT(on_horizontalSlider_valueChanged()()));
    connect(ui.threadsSlider, SIGNAL(ui.threadsSlider->valueChanged()), this,
        SLOT(on_threadsSlider_valueChanged()()));
    connect(ui.GBSlider, SIGNAL(ui.GBSlider->valueChanged()), this,
        SLOT(on_GBSlider_valueChanged()()));
    connect(ui.signatureLengthSlider, SIGNAL(ui.signatureLengthSlider->valueChanged()), this,
        SLOT(on_signatureLengthSlider_valueChanged()()));
}

KMCGUI::~KMCGUI()
{}

void KMCGUI::on_kmerLengthSlider_valueChanged()
{
    ui.kmerLengthValue->setText(QString::number(ui.kmerLengthSlider->value()));
}

void KMCGUI::on_threadsSlider_valueChanged()
{
    ui.threadsValue->setText(QString::number(ui.threadsSlider->value()));
}

void KMCGUI::on_GBSlider_valueChanged()
{
    ui.maxGBValue->setText(QString::number(ui.GBSlider->value()));
}

void KMCGUI::on_signatureLengthSlider_valueChanged()
{
    ui.signatureLengthValue->setText(QString::number(ui.signatureLengthSlider->value()));
}

void KMCGUI::on_chooseButton_clicked()
{
    QFileDialog* fileDialog = new QFileDialog();
    fileNames = fileDialog->getOpenFileNames(this, tr("Open Files"),
        "",
        tr("Fasta files (*.fq *.fastq)"));
}

void KMCGUI::on_runButton_clicked()
{
    ui.errorMessageBox->setText("");
    ui.totalKmersValue->setText("Wait for result...");
    ui.totalUniqueKmersValue->setText("Wait for result...");

    if (fileNames.count() == 0)
    {
        ui.errorMessageBox->setText("No files choosed");
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
                .SetKmerLen(ui.kmerLengthSlider->value())
                .SetNThreads(ui.threadsSlider->value())
                .SetMaxRamGB(ui.GBSlider->value())
                .SetSignatureLen(ui.signatureLengthSlider->value())
                .SetHomopolymerCompressed(ui.homopomylerCompressionCheckBox->isChecked())
                .SetCanonicalKmers(ui.canoncialKmersCheckBox->isChecked())
                .SetRamOnlyMode(ui.ramModeCheckBox->isChecked())
                .SetInputFiles(stringFileNames);

            auto stage1Result = runner.RunStage1(stage1Params);

            KMC::Stage2Params stage2Params;

            stage2Params
                .SetOutputFileName("31mers");

            auto stage2Result = runner.RunStage2(stage2Params);
        

            std::string totalKmers = std::to_string(stage2Result.nTotalKmers);
            std::string uniqueKmers = std::to_string(stage2Result.nUniqueKmers);

            ui.totalKmersValue->setText(QString::fromUtf8(totalKmers.c_str()));
            ui.totalUniqueKmersValue->setText(QString::fromUtf8(uniqueKmers.c_str()));
            
        }
        catch (const std::exception& e)
        {
            ui.errorMessageBox->setText(QString::fromUtf8(e.what()));
        }
    }
}