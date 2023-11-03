# Modular C! Or C for GigaChads

## What is modular C?

Modular C is a way to structure data to mimic what is done in Object Oriented
Programming.

*Object Oriented Programming*: A programming paradigm where funcionnalities
are encapsulated into objects in order to abstract concepts.

This has many advantages compared to classical C:
- It make structures more powerful and standardises the way they are treated
- It enables *polymorphisme*, or the possibility to write generic code. For exemple,
  modular C renders the following code feasible:

```C
car_t *car;
truck_t *truc;
//...
car->honk(car);
truck->move(truck);
throw_in_the_sky(car);
throw_in_the_sky(truck);
```
In the code sample above, you can see that:
- there are two objects, representing a car and a truck
- both of these objects have *member functions* that can be called just like
  normal functions, with the difference that they are accessible only through
  their object.
- the throw_in_the_sky can take both a car_t and a truck_t as argument, and treats
  them the same way. This is not normally possible in classical C without
  passing additionnal parameters to define the type of the objects.

With modular C, it is not necessary to keep track of the type of objects because
they contain their own functionalities, and a lot of work can be abstracted in order
to reduce the quantity of code.

Lets see how to do this.

## Structures and memory alignment

Lets start with the concept of memory alignement. Take the following structure:

```C
struct foo_s {
    int x;
    int y;
};

struct foo_s foo;
```

The important idea here is that the two following statements give the exact same result:
```C
    &foo;
    &(foo.x);
```
This works only because foo.x is the first member of the struture, thus they begin at
the same place in memory.
Why is that important? This makes possible to use pointers to both foo.x and
foo as if they were the same object. Note that they are not of the same type
(foo is of type ``` struct foo_s ``` and foo.x is of type ``` int ```), but because
they have the same address they can be used interchangeably.

Now take this more concrete exemple:

```C
typedef struct vehicule_s {
    int weight;
    void (*move)(vehicule_t *self, int distance);
} vehicule_t;

typedef struct car_s {
    vehicule_t base;
    void (*honk)(truck_t *self, void);
} car_t;

typedef struct truck_s {
    vehicule_t base;

    int charge;
    void (*load)(truck_t *self, int weight);
} truck_t;

vehicule_t *car; // actually a car
vehicule_t *truck; // actually a truck

// initialize the variables here
//...

car->move(car, 3);
truck->move(truck, 2);

((car_t *) car)->honk(car);
((truck_t *truck))->load(truck, 20);

```
You see that it is possible to treat both objects as vehicule, while we retain
the possibility to use their special characteristics. Take a moment to understand
how this works.

## You go now!

Take the vehicule, car and truck classes defined above, and write
```C
car_t *new_car()
```

