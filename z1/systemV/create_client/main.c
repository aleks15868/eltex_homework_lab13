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

    // Получение доступа к очереди сообщений
    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Отправка сообщения серверу
    strncpy(msg.msg_text, "Hi!", sizeof(msg.msg_text) - 2);
    msg.msg_text[sizeof(msg.msg_text) - 2] = '\0';
    msg.msg_type = 2;
    if (msgsnd(msgid, &msg, sizeof(msg), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    // Получение ответа от сервера
    if (msgrcv(msgid, &msg, sizeof(msg), 1, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Received message from server: %s\n", msg.msg_text);

    // Удаление очереди сообщений (это может быть опущено, если не требуется)
    // msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
