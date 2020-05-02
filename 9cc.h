#include <stdbool.h>
#include <ctype.h>

//
// 型定義
//
// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  int len;        // トークンの長さ
};

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // 整数
  ND_EQ, // ==
  ND_NEQ, // !=
  ND_LT, // <
  ND_LTE, // <=
  ND_GT, // >
  ND_GTE, // >=
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind; // ノードの型
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  int val;       // kindがND_NUMの場合のみ使う
};

//
// グローバル変数
//
// 現在着目しているトークン
extern Token *token;
// 入力プログラム
extern char *user_input;


//
// 関数宣言
//
// ===== エラー処理 =====
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
// ===== トークン解析 =====
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);
// ===== 構文解析 =====
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
// ===== 構文木のアセンブル =====
void gen(Node *node);
