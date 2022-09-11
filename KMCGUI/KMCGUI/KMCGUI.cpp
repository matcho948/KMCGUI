#include "KMCGUI.h"
#include <QFileDialog>
#include <kmc_runner.h>
#include <iostream>

KMCGUI::KMCGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.chooseButton, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked()));
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
}

KMCGUI::~KMCGUI()
{}

void KMCGUI::on_pushButton_2_clicked()
{
    QFileDialog* fileDialog = new QFileDialog();
    fileNames = fileDialog->getOpenFileNames(this, tr("Open Files"),
        "",
        tr("Fasta files (*.fq *.fastq)"));
}

void KMCGUI::on_pushButton_clicked()
{
    try
    {
        KMC::Runner runner;

        KMC::Stage1Params stage1Params;
        stage1Params
            .SetKmerLen(3)
            .SetInputFiles({ "C:\\Users\\match\\Desktop\\test1.fq"});

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
