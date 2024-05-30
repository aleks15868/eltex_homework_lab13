#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

#define MSG_SIZE 128

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer msg;

    // Генерация ключа
    key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Создание очереди сообщений
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Ожидание сообщения от клиента
    if (msgrcv(msgid, &msg, sizeof(msg), 2, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Received message from client: %s\n", msg.msg_text);

    // Отправка ответа клиенту
    strncpy(msg.msg_text, "Hello!", sizeof(msg.msg_text) - 2);
    msg.msg_text[sizeof(msg.msg_text) - 2] = '\0'; // Убеждаемся, что строка заканчивается нулевым символом
    msg.msg_type = 1;
    if (msgsnd(msgid, &msg, sizeof(msg), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    // Удаление очереди сообщений
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
