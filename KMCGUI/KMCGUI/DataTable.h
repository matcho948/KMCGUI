#pragma once

#include <QObject>
#include <QtWidgets/QMainWindow>
#include "ui_DataTable.h"

class DataTable  : public QMainWindow
{
	Q_OBJECT

public:
	DataTable(QWidget* parent);
	DataTable();
	~DataTable();
	void showDataTable();
	std::string resultFileName; 
	Ui::MainWindow ui;
};
