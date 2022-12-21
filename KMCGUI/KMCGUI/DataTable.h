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
	void on_searchButton_clicked();
private:
	void showFirstOrLastPage();
	void showPage();
	int currentPage;
	int totalPages;
};
