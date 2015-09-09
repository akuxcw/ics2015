#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

void new_wp(char *args) {
	WP *new_ = free_;
	if (head == NULL) {
		head = new_;
		new_->NO = 1;
	} else {
		WP *tail = head;
		while (tail->next) tail = tail->next;
		tail->next = new_;
		new_->NO = tail->NO + 1;
	}
	free_ = free_->next;
	new_->next = NULL;
	strcpy(new_->str, args);
	new_->str[strlen(args)] = '\0';
}


