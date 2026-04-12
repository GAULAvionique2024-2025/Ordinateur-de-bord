/*
 * ringbuffer.h
 *
 * Prototypes and structures for the ring buffer module.
 */

#ifndef INC_GAUL_DRIVERS_LOWLEVEL_RINGBUFFER_H_
#define INC_GAUL_DRIVERS_LOWLEVEL_RINGBUFFER_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


typedef struct {
    uint8_t *buffer;
    size_t  mask;
    size_t  head;
    size_t  tail;
} ring_buffer_t;

void RingBuffer_Init(ring_buffer_t *rb, uint8_t *buf, size_t buf_size);

void RingBuffer_Queue(ring_buffer_t *rb, uint8_t data);
void RingBuffer_Queue_Array(ring_buffer_t *rb, const uint8_t *data, size_t size);

bool RingBuffer_Dequeue(ring_buffer_t *rb, uint8_t *data);
size_t RingBuffer_Dequeue_Array(ring_buffer_t *rb, uint8_t *data, size_t len);

bool RingBuffer_Peek(ring_buffer_t *rb, uint8_t *data, size_t index);

// Inline helper functions
static inline size_t RingBuffer_NumItems(const ring_buffer_t *rb) {
    return (rb->head - rb->tail) & rb->mask;
}

static inline bool RingBuffer_IsEmpty(const ring_buffer_t *rb) {
    return rb->head == rb->tail;
}

static inline bool RingBuffer_IsFull(const ring_buffer_t *rb) {
    return RingBuffer_NumItems(rb) == rb->mask;
}

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_RINGBUFFER_H_ */
