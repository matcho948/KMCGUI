#include "DataTable.h"
#include <windows.h>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "DataTableModel.h"
#include <QStandardItemModel>
#include "kmc_file.h"

DataTable::DataTable(QWidget* parent)
	: QMainWindow(parent)
{
	currentPage = 1;
	ui.setupUi(this);
}

DataTable::DataTable()
{
	currentPage = 1;
	ui.setupUi(this);
}

DataTable::~DataTable()
{}

void DataTable::showDataTable()
{
	std::ifstream aFile;
	aFile.open(resultFileName + ".txt");
	int lines_count = std::count(std::istreambuf_iterator<char>(aFile),
		std::istreambuf_iterator<char>(), '\n');

	totalPages = lines_count % 10 == 0 ? lines_count / 10 : (lines_count / 10 + 1);

	ui.totalPages->setText(QString::number(totalPages));
	aFile.close();

	std::ifstream secondFile;
	secondFile.open(resultFileName + ".txt");

	int iterator = 0;
	std::vector<DataTableModel> data;

	if (currentPage == totalPages)
	{
		showFirstOrLastPage();
	}
	else
	{
		showPage();
	}
	aFile.close();
}

void DataTable::on_previousPageButton_clicked()
{
	if (currentPage > 1)
	{
		currentPage--;
		ui.currentPage->setText(QString::number(currentPage));

		std::ifstream secondFile;
		secondFile.open(resultFileName + ".txt");

		int iterator = 0;
		int it2 = 0;
		std::vector<DataTableModel> data;

		while (secondFile.eof() || iterator < 10)
		{
			std::string kmer;
			std::string occurences;
			while ((currentPage - 1) * 10 > it2)
			{
				secondFile >> kmer >> occurences;
				it2++;
			}
			secondFile >> kmer >> occurences;
			DataTableModel dataTableModel(kmer, occurences);
			data.push_back(dataTableModel);
			iterator++;
		}
		QStandardItemModel* model = new QStandardItemModel(1, 2, this);
		QStringList headers;
		headers.append("Kmer");
		headers.append("Occurencces");

		model->setColumnCount(2);
		model->setHorizontalHeaderLabels(headers);

		for (int row = 0; row < 10; row++)
		{
			if (row < data.size())
			{
				DataTableModel dataModel = data[row];

				model->setItem(row, 0, new QStandardItem(QString::fromUtf8(dataModel.kmer.c_str())));
				model->setItem(row, 1, new QStandardItem(QString::fromUtf8(dataModel.occurences.c_str())));
			}
		}

		ui.dataTable->setModel(model);
	}
}

void DataTable::on_nextPageButton_clicked()
{
	if (currentPage < totalPages)
	{
		currentPage++;
		ui.currentPage->setText(QString::number(currentPage));

		std::ifstream secondFile;
		secondFile.open(resultFileName + ".txt");

		int iterator = 0;
		int it2 = 0;
		std::vector<DataTableModel> data;

		if (currentPage == totalPages)
		{
			showFirstOrLastPage();
		}
		else
		{
			showPage();
		}
	}
}

void DataTable::on_searchButton_clicked()
{
	try
	{
		CKMCFile kmer_data_base;
		CKmerAPI kmer;
		QString qstring = ui.kmerSearchValue->text();
		std::string stringKmer = qstring.toLocal8Bit().constData();
		kmer.from_string(stringKmer);
		uint64 kmercounter;

		if (kmer_data_base.OpenForRA(resultFileName))
		{
			ui.kmerValue->setText(ui.kmerSearchValue->text());
			if (kmer_data_base.CheckKmer(kmer, kmercounter))
			{
				ui.kmerOccurences->setText(QString::number(kmercounter));
			}
			else
			{
				ui.kmerOccurences->setText(QString::number(0));
			}
		}
	}
	catch (...)
	{
		ui.kmerOccurences->setText(QString::number(0));
	}
}

void DataTable::on_goButton_clicked()
{
	QString qPage = ui.goPage->text();
	int page = qPage.toInt();
	int tmpCurrentPage = currentPage;
	currentPage = page;

	if (currentPage <= totalPages && currentPage >= 1)
	{
		ui.errorPageLabel->setText("");

		if (currentPage == totalPages)
		{
			showFirstOrLastPage();
		}
		else
		{
			showPage();
		}
	}
	else
	{
		ui.errorPageLabel->setText("Page out of range");
		currentPage = tmpCurrentPage;
	}
}

void DataTable::showFirstOrLastPage()
{
	ui.currentPage->setText(QString::number(currentPage));

	std::ifstream secondFile;
	secondFile.open(resultFileName + ".txt");

	int iterator = 0;
	int it2 = 0;
	std::vector<DataTableModel> data;

	if (currentPage == totalPages)
	{
		std::ifstream aFile;
		aFile.open(resultFileName + ".txt");
		int lines_count = std::count(std::istreambuf_iterator<char>(aFile),
			std::istreambuf_iterator<char>(), '\n');

		int newIterator = lines_count % 10 == 0 ? 10 : lines_count % 10;

		aFile.close();

		while (secondFile.eof() || iterator < newIterator)
		{
			std::string kmer;
			std::string occurences;
			while ((currentPage - 1) * 10 > it2)
			{
				secondFile >> kmer >> occurences;
				it2++;
			}
			secondFile >> kmer >> occurences;
			DataTableModel dataTableModel(kmer, occurences);
			data.push_back(dataTableModel);
			iterator++;
		}
		QStandardItemModel* model = new QStandardItemModel(1, 2, this);
		QStringList headers;
		headers.append("Kmer");
		headers.append("Occurencces");

		model->setColumnCount(2);
		model->setHorizontalHeaderLabels(headers);

		for (int row = 0; row < 10; row++)
		{
			if (row < data.size())
			{
				DataTableModel dataModel = data[row];

				model->setItem(row, 0, new QStandardItem(QString::fromUtf8(dataModel.kmer.c_str())));
				model->setItem(row, 1, new QStandardItem(QString::fromUtf8(dataModel.occurences.c_str())));
			}
		}
		ui.dataTable->setModel(model);
	}
}

void DataTable::showPage()
{
	ui.currentPage->setText(QString::number(currentPage));

	std::ifstream secondFile;
	secondFile.open(resultFileName + ".txt");

	int iterator = 0;
	int it2 = 0;
	std::vector<DataTableModel> data;

	while (secondFile.eof() || iterator < 10)
	{
		std::string kmer;
		std::string occurences;
		while ((currentPage - 1) * 10 > it2)
		{
			secondFile >> kmer >> occurences;
			it2++;
		}
		secondFile >> kmer >> occurences;
		DataTableModel dataTableModel(kmer, occurences);
		data.push_back(dataTableModel);
		iterator++;
	}
	QStandardItemModel* model = new QStandardItemModel(1, 2, this);
	QStringList headers;
	headers.append("Kmer");
	headers.append("Occurencces");

	model->setColumnCount(2);
	model->setHorizontalHeaderLabels(headers);

	for (int row = 0; row < 10; row++)
	{
		if (row < data.size())
		{
			DataTableModel dataModel = data[row];

			model->setItem(row, 0, new QStandardItem(QString::fromUtf8(dataModel.kmer.c_str())));
			model->setItem(row, 1, new QStandardItem(QString::fromUtf8(dataModel.occurences.c_str())));
		}
	}
	ui.dataTable->setModel(model);
}