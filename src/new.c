/*
** EPITECH PROJECT, 2023
** modular
** File description:
** new.c
*/

#include "class.h"
#include <stdlib.h>
#include <string.h>

object_t new_object(class_t *base, ...)
{
    object_t new = malloc(base->size);
    va_list args;

    if (!new)
        return (NULL);
    memcpy(new, base, base->size);
    if (base->ctor) {
        va_start(args, base);
        base->ctor(new, &args);
        va_end(args);
    }
    return (new);
}

void delete_object(object_t ptr)
{
    class_t *base = ptr;

    if (base->dtor) {
        base->dtor(ptr);
    }
    free(ptr);
}
