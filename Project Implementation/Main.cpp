#include "Table.h"

int main()
{
	Table table;
	table.set_table(3, 3);

	table.set_expression({ 0,0,"-1" });
	table.set_expression({ 1,2,"4*(-2)" });
	std::cout << table.get_value(1, 2) << std::endl;

	return 0;
}