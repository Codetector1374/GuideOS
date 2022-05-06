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

void dequeue_iterator(dequeue_t* q, dequeue_iterator_t* iter) {
  iter->current_node = q->head;
  iter->q = q;
}

void* dequeue_iterator_next(dequeue_iterator_t* iter) {
  void* data = NULL;
  if (iter->current_node != NULL) {
    data = iter->current_node->data;
    iter->current_node = iter->current_node->next;
  }
  return data;
}

bool dequeue_iterator_has_next(dequeue_iterator_t* iter) {
  return iter->current_node != NULL;
}

void dequeue_iterator_delete(dequeue_iterator_t* iter) {
  if (iter->q->size == 0) {
    return;
  }
  if (iter->q->size == 1) {
    (void)dequeue_pop_front(iter->q);
    return;
  }
  struct dequeue_node* target_node = NULL;
  if (iter->current_node == NULL) {
    target_node = iter->q->tail;
  } else {
    target_node = iter->current_node->prev;
  }
  if (target_node != NULL) {
    // Not head
    if (target_node->prev != NULL) {
      target_node->prev->next = target_node->next;
    } else {
      iter->q->head = target_node->next;
    }

    // Not Tail
    if (target_node->next != NULL) {
      target_node->next->prev = target_node->prev;
    } else {
      iter->q->tail = target_node->prev;
    }
    target_node->next = NULL;
    target_node->prev = NULL;
    iter->q->size--;
  }
}
