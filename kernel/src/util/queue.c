#include "defs.h"
#include "util/queue.h"

void dequeue_construct(dequeue_t *q) {
  if (q == NULL) {
    return;
  }
  memset(q, 0, sizeof(dequeue_t));
}

void dequeue_push_back(dequeue_t *q, void* item) {
  struct dequeue_node* node = kmalloc(sizeof(struct dequeue_node));
  node->data = item;
  node->next = NULL;
  if (q->size == 0) {
    node->prev = NULL;
    q->head = node;
  } else {
    node->prev = q->tail;
    q->tail->next = node;
  }
  q->tail = node;
  q->size++;
}

void dequeue_push_front(dequeue_t *q, void* item) {
  if (q->size == 0) {
    dequeue_push_front(q, item);
    return;
  }
  struct dequeue_node* node = kmalloc(sizeof(struct dequeue_node));
  node->data = item;
  node->prev = NULL;
  node->next = q->head;
  q->head->prev = node;
  q->head = node;
  q->size++;
}

void* dequeue_peek_front(dequeue_t* q) {
  if (q->size == 0) {
    return NULL;
  }
  return q->head->data;
}

void* dequeue_pop_front(dequeue_t* q) {
  if (q->size == 0) {
    return NULL;
  }
  struct dequeue_node *node = q->head;
  q->head = q->head->next;

  if (--q->size == 0) {
    q->tail = NULL;
  } else {
    if (q->head == NULL) {
      panic("null head size > 0?");
    }
    q->head->prev = NULL;
  }
  void* data = node->data;
  kfree(node);

  return data;
}

void* dequeue_pop_back(dequeue_t* q) {
  panic("not implemented");
  return NULL;
}
