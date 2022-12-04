#include "KMCGUI.h"
#include <QFileDialog>
#include <kmc_runner.h>
#include <iostream>
#include <algorithm>
#include "configuration.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <QFutureWatcher>
#include <windows.h>
#include <QtConcurrent/QtConcurrent>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "DataTable.h"

KMC::Runner runner;
KMC::Stage1Params stage1Params;

KMCGUI::KMCGUI(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	configureSliders(ui);
	configureCheckboxes(ui);
	ui.progressBarValue->setValue(0);

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

void KMCGUI::SetLabel(const std::string& label)
{
	this->label = label;
}

void KMCGUI::ProgressChanged(int newValue)
{
	emit computationProgress(newValue);
	emit repaintProgressBar();
	qApp->processEvents();
}

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

void KMCGUI::on_cutoffMinSliderStage2_valueChanged()
{
	ui.cutoffMinValue->setText(QString::number(ui.cutoffMinSliderStage2->value()));
}

void KMCGUI::on_counterMaxSliderStage2_valueChanged()
{
	ui.counterMaxValue->setText(QString::number(ui.counterMaxSliderStage2->value()));
}

void KMCGUI::on_cutoffMaxSliderStage2_valueChanged()
{
	ui.cutOffMaxValue->setText(QString::number(ui.cutoffMaxSliderStage2->value()));
}

void KMCGUI::on_chooseButton_clicked()
{
	QFileDialog* fileDialog = new QFileDialog();
	fileNames = fileDialog->getOpenFileNames(this, tr("Open Files"),
		"",
		tr("Fastq file (*.fastq);;Fq file (*.fq);;Fasta file (*.fasta);;Fa file (*.fa);;Compressed Fastq file (*.fastq.gz);;Compressed Fq file (*.fq.gz);;Compressed Fasta file (*.fasta.gz);;Compressed fa file (*.fa.gz);;Fm file (*.fm);;Compressed fm file (*.fm.gz)"));
	if (fileNames.count() > 0)
	{
		ui.choosenFileLabel->setText(fileNames[0]);
	}
}

void KMCGUI::on_runButton_clicked()
{
	ui.progressBarValue->setValue(0);

	connect(this, SIGNAL(repaintProgressBar()), ui.progressBarValue, SLOT(repaint()), Qt::QueuedConnection);
	connect(this, SIGNAL(computationProgress(int)), ui.progressBarValue, SLOT(setValue(int)), Qt::QueuedConnection);

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
			QString outputFileNameQString = ui.outputFileName->toPlainText();
			std::string outputFileName = outputFileNameQString.toStdString();

			auto fileName = ui.outputFileName->toPlainText();
			outputFileName = fileName.toLocal8Bit().constData();

			stage1Params
				.SetKmerLen(ui.kmerLengthSlider->value())
				.SetNThreads(ui.threadsSlider->value())
				.SetMaxRamGB(ui.GBSlider->value())
				.SetSignatureLen(ui.signatureLengthSlider->value())
				.SetHomopolymerCompressed(ui.homopomylerCompressionCheckBox->isChecked())
				.SetCanonicalKmers(ui.canoncialKmersCheckBox->isChecked())
				.SetRamOnlyMode(ui.ramModeCheckBox->isChecked())
				.SetNBins(ui.nBinsSlider->value())
				.SetInputFiles(stringFileNames)
				.SetPercentProgressObserver(this);

			ui.progressBarValue->repaint();
			qApp->processEvents();

			KMC::Stage1Results stage1Result = runner.RunStage1(stage1Params);

			ui.progressBarValue->repaint();
			qApp->processEvents();

			KMC::Stage2Params stage2Params;

			if (outputFileName == "")
			{
				stage2Params
					.SetNThreads(ui.threadsSliderStage2->value())
					.SetMaxRamGB(ui.GBSliderStage2->value())
					.SetStrictMemoryMode(ui.strictMemoryModeCheckbox->isChecked())
					.SetCutoffMin(ui.cutoffMinSliderStage2->value())
					.SetCutoffMax(ui.cutoffMaxSliderStage2->value())
					.SetCounterMax(ui.counterMaxSliderStage2->value())
					.SetOutputFileName("31mers");

				lastOutputFileName = "31mers";
			}
			else
			{
				stage2Params
					.SetNThreads(ui.threadsSliderStage2->value())
					.SetMaxRamGB(ui.GBSliderStage2->value())
					.SetStrictMemoryMode(ui.strictMemoryModeCheckbox->isChecked())
					.SetCutoffMin(ui.cutoffMinSliderStage2->value())
					.SetCutoffMax(ui.cutoffMaxSliderStage2->value())
					.SetCounterMax(ui.counterMaxSliderStage2->value())
					.SetOutputFileName(outputFileName);

				lastOutputFileName = outputFileName;
			}

			auto stage2Result = runner.RunStage2(stage2Params);

			std::string totalKmers = std::to_string(stage2Result.nTotalKmers);
			std::string uniqueKmers = std::to_string(stage2Result.nUniqueKmers);

			std::string st = "./kmc_tools transform " + lastOutputFileName + " dump "
				+ lastOutputFileName + ".txt";

			/*wchar_t* wString = new wchar_t[4096];
			MultiByteToWideChar(CP_ACP, 0, st.c_str(), -1, wString, 4096);
			ShellExecute(NULL, NULL, wString, NULL, NULL, SW_HIDE);*/

			WinExec(st.c_str(), 0);

			ui.totalKmersValue->setText(QString::fromUtf8(totalKmers.c_str()));
			ui.totalUniqueKmersValue->setText(QString::fromUtf8(uniqueKmers.c_str()));
		}
		catch (const std::exception& e)
		{
			ui.errorMessageBox->setText(QString::fromUtf8(e.what()));
		}
	}
}

void KMCGUI::on_checkDatabaseButton_clicked()
{
	dataTable = new DataTable(this);
	dataTable->resultFileName = lastOutputFileName;
	dataTable->show();
	dataTable->showDataTable();
}