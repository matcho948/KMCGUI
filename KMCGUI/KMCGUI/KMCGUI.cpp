#include "KMCGUI.h"
#include <QFileDialog>
#include <kmc_runner.h>
#include <iostream>
#include <algorithm>
#include "configuration.h"

KMCGUI::KMCGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    configureSliders(ui);
    configureCheckboxes(ui);

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
    connect(ui.nBinsSlider, SIGNAL(ui.nBinsSlider->valueChanged()), this,
        SLOT(on_nBinsSlider_valueChanged()()));
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

void KMCGUI::on_nBinsSlider_valueChanged()
{
    ui.nBinsValue->setText(QString::number(ui.nBinsSlider->value()));
}

void KMCGUI::on_threadsSliderStage2_valueChanged()
{
    ui.threadsValueStage2->setText(QString::number(ui.threadsSliderStage2->value()));

}

void KMCGUI::on_GBSliderStage2_valueChanged()
{
    ui.maxGBValueStage2->setText(QString::number(ui.GBSliderStage2->value()));
}

void KMCGUI::on_chooseButton_clicked()
{
    QFileDialog* fileDialog = new QFileDialog();
    fileNames = fileDialog->getOpenFileNames(this, tr("Open Files"),
        "",
        tr("Fastq file (*.fastq);;Fq file (*.fq);;Fasta file (*.fasta);;Fa file (*.fa);;Compressed Fastq file (*.fastq.gz);;Compressed Fq file (*.fq.gz);;Compressed Fasta file (*.fasta.gz);;Compressed fa file (*.fa.gz);;Fm file (*.fm);;Compressed fm file (*.fm.gz)"));
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
                .SetNBins(ui.nBinsSlider->value())
                .SetInputFiles(stringFileNames);

            auto stage1Result = runner.RunStage1(stage1Params);

            KMC::Stage2Params stage2Params;

            stage2Params
                .SetNThreads(ui.threadsSliderStage2->value())
                .SetMaxRamGB(ui.GBSliderStage2->value())
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