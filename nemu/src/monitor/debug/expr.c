#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NB, MS, DR

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
	int level;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE, 10},				// spaces
	{"\\+", '+', 1},					// plus
	{"-", '-', 1},						// minus
	{"\\*", '*', 2},					// multiply
	{"/", '/', 2},						// devide
	{"0x[0-9a-fA-F]+|[0-9]+", NB, 10},					// number
	{"==", EQ, 0},						// equal
	{"\\(", '(', 10},					// left par
	{"\\)", ')', 10},					// right par
	{"-", MS, 9},						// minus sign
	{"\\*", DR, 9},						// dereference
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;
	
	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
	int level;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;
	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
			//	printf("*****\n");
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				tokens[nr_token].type = rules[i].token_type;
				tokens[nr_token].level = rules[i].level;
				switch(rules[i].token_type) {
					case '+': case '(': case ')': case '/':	
					case EQ: case MS: case DR: break;
					case '-': 
						if (nr_token == 0 || tokens[nr_token-1].type != NB) {
							tokens[nr_token].type = MS;
							tokens[nr_token].level = 9;
						}
						break;
					case '*':
						if (nr_token == 0 || tokens[nr_token-1].type != NB) {
							tokens[nr_token].type = DR;
							tokens[nr_token].level = 9;
						}
						break;
					case NB: 
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						break;
					case NOTYPE: nr_token --; break;
					default: panic("please implement me");
				}
			//	printf("***%d\n",tokens[nr_token].type);
				nr_token ++;
				break;
			}
		}
		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}

extern bool flag;

bool check_parentheses(int p, int q) {
	if (tokens[p].type != '(' || tokens[q].type != ')') return false;
	int i,num=0;
	for(i = p + 1; i < q; ++ i) {
		if (tokens[i].type == '(') num ++;
		if (tokens[i].type == ')') num --;
		if (num < 0) return false;
	}
	return num == 0;
}

uint32_t select_op(int p, int q) {
	int i, min_level = 10;
	int in_par = 0;
	for(i = p; i <= q; ++ i) {
		if (tokens[i].type == '(') in_par ++;
		if (tokens[i].type == ')') in_par --;		
		if (in_par) continue;
		if (tokens[i].level < min_level) min_level = tokens[i].level;
	}
	for(i = q; i >= p; -- i) {
		if (tokens[i].type == '(') in_par ++;
		if (tokens[i].type == ')') in_par --;		
		if (in_par) continue;
		if (tokens[i].level == min_level) {
			if (tokens[i].level == 9) {
				while(i > 0 && tokens[i-1].level == 9) {
					printf("%d",i);
					i --;
				}
			}
			return i;
		}
	}
//	panic("Can't find op!");
	flag = false;
	return 0;

}

uint32_t eval(p, q) {
	if (!flag) return 0;
	if (p > q) {
//		panic("No number!");
		flag = false;
		return 0;
		/* Bad expression */
	}
	else if (p == q) {
		/* Single token.
		 * For now this token should be a number. 
		 * Return the value of the number.
		 */
		if (tokens[p].type != NB) {
			flag = false;
			return 0;
		}
		int value = 0,i;
//		printf("str=%s\n,value=%d\n",tokens[p].str,value);
		if (strlen(tokens[p].str) < 2 || tokens[p].str[1] != 'x') {
			for(i = 0; i < strlen(tokens[p].str); ++ i) {
				value = value * 10 + tokens[p].str[i] - '0';
			}
		} else {
			for(i = 2; i < strlen(tokens[p].str); ++ i) {
				if(tokens[p].str[i] >= '0' && tokens[p].str[i] <= '9')
					value = value * 16 + tokens[p].str[i] - '0'; 
				if(tokens[p].str[i] >= 'A' && tokens[p].str[i] <= 'F')
					value = value * 16 + tokens[p].str[i] - 'A' + 10; 
				if(tokens[p].str[i] >= 'a' && tokens[p].str[i] <= 'f')
					value = value * 16 + tokens[p].str[i] - 'a' + 10;
			}
		}
//		printf("**********%d************\n",value);
		return value;
	}
	else if(check_parentheses(p, q) == true) {
		/* The expression is surrounded by a matched pair of parentheses. 
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(p + 1, q - 1);
	}
	else {
		int op = select_op(p, q); //the position of dominant operator in the token expression;
		int val1, val2; 
		if (!flag) return 0;
		switch(tokens[op].type) {
			case '+': 
				val1 = eval(p, op - 1); 
				val2 = eval(op + 1, q);
				return val1 + val2;
			case '-':
				val1 = eval(p, op - 1); 
				val2 = eval(op + 1, q);
				return val1 - val2;
			case '*': 
				val1 = eval(p, op - 1); 
				val2 = eval(op + 1, q);
				return val1 * val2;
			case '/': 
				val1 = eval(p, op - 1); 
				val2 = eval(op + 1, q);
				return val1 / val2;
			case EQ:
				val1 = eval(p, op - 1); 
				val2 = eval(op + 1, q);
			 	return 	val1 == val2;
			case MS: 
				val1 = eval(op + 1, q);
				return - val1;
			case DR:
				val1 = eval(op + 1, q);
				return swaddr_read(val1,4);
		default: assert(0);
		}
	//	panic("error");
		return 0;
	}
	return 0;
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	return eval(0,nr_token-1);
	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

