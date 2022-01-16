#include "Table.h"

int main()
{
	Table table;
	table.set_rows_and_columns(3, 4);
	table.set_expression({ 1,2,"5+4" });
	table.set_expression({ 0,0,"5+R1C2" });
	table.set_expression({ 0,2,"6*2" });
	table.set_expression({ 2,2,"R1C2==R0C2" });
	table.set_expression({ 1,3,"R2C2?4:5" });

	table.print_all_values();
}