﻿#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>

// За функцията shunting_yard съм използвал код от доцент Армянов, но съм го променил
// да работи и за адреси и за повече оператори, също така и get_number го видях от него.
// Неконстантната функция get_cell я видях от асистент Александър Димитров.
// При десериализацията четенето с while циклите го видях от интернет, но не съм сигурен точно откъде


struct Data
{
	int row{};
	int column{};
	std::string expression{};

	Data(int _row = 0, int _column = 0, const std::string _expression = "")
	{
		row = _row;
		column = _column;
		expression = _expression;
	}

	void set_rows_and_columns(int _row, int _column)
	{
		row = _row;
		column = _column;
	}
};

class Table
{
private:
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
	bool is_valid_address(const Data&, size_t&, Data&) const;
	bool is_relative_address(const Data&, size_t&, Data&) const;
	bool is_relative(char) const;
	bool is_absolute_address(const std::string&, size_t&, Data&) const;
	bool apply_operator(std::stack<int>&, const std::string&) const;
	bool shunting_yard(const Data&, int&) const;
	bool if_else_operator(const Data&, size_t&, int&) const;
	bool plus_or_minus_number(const Data&, size_t&, std::stack<int>&) const;

	int get_operator_priority(const std::string&) const;
	int apply(int, int, const std::string&) const;
	int get_sum_or_count(const Data&, const Data&, bool) const;

	const Data* get_cell(int, int) const;
	Data* get_cell(int, int);

	template <class Predicate>
	void print(Predicate predicate) const;

	template <class Predicate>
	void plus_or_minus_one(int, int, Predicate);

public:
	Table() = default;
	
	void set_expression(const Data&);
	void clear_table();

	int get_value(int, int) const;
	const std::string get_expression(int, int) const;
	int get_number(const std::string&, size_t&) const;

	int get_table_rows() const;
	int get_table_columns() const;

	int sum(const Data&, const Data&) const;
	int count(const Data&, const Data&) const;

	void plus_plus(int, int);
	void minus_minus(int, int);

	void print_all_values() const;
	void print_all_expressions() const;

	friend std::ostream& operator<<(std::ostream&, const Table&);
	friend std::istream& operator>>(std::istream&, Table&);

	//for the tests
	void set_table(int, int);
};

