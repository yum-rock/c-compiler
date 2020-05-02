#include <stdlib.h>
#include <string.h>
#include "9cc.h"

// ===== トークン解析 =====
// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // 空白文字をスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (strncmp(p, "<=", strlen("<=")) == 0) {
      cur = new_token(TK_RESERVED, cur, p, strlen("<="));
      p += strlen("<=");
      continue;      
    }

    if (strncmp(p, ">=", strlen(">=")) == 0) {
      cur = new_token(TK_RESERVED, cur, p, strlen(">="));
      p += strlen(">=");
      continue;      
    }

    if (strncmp(p, "!=", strlen("!=")) == 0) {
      cur = new_token(TK_RESERVED, cur, p, strlen("!="));
      p += strlen("!=");
      continue;      
    }

    if (strncmp(p, "==", strlen("==")) == 0) {
      cur = new_token(TK_RESERVED, cur, p, strlen("=="));
      p += strlen("==");
      continue;      
    }

    if (strchr("+-*/()<>", *p)) {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p, 0);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}

// ===== 構文解析 =====
// 再帰下降構文解析法: EBNFを用いた文法の生成規則
// 加減算(+-)
Node *expr() {
  Node *node = equality();
  return node;
}

Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_node(ND_EQ, node, relational());
    else if (consume("!="))
      node = new_node(ND_NEQ, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume("<"))
      node = new_node(ND_LT, node, add());
    else if (consume("<="))
      node = new_node(ND_LTE, node, add());
    else if (consume(">"))
      node = new_node(ND_GT, node, add());
    else if (consume(">="))
      node = new_node(ND_GTE, node, add());
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

// 乗除算(*/)
Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

// 単項演算子(+-)
Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

// 要素
Node *primary() {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number());
}