/*
** EPITECH PROJECT, 2023
** modular
** File description:
** vector.c
*/

#include "vector.h"

static void vector_ctor(object_t *s, va_list *args)
{
    vector_t *self = (vector_t *)s;

    self->capacity = va_arg(*args, size_t);
    self->size = 0;
    self->data = malloc(sizeof(void *) * self->capacity);
    if (!self->data)
        return;
    for (size_t i = 0; i < self->capacity; i++)
        self->data[i] = NULL;
}

static void vector_dtor(object_t *s)
{
    vector_t *self = (vector_t *)s;

    if (self->data)
        free(self->data);
}

static void vector_push_back(vector_t *self, void *data)
{
    if (!self->data) {
        self->capacity = 1;
        self->size = 0;
        self->data = malloc(sizeof(void *) * self->capacity);
        if (!self->data)
            return;
    }
    if (self->size == self->capacity) {
        if (self->capacity == 0)
            self->capacity = 1;
        self->capacity *= 2;
        self->data = realloc(self->data, sizeof(void *) * self->capacity);
        if (!self->data) {
            return;
        }
    }
    self->data[self->size++] = data;
}

static void *vector_pop_back(vector_t *self)
{
    if (!self->size)
        return (NULL);
    return (self->data[--self->size]);
}

static void *vector_front(vector_t *self)
{
    if (!self->size)
        return (NULL);
    return (self->data[0]);
}

static void *vector_back(vector_t *self)
{
    if (!self->size)
        return (NULL);
    return (self->data[self->size - 1]);
}

static void *vector_at(vector_t *self, int index)
{
    if (index >= self->size)
        return (NULL);
    return (self->data[index]);
}

static void vector_insert(vector_t *self, int index, void *data)
{
    if (index > self->size)
        return;
    if (self->size == self->capacity) {
        self->capacity *= 2;
        self->data = realloc(self->data, sizeof(void *) * self->capacity);
        if (!self->data)
            return;
    }
    for (size_t i = self->size; i > index; i--)
        self->data[i] = self->data[i - 1];
    self->data[index] = data;
    self->size++;
}

static void vector_erase(vector_t *self, int index)
{
    if (index >= self->size)
        return;
    for (size_t i = index; i < self->size - 1; i++)
        self->data[i] = self->data[i + 1];
    self->size--;
}

static void vector_clear(vector_t *self)
{
    for (size_t i = 0; i < self->size; i++)
        free(self->data[i]);
    self->size = 0;
}

static void vector_foreach(vector_t *self, void (*func)(void *))
{
    for (size_t i = 0; i < self->size; i++)
        func(self->data[i]);
}

static const vector_t vector_default_instance = {
    .base = {
        .name = "vector",
        .size = sizeof(vector_t),
        .ctor = (ctor_t)&vector_ctor,
        .dtor = (dtor_t)&vector_dtor
    },
    .size = 0,
    .capacity = 0,
    .data = NULL,
    .push_back = &vector_push_back,
    .pop_back = &vector_pop_back,
    .at = &vector_at,
    .insert = &vector_insert,
    .erase = &vector_erase,
    .clear = &vector_clear,
    .front = &vector_front,
    .back = &vector_back,
    .foreach = &vector_foreach
};

const class_t *vector_t_class = (class_t *)&vector_default_instance;
