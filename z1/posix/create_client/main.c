#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>

#define MSG_SIZE 128
#define QUEUE_NAME "/my_queue"

char msg_text[MSG_SIZE];

int main() {
    mqd_t mq_server;

    // Открытие очереди сервера для чтения
    mq_server = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_server == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Отправка сообщения серверу
    strncpy(msg_text, "Hi!", sizeof(msg_text) - 2);
    msg_text[sizeof(msg_text) - 2] = '\0';
    if (mq_send(mq_server, (const char *)&msg_text, sizeof(msg_text), 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    // Закрытие очереди сервера
    if (mq_close(mq_server) == -1) {
        perror("mq_close");
        exit(1);
    }

    mqd_t mq_client;
    unsigned int prio;

    // Открытие очереди клиента для чтения
    mq_client = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq_client == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Получение ответа от сервера
    if (mq_receive(mq_client, msg_text, MSG_SIZE, &prio) == -1) {
        perror("mq_receive");
        exit(1);
    }

    printf("Received message from server: %s\n", msg_text);

    // Закрытие очереди клиента
    if (mq_close(mq_client) == -1) {
        perror("mq_close");
        exit(1);
    }


    return 0;
}
