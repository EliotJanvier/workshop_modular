/*
** EPITECH PROJECT, 2023
** modular
** File description:
** vector.h
*/

#pragma once

#include "class.h"
#include <stdlib.h>

typedef struct vector_s {
    class_t base;
    void **data;
    int size;
    int capacity;

    void (*push_back)(struct vector_s *self, void *data);
    void *(*pop_back)(struct vector_s *self);
    void *(*at)(struct vector_s *self, int index);
    void (*insert)(struct vector_s *self, int index, void *data);
    void (*erase)(struct vector_s *self, int index);
    void (*clear)(struct vector_s *self);
    void *(*front)(struct vector_s *self);
    void *(*back)(struct vector_s *self);
    void (*foreach)(struct vector_s *self, void (*func)(void *data));
} vector_t;

extern const class_t *vector_t_class;
