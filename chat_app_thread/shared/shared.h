#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gmodule.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/ipc.h>

#define MAXLINE 1024
#define PORT 8080
#define CONTROL_MESSAGE "CONTROL_MESSAGE"
#define NAME_REQUEST "NAME_REQUEST"
#define FRND_REQ "FRND_REQ"
#define FRIEND_RESPONSE_SUCCESS "FRIEND_RESPONSE_SUCCESS"
#define FRIEND_RESPONSE_FAILURE "FRIEND_RESPONSE_FAILURE"
#define LIST_REQ "LIST_REQ"
#define LIST_RESPONSE "LIST_RESPONSE"
#define FRIEND_OFFLINE "FRIEND_OFFLINE"
#define EXIT_CHAT "EXIT_CHAT"
