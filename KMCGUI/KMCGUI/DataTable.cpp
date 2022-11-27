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
    aFile.open(resultFileName +  ".txt");
    int lines_count = std::count(std::istreambuf_iterator<char>(aFile),
        std::istreambuf_iterator<char>(), '\n');

    totalPages = lines_count % 10 == 0 ? lines_count / 10 : (lines_count / 10 + 1);

    ui.totalPages->setText(QString::number(totalPages));
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
        else
        {
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
}