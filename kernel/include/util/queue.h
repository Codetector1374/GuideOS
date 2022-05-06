#pragma once

#include "types.h"

typedef struct dequeue_iterator {
  struct dequeue *q;
  struct dequeue_node* current_node;
} dequeue_iterator_t;

typedef struct dequeue {
  size_t size;
  struct dequeue_node* head;
  struct dequeue_node* tail;
} dequeue_t;

struct dequeue_node {
  void* data;
  struct dequeue_node* next;
  struct dequeue_node* prev;
};

void dequeue_construct(dequeue_t* q);
void dequeue_push_back(dequeue_t* q, void* item);
void dequeue_push_front(dequeue_t* q, void* item);
void* dequeue_peek_front(dequeue_t* q);
void* dequeue_pop_front(dequeue_t* q);
void* dequeue_pop_back(dequeue_t* q);

void dequeue_iterator(dequeue_t* q, dequeue_iterator_t* iter);
void* dequeue_iterator_next(dequeue_iterator_t* iter);
bool dequeue_iterator_has_next(dequeue_iterator_t* iter);
void dequeue_iterator_delete(dequeue_iterator_t* iter);
