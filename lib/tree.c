#pragma once

#include <stdlib.h>
#include <sys/types.h>

typedef struct Node {
  void * data;
  uint length;
  struct Node * parent;
  struct Node ** children;
} Node;

Node * NodeNew(void * data, int cq) {
  Node * n = malloc(sizeof(Node));
  
  n->data = data;
  n->children = malloc(sizeof(Node) * cq);
  n->parent = NULL;
  n->length = 0;

  return n;
}

Node * NodeNewChild(Node * node, void * data, int cq) {
  Node * c = malloc(sizeof(Node));
  
  node->length++;
  node->children[node->length - 1] = c;

  c->data = data;
  c->parent = node;
  c->children = malloc(sizeof(Node) * cq);

  return c;
}

void TreeFree(Node * node) {
  for (int i = 0; i < node->length; i++)
    TreeFree(node->children[i]);

  free(node->data);
  node->data = NULL;

  free(node);
  node = NULL;
}
