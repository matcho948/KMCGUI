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

DataTable::DataTable(QWidget* parent)
	: QMainWindow(parent)
{
    ui.setupUi(this);
}

DataTable::DataTable()
{
    ui.setupUi(this);
}

DataTable::~DataTable()
{}

void DataTable::showDataTable()
{
    std::ifstream aFile;
    aFile.open(resultFileName +  ".txt");
    int lines_count = std::count(std::istreambuf_iterator<char>(aFile),
        std::istreambuf_iterator<char>(), '\n');

    ui.totalPages->setText(QString::number((lines_count / 10)+1));
    aFile.close();

    std::ifstream secondFile;
    secondFile.open(resultFileName + ".txt");

    int iterator = 0;
    std::vector<DataTableModel> data;
    while ( secondFile.eof() || iterator < 10)
    {
        std::string kmer;
        std::string occurences;
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
    aFile.close();
}