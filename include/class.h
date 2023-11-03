/*
** EPITECH PROJECT, 2023
** modular
** File description:
** class.h
*/

#pragma once

#include <stdarg.h>
#include <stddef.h>

typedef void *object_t;

typedef void (*ctor_t)(object_t *self, va_list *args);
typedef void (*dtor_t)(object_t *self);

typedef struct class_base {
    char *name;
    size_t size;
    ctor_t ctor;
    dtor_t dtor;
} class_t;

#define new(type, ...) ((type *)new_object((class_t *) type##_class, ##__VA_ARGS__))
#define delete(obj) delete_object(obj)

object_t new_object(class_t *c, ...);
void delete_object(object_t obj);
