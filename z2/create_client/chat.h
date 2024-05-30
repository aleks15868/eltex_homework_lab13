/**
 * @file chat.h
 * @brief Header file containing definitions and structures for the chat program.
 */
#ifndef CHAT_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <ctype.h>
#include <ncurses.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

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
 * @def MAX_MESSAGE_SIZE
 * @brief Maximum size of a chat message.
 */
#define MAX_MESSAGE_SIZE 256

/**
 * @def MAX_CLENT
 * @brief Maximum number of clients supported by the server.
 */
#define MAX_CLENT 10

/**
 * @def COLOR_DARK_GREEN
 * @brief Color code for dark green.
 */
#define COLOR_DARK_GREEN 10

/**
 * @def COLOR_WHITE_GREEN
 * @brief Color code for white green.
 */
#define COLOR_WHITE_GREEN 11

/**
 * @def BACKGROUND_COLOR_DARK_GREEN
 * @brief Background color code for dark green.
 */
#define BACKGROUND_COLOR_DARK_GREEN 1

/**
 * @def BACKGROUND_COLOR_WHITE_GREEN
 * @brief Background color code for white green.
 */
#define BACKGROUND_COLOR_WHITE_GREEN 2

/**
 * @def WIDTH_NIKNAME
 * @brief Maximum width of a nickname.
 */
#define WIDTH_NIKNAME 40

/**
 * @def WIDTH_NIKNAME_GRAPHICALLY
 * @brief Maximum width of a nickname, considering graphical characters.
 */
#define WIDTH_NIKNAME_GRAPHICALLY 24

/**
 * @def MAX_MSG_COUNT
 * @brief Maximum number of messages to display in the chat window.
 */
#define MAX_MSG_COUNT 25

/**
 * @brief Structure representing the maximum size of the chat window.
 */
typedef struct {
    int width; /**< Width of the chat window. */
    int height; /**< Height of the chat window. */
} MaxSize;

/**
 * @brief Structure representing parameters for a chat thread.
 */
typedef struct {
    MaxSize size; /**< Size of the chat window. */
    mqd_t mq; /**< Message queue descriptor. */
    int Gap; /**< Gap size in the chat window. */
    int exit_programm; /**< Flag indicating if the program should exit. */
    char Input; /**< Input character for the chat thread. */
    int id; /**< ID of the client. */
    char Name[WIDTH_NIKNAME + 1]; /**< Nickname of the client. */
    struct mq_attr attr; /**< Attributes of the message queue. */
} parametr_thread;

/**
 * @brief Structure representing a chat message.
 */
typedef struct {
    int id; /**< ID of the client sending the message. */
    char buffer[MAX_MESSAGE_SIZE + 1]; /**< Message buffer. */
} Message;
/**
 * @brief Display messages in the chat window.
 * 
 * @param [in] param parametr_thread, Pointer to the parameter structure of the thread.
 * @return NULL
 */
void print_all_chat(parametr_thread* param);
/**
 * @brief Display messages in the chat window.
 * 
 * @param [in] param parametr_thread, Pointer to the parameter structure of the thread.
 * @return NULL
 */
void print_window_input_name(parametr_thread* param);
/**
 * @brief Remove a substring from a string.
 * 
 * @param [in, out] s Char, The string to modify.
 * @param [in] toremove Char, The substring to remove.
 */
void removeSubstring(char *s, const char *toremove);

/**
 * @brief Display messages in the chat window.
 * 
 * @param [in] arg parametr_thread, Pointer to the parameter structure of the thread.
 * @return NULL
 */
void* display_thread(void* arg);

/**
 * @brief Handle user input in the chat window.
 * 
 * @param [in] arg parametr_thread, Pointer to the parameter structure of the thread.
 * @return NULL
 */
void* input_thread(void* arg);

/**
 * @brief Process incoming messages from other clients.
 * 
 * @param [in] arg parametr_thread, Pointer to the parameter structure of the thread.
 * @return NULL
 */
void* message_processor_thread(void* arg);

#endif // COLORS_H

