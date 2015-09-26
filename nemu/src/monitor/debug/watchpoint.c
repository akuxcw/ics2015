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

extern bool flag;

void new_wp(char *args) {
	WP *new_ = free_;
	if (head == NULL) {
		head = new_;
		new_->NO = 1;
	} else {
		WP *tail = head;
		while (tail->next != NULL) tail = tail->next;
		tail->next = new_;
		new_->NO = tail->NO + 1;
	}
	free_ = free_->next;
	new_->next = NULL;
	strcpy(new_->str, args);
	new_->str[strlen(args)] = '\0';
	new_->last_value = expr(args, &flag);
}

void print_wp(int op);

void free_wp(int n) {
	WP *wp = head;
	while (n -- && wp->next != NULL) wp = wp->next;
	WP *tail = free_;
	if(free_ == NULL) {
		free_ = wp;
	} else {
		while (tail->next != NULL) tail = tail->next;
		tail->next = wp;
	}
	printf("[[[%s\n",wp->str);
	printf("**\n");
	WP *p = head;
	if (p != wp) {
		while (p->next != wp) p = p->next;
		p->next = wp->next;
		wp->next = NULL;
		p = p->next;
	} else {
		head = head->next;
		p = head;
	}
	printf("[[[%s\n",p->str);
	while (p != NULL) {
		p->NO --;
		p = p->next;
	}
}

void check_wp(int *nemu_state) {
	WP *wp = head;
	int value = 0;
	while(wp != NULL) {
		value = expr(wp->str, &flag);
		if (value != wp->last_value) {
			printf("The \"%s\"'s value is changed!\n",wp->str);
			wp->last_value = value;
			*nemu_state = 0;
		}
		wp = wp->next;
	}

}

#define DELETE 1

void print_wp(int op) {
	if (head == NULL) {
		if (op != DELETE) printf("There is no watchpoint!\n");
	} else {
		WP *p = head;
		while (p != NULL) {
			printf(p->str[0] == '$'?
					"#%d: %s = 0x%x\n":"#%d: %s = %d\n", p->NO, p->str, p->last_value);
			p = p->next;
		}
	}
}

