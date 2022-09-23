#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Landmark {
    int x, y;
    void * difficulty;
} Landmark;

typedef struct Route {
    Landmark * landmarks;
    unsigned int (*is_difficult)(Landmark *);
    size_t size;
} Route;

void init_route(Route * route, unsigned int (*is_difficult)(Landmark *))
{
    if (route == NULL || is_difficult == NULL)
    {
        return;
    }
    route->landmarks = NULL;
    route->is_difficult = is_difficult;
    route->size = 0;
}

int add_to_route(Route * route, int x, int y, void * difficulty)
{
	if (route == NULL || difficulty == NULL)
	{
		return EXIT_FAILURE;
	}
	if (route->landmarks == NULL)
	{
		route->landmarks = (Landmark *)calloc(1, sizeof(Landmark));
		if (route->landmarks == NULL)
		{
			return EXIT_FAILURE;
		}
		route->size++;
	}
	else
	{
		route->size++;
		route->landmarks = realloc(route->landmarks, route->size * sizeof(Landmark));
		if (route->landmarks == NULL)
		{
			route->size--;
			return EXIT_FAILURE;
		}
	}

	route->landmarks[(route->size) - 1].x = x;
	route->landmarks[(route->size) - 1].y = y;
	route->landmarks[(route->size) - 1].difficulty = difficulty;

	return EXIT_SUCCESS;
}

void free_route(Route *route) 
{
	if (route == NULL || route->landmarks == NULL)
	{
		return;
	}
	free(route->landmarks);
	route->landmarks = NULL;
	route->size = 0;
}

int is_hard_route(Route * route)
{
	if (route == NULL)
	{
		return -1;
	}
	unsigned int halfOfLandmarks = route->size / 2;
	unsigned int difficultLandmarks = 0;
	for (size_t i = 0; i < route->size; ++i)
	{
		if (route->is_difficult(route->landmarks + i))
		{
			difficultLandmarks++;
		}
	}
	return difficultLandmarks > halfOfLandmarks;

}

void print_route(Route *route)  {
    if (route == NULL || route->landmarks == NULL) {
        return;
    }
    for (size_t i = 0; i < route->size; ++i) {
        printf("%d, %d\n", route->landmarks[i].x, route->landmarks[i].y);
    }
    return;
}