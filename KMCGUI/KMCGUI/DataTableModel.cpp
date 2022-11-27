#include "DataTableModel.h"

DataTableModel::DataTableModel()
{}

DataTableModel::DataTableModel(std::string kMer, std::string amountOfOccurences)
{
	occurences = amountOfOccurences;
	kmer = kMer;
}

DataTableModel::~DataTableModel()
{}