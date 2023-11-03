/*
** EPITECH PROJECT, 2023
** modular
** File description:
** main.c
*/

#include <stdio.h>
#include "class.h"
#include "vector.h"

void put_str(void *data)
{
    printf("%s", (char *)data);
}

int main(void)
{
    vector_t *vec = new(vector_t, 0);

    vec->push_back(vec, "Hello");
    vec->push_back(vec, " World");
    vec->push_back(vec, " !\n");
    vec->foreach(vec, &put_str);
    delete(vec);
    return 0;
}
