#pragma once

#include <QObject>
#include <QtWidgets/QMainWindow>
#include "ui_DataTable.h"

class DataTable : public QMainWindow
{
	Q_OBJECT

public:
	DataTable(QWidget* parent);
	DataTable();
	~DataTable();
	void showDataTable();
	std::string resultFileName;
	Ui::MainWindow ui;
public slots:
	void on_previousPageButton_clicked();
	void on_nextPageButton_clicked();
	void on_goButton_clicked();
private:
	int currentPage;
	int totalPages;
};
