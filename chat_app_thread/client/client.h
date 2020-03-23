#include "shared.h"

int sfd;
int friend_connected;

void *client_in_handler(void *arg);
void *client_out_handler(void *arg);
