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

		Data(int _row, int _column, const std::string _expression = "")
		{
			row = _row;
			column = _column;
			expression = _expression;
		}
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
	int get_value_from_address(const std::string&, size_t&) const;
	int get_sum_or_count(const Data&, const Data&, int) const;

	const Data* get_cell(int, int) const;
	Data* get_cell(int, int);

	template <class Predicate>
	void print(Predicate predicate) const;
public:
	Table() = default;
	
	void set_expression(const Data&);
	void plus_or_minus_one(int, int, char);
	void set_rows_and_columns(int,int);

	int get_value(int, int) const;
	const std::string& get_expression(int, int) const;

	int get_table_rows() const;
	int get_table_columns() const;

	int sum(const Data&, const Data&) const;
	int count(const Data&, const Data&) const;

	void print_all_values() const;
	void print_all_expressions() const;
};

template<class Predicate>
void Table::print(Predicate predicate) const
{
	for (size_t i = 0; i < max_rows; ++i)
	{
		for (size_t j = 0; j < max_columns; ++j)
		{
			std::cout << predicate(i, j) << '\t';
		}
		std::cout << std::endl;
	}
}