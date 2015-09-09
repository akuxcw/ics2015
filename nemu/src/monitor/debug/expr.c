#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NB

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"-", '-'},
	{"\\*", '*'},
	{"/", '/'},
	{"[0-9]+", NB},
	{"	==", EQ},						// equal
	{"\\(", '('},
	{"\\)", ')'},

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

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				tokens[nr_token].type = rules[i].token_type;
				switch(rules[i].token_type) {
					case NOTYPE:
					case '+': case '-': case '*': case '/':
					case EQ:
					case '(':
					case ')':
					case NB: strncpy(tokens[nr_token].str, substr_start, substr_len);
							 break;

					default: panic("please implement me");
				}
				nr_token ++;
//				printf("****************%s****************\n", tokens[nr_token-1].str);
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

bool check_parentheses(int p, int q) {
	if (tokens[p].type != '(' || tokens[q].type != ')') return false;
	int i,num=0;
	for(i = p + 1; i < q; ++ i) {
		if (tokens[i].type == '(') num ++;
		if (tokens[i].type == ')') num --;
		if (num < 0) panic("%d",num);//return false;
	}
	return num == 0;
}

uint32_t eval(p, q) {
	if (p > q) {
		/* Bad expression */
	}
	else if (p == q) {
		/* Single token.
		 * For now this token should be a number. 
		 * Return the value of the number.
		 */ 
		int value = 0,i;
		for(i = 0; i < strlen(tokens[p].str); ++ i) {
			value = value * 10 + tokens[p].str[i] - '0';
		}
		return value;
	}
	else if(check_parentheses(p, q) == true) {
		/* The expression is surrounded by a matched pair of parentheses. 
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(p + 1, q - 1); 
	}
	else {
//		op = the position of dominant operator in the token expression;
//		val1 = eval(p, op - 1);
//		val2 = eval(op + 1, q);
//		switch(op_type) {
//			case '+': return val1 + val2;
//			case '-': /* ... */
//			case '*': /* ... */
//			case '/': /* ... */
//		default: assert(0);
//		}
		panic("error");
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

