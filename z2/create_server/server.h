#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

/**
 * @def QUEUE_NAME
 * @brief The name of the main server message queue.
 */
#define QUEUE_NAME "/server_main_"

/**
 * @def CLIENT_NAME
 * @brief The base name for client-specific message queues.
 */
#define CLIENT_NAME "/client_names_"

/**
 * @def MAX_MSG_SIZE
 * @brief Maximum size of a chat message.
 */
#define MAX_MSG_SIZE 256

/**
 * @def MAX_CLIENT
 * @brief Maximum number of clients supported by the server.
 */
#define MAX_CLIENT 10

/**
 * @def WIDTH_NIKNAME
 * @brief Maximum width of a nickname.
 */
#define WIDTH_NIKNAME 40

/**
 * @brief Structure representing a chat message.
 */
typedef struct {
    int id; /**< ID of the client sending the message. */
    char buffer[MAX_MSG_SIZE + 1]; /**< Message buffer. */
} Message;

/**
 * @brief Structure representing an array of client IDs and names.
 */
typedef struct {
    int Id; /**< ID of the client. */
    char Name[WIDTH_NIKNAME + 1]; /**< Name of the client. */
} Array_Id;

/**
 * @brief Duplicate an array element and add a new element.
 * 
 * @param array The array to modify.
 * @param num The index of the element to duplicate.
 * @param count Pointer to the count of elements in the array.
 * @param Name The name of the new element.
 */
void array_duplication(Array_Id* array, int num, int *count, char* Name);

/**
 * @brief Remove a substring from a string.
 * 
 * @param s The string to modify.
 * @param toremove The substring to remove.
 */
void removeSubstring(char *s, const char *toremove);

#endif // SERVER_H
