/*
 * ringbuffer.c
 *
 * Implementation of ring buffer functions.
 */


#include "Utils/ringbuffer.h"
#include <assert.h>

#define IS_POWER_OF_TWO(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))


void RingBuffer_Init(ring_buffer_t *rb, uint8_t *buf, size_t buf_size) {
    assert(IS_POWER_OF_TWO(buf_size));
    
    rb->buffer = buf;
    rb->mask   = buf_size - 1;
    rb->head   = 0;
    rb->tail   = 0;
}

void RingBuffer_Queue(ring_buffer_t *rb, uint8_t data) {
    if(RingBuffer_IsFull(rb)) {
        rb->tail = (rb->tail + 1) & rb->mask;
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) & rb->mask;
}

void RingBuffer_Queue_Array(ring_buffer_t *rb, const uint8_t *data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        RingBuffer_Queue(rb, data[i]);
    }
}

bool RingBuffer_Dequeue(ring_buffer_t *rb, uint8_t *data) {
    if(RingBuffer_IsEmpty(rb)) {
        return false;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) & rb->mask;
    
    return true;
}

size_t RingBuffer_Dequeue_Array(ring_buffer_t *rb, uint8_t *data, size_t len) {
    size_t cnt = 0;
    
    while((cnt < len) && RingBuffer_Dequeue(rb, &data[cnt])) {
        cnt++;
    }
    
    return cnt;
}

bool RingBuffer_Peek(ring_buffer_t *rb, uint8_t *data, size_t index) {
    if(index >= RingBuffer_NumItems(rb)) {
        return false;
    }

    size_t data_index = (rb->tail + index) & rb->mask;
    *data = rb->buffer[data_index];
    
    return true;
}
