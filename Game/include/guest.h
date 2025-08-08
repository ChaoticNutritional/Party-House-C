#pragma once

typedef struct guest_t Guest;
typedef struct guestInstance_t GuestInstance;

void guestInitTextures();
Guest* guestNew();
void guestDelete(Guest* guest);