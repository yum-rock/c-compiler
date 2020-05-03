#include <stdio.h>
#include <stdlib.h>
#include "9cc.h"

// グローバル変数
// 入力プログラム
char *user_input;

int main(int argc, char **argv) {
  if (argc != 2) {
    error("引数の個数が正しくありません");
  }

  // トークナイズしてパースする
  user_input = argv[1];
  tokenize(user_input);
  program();
  codegen();

  return 0;
}
