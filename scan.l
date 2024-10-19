/* calc.l */
%option noyywrap

%{
/* 引入 calc.y 定义的 token */
/* calc.tab.h 文件由 Bison 生成 */
/* 当我们在.y 文件中使用 %token 声明一个 token 时，这个 token 就会导出到 .h 中，
   可以在 C 代码中直接使用，供 Flex 使用。就如 .l 文件中的\( { return LPAREN; }，
   其中 LPAREN 定义来自 calc.tab.h，由对应的 .y 文件生成 */
#include "par.tab.h"
%}


/* 规则部分 yylval 同样来自 calc.tab.h 文件，其类型为 YYSTYPE，用于 token 的相关属性，比如 NUMBER 对应的实际数值 */
/* 在这个例子中，YYSTYPE 定义如下

typedef union YYSTYPE {
  char op;
  double num;
} YYSTYPE;

其同样由 .y 文件根据 %union 生成，在文件中我们的 %union 定义如下

%union {
    char   op;
    double num;
}
*/

%%
\( { return LPAREN; }
\) { return RPAREN; }
"+"|"-" { yylval.op = yytext[0]; return ADDOP; }
"*"|"/" { yylval.op = yytext[0]; return MULOP; }
[0-9]+|[0-9]+\.[0-9]*|[0-9]*\.[0-9]+ { yylval.num = atof(yytext); return NUMBER; }
" "|\t {  }
\r\n|\n|\r { return RET; }
%%