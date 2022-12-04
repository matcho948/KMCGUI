#pragma once
#include <iostream>

class DataTableModel
{
public:
	DataTableModel(std::string kmer, std::string occurences);
	DataTableModel();
	~DataTableModel();
	std::string kmer;
	std::string occurences;
};