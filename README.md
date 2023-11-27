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

Take the vehicule, car and truck classes defined above, and write a constructor that creates a car
from a vehicule given in argument.
```C
car_t *car_ctor(vehicule_t *base);
```
Do the same to create a truck.
```C
truck_t *truck_ctor(vehicule_t *base);
```

## End of the intro, let's get real.

Now that you have understood the basics, lets do it for real. We want to be able to
store anything in our structures, not just cars and trucks.
Just like the vehicule_t struct, we will define a struct called class_t which
will be the base for all our objects.

```C
typedef void *object_t; // this can hold any pointer

typedef void (*ctor_t)(object_t *self, va_list *args); // remember the ... in printf?
typedef void (*dtor_t)(object_t *self);

typedef struct class_base {
    char *name;
    size_t size;
    ctor_t ctor;
    dtor_t dtor;
} class_t;
```
Here, name is the name of the class, size is the size of the instances of this class,
and ctor and dtor are pointer functions to class specifics constructors and destructors.

- Adapt the constructors of your car and truck structure so that they have the same
prototype as ctor_t.
- Redefine the car and truck structs so that they are based on the class_t instead of the vehicule_t. Then, create two ```static const``` variables called ```car_default_instance``` and ```truck_default_instance``` that will represent the default, unititialized state of the car and truck classes. Fill in the base with the values that best describe your two new classes.

*Hint* If you don't understand from now on, you can look at the exemple in vector.c to see how it is done.

When that is done, create a pointer of type ```class_t``` called ```car_t_class``` and give it as a value the address of the ```car_default_instance``` variable. If you did everything correctly, you should be able to use the ```new``` and ```delete``` macros given in class.h to create both
instances of cars and trucks.

## A Deeper look into ```new``` and ```delete```
```new``` and ```delete``` are defined in the header class.h.
New can be called to allocate and initialize any struct that is based on the class_t struct.
It calls the function ```new_object``` defined in new.c. Lets break it down:

```C
#include <string.h>

// @param class_t *base: the typename_class pointer to the default instance of the class to construct.
// @param: ...: it takes a variadic number of arguments to accomodate any needs that the class to build may have.
object_t new_object(class_t *base, ...)
{
    object_t new = malloc(base->size); // the class_t base should contain the size to malloc for creating an instance of its type
    va_list args;

    if (!new)
        return (NULL);
    memcpy(new, base, base->size);  // copy the default instance of the class so that the defaults members are initialized the same for all instances of the same class
    if (base->ctor) { // if the class has a custom constructor, call it
        va_start(args, base);
        base->ctor(new, &args); // we call it with the variadic arguments
        va_end(args);
    }
    return (new); // we return the created instance
}
```
Delete will free the object. It will call the custom constructor if there is any need to free members of the class.
```C
void delete_object(object_t ptr)
{
    class_t *base = ptr; // we cast the pointer to a class_t for more readability

    if (base->dtor) { // if the destructor exists, we call it
        base->dtor(ptr);
    }
    free(ptr);
}
```

Now you should be able to understand the vector class given in the subject!

## A practical use case
Lets code a small game using what we learned.
This game will be very simple: the user can specify a number through the command line when starting the program, and the game will display this number of circles. Using the WASD keys, the user will be able to move all the circles on the screen.
Every object in the game (meaning every circle) will be represented by a 'gameobject'. Every functionality of the object is represented by what we call a component: the image (sprite) is a component, and the controller that moves it following the keys of the keyboard is another one.

In the main, open an SFML window and write its main loop. Create the following classes:
- ```component_t```, a class with a function pointer called update, of type ```void (*)(component_t *, gameobject_t *)```. All components should be created with a reference to the renderwindow, and to the array in the main containing all components.
- ```gameobject_t```, a class with an array of ```component_t``` and a method called ```void update_components(gameobject_t *)```. This method should call ```update``` on all components stored in the gameobject. Also add a method called ```void add_component(gameobject_t *, component_t *)``` that adds a component to the array of components of the gameobject.
- ```circle_t```, a class based on the ```component_t``` (just like cars were based on the vehicules!) with a method ```void draw(component_t *, gameobject_t *)``` which draws a circle on screen. The default instance of the circle should set the ```update``` method of its base component to its ```draw``` method.
- ```circle_controller_t```, a class based on the ```component_t```, and that sets the base ```update``` method to ```void take_input(component_t *, gameobject_t *)```. This method takes input from the keyboard and moves the circles present on the given gameobject.

Now add a for loop in the main loop, to call ```update_components``` on each gameobject in the game.

You can now add some gameobjects containing a circle and a circle controller to your program.
Do you see how easy it will be to add more functionalities? Try to make the game more interactive and fun! (Some ideas: make the circles chase the mouse and kill the player if they touch it, make the circles shoot at each other to destroy the others...).
You can use the ```vector``` class given in this repository to go faster and create objects dynamically.

*Note* Your main *could* look something like this:

```C
int main(int ac, char **av) {
    if (ac != 2) { // some error handling
        return 84;
    }

    unsigned n_objects = atoi(av[1]); // how many circles?
    gameobject_t *objects = malloc(sizeof(gameobjects_t *) * n_objects); // allocing space for the gameobjects

    sfVideoMode mode = {800, 600, 32}; // small window
    sfRenderWindow* window;
    window = sfRenderWindow_create(mode, "Modular is life", sfResize | sfClose, NULL);

    if (!window) // more error handling
        return 84;

    for (unsigned i = 0; i < n_objects; ++i) {
        objects[i] = new(gameobject_t); // this mallocs the objects themselves
        objects[i]->add_component(objects[i], (component_t *) new(circle_t, rand() % 100, rand() % 100, 5)); // just a simple circle at a random position, with a radius of five
        objects[i]->add_component(objects[i], (component_t *) new(circle_controller_t)); // adding the script that will make this circle move
    }

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) { // you know this by know dont you?
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, sfBlack); // we clear the window here. Objects should redraw themselves every frame!
        for (unsigned i = 0; i < n_objects; ++i) {
            objects[i]->update_components(objects[i]); // the core of the game is here.
        }
        sfRenderWindow_display(window);
    }

    for (int i = 0; i < n_objects; ++i) {
        delete(objects[i]); // this should destroy the components automatically too!
    }
    sfRenderWindow_destroy(window);
    return 0;

}
```
