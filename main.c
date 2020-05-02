#include "9cc.h"

// グローバル変数
// 現在着目しているトークン
Token *token;
// 入力プログラム
char *user_input;

int main(int argc, char **argv) {
  if (argc != 2) {
    error("引数の個数が正しくありません");
  }

  // トークナイズしてパースする
  user_input = argv[1];
  token = tokenize(user_input);
  Node *node = expr();

  // 抽象構文木を下りながらコード生成
  gen(node);

  return 0;
}
