#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>

#define MSG_SIZE 128
#define QUEUE_NAME "/my_queue"

int main() {
    mqd_t mq_server;
    mqd_t mq_client;
    struct mq_attr attr;
    char buffer[MSG_SIZE];

    // Создание атрибутов очереди
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10; // Максимальное количество сообщений в очереди
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Создание очереди сервера
    mq_server = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, &attr);
    if (mq_server == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Открытие очереди клиента для записи
    mq_client = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_client == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Ожидание сообщения от клиента
    if (mq_receive(mq_server, buffer, MSG_SIZE, 0) == -1) {
        perror("mq_receive");
        exit(1);
    }

    printf("Received message from client: %s\n", buffer);

    // Отправка ответа клиенту
    strncpy(buffer, "Hello!", MSG_SIZE - 1);
    buffer[MSG_SIZE - 1] = '\0';
    if (mq_send(mq_client, buffer, strlen(buffer) + 1, 1) == -1) {
        perror("mq_send");
        exit(1);
    }

    // Закрытие очередей и их удаление
    if (mq_close(mq_server) == -1) {
        perror("mq_close");
        exit(1);
    }

    if (mq_close(mq_client) == -1) {
        perror("mq_close");
        exit(1);
    }

    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        exit(1);
    }

    return 0;
}
