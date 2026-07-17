#include "Guest.h"

#include <stdlib.h>
#include <assert.h>

guest* guestNew(const guest_data* data)
{
    guest* g = malloc(sizeof(guest));
    assert(g != NULL);
    g->data = data;
    return g;
}

void guestDelete(guest* g)
{
    free(g);
}
