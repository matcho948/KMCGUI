#pragma once

#include <QObject>
#include <QtWidgets/QMainWindow>


class DataTable  : public QMainWindow
{
	Q_OBJECT

public:
	DataTable(QWidget* parent);
	DataTable();
	~DataTable();
};
