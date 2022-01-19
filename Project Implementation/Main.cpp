#include "ConsoleApp.h"

int main()
{
	Table table;
	table.set_table(2, 3);
	table.set_expression({ 0, 0, "5 = 3" });
	std::cout << table.get_value(0,0);
}