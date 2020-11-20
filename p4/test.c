#include <stdlib.h>
#include <stdio.h>

#include "page_table.h"

int main() {
	PageT* page_t = create_page_t();
	char *pid = NULL;
	char *vpn = NULL;
	int count = 0;
	for (int i = 0; i < 5; i++) {		
		for (int j = 0; j < 5; j++) {
			pid = malloc(2);
			vpn = malloc(2);
			sprintf(pid, "%d", i);
			sprintf(vpn, "%d", j);
			add_page_te(page_t, pid, vpn, count++);
			free(pid);
			free(vpn);
		}
	}
	delete_page_te(page_t, "1", "2");
	delete_page_te(page_t, "4", "4");
	delete_page_te(page_t, "9", "9");
	print_page_t(page_t);
	clear_page_t(page_t);
}
