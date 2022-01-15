#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>

class Table
{
private:
	struct Data
	{
		int row{};
		int column{};
		std::string expression{};
	};
	int max_rows{};
	int max_columns{};
	std::vector<std::vector<Data>> table{ max_rows };

	bool is_digit(char) const;
	bool is_open_bracket(char) const;
	bool is_close_bracket(char) const;
	bool is_bool_operator(char) const;
	bool is_arithmetic_operator(char) const;
	bool is_operator(char) const;
	bool matching_brackets(char, char) const;

	int get_number(const std::string&, size_t&) const;
	int get_operation_priority(const std::string&) const;
	void apply_operator(std::stack<int>&, const std::string&) const;
	int apply(int, int, const std::string&) const;
	int shunting_yard(const std::string&) const;

	const Data* get_cell(int, int) const;
	Data* get_cell(int, int);
public:
	Table() = default;
	Table(int, int);

	void set_expression(const Data&);
	int get_value(int, int) const;
	void plus_or_minus_one(int, int, char);

	int get_table_rows() const;
	int get_table_columns() const;

	
};

