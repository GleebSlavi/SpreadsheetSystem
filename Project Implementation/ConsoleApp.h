#pragma once
#include "Table.h"
#include <fstream>

class ConsoleApp
{
private:
	Table table;
	bool is_loaded = false;

	void get_stream_strings(const std::string&, std::string&, std::string&, std::string&) const;
	bool row_and_column_check(int&, const std::string&,size_t&) const;
	bool csv_file_check(const std::string&) const;
	bool get_and_check_address(Data&, const std::string&) const;

	void save(const std::string&) const;
	void load(const std::string&);
	void help() const;
	void operators() const;
	void exit();

	template <class FirstFunc, class SecondFunc >
	bool print_command(const std::string&, FirstFunc, SecondFunc) const;
public:
	void app();
	void print();
};