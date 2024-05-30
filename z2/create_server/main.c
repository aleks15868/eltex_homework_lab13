#include "server.h"

void sending_messages(int count_id, Array_Id* Name_id,struct mq_attr* attr,
unsigned int priority, Message* data){
    mqd_t mq_client;
    char clientName[sizeof(CLIENT_NAME)+11]; // Assuming clientName won't exceed 20 characters

    for (int i = 0; i < count_id; i++) {
        sprintf(clientName, "%s%d", CLIENT_NAME, Name_id[i].Id);
        mq_client = mq_open(clientName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, attr);
        if (mq_client == (mqd_t)-1) {
            perror("mq_open");
            exit(1);
        }
        // Sending the message
        if (mq_send(mq_client, (const char*)&data[0], sizeof(Message), priority) == -1) {
            perror("mq_send");
            exit(1);
        }
        printf("id:%s:%s\n", clientName, data->buffer);
        mq_close(mq_client);
    }
}

int main() {
    mqd_t mq;
    struct mq_attr attr;
    Array_Id Name_id[MAX_CLIENT];
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;
    int must_stop = 0;
    int count_id = 0;
    unsigned int priority;
    // char clientName[sizeof(CLIENT_NAME)+11];
    // Создаем очередь сообщений POSIX
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // Define the data structure and fill it out
    Message data;
    
    while (!must_stop) {
        // Receiving a message from a queue
        if (mq_receive(mq, (char *)&data, attr.mq_msgsize, &priority) == -1) {
            perror("mq_receive");
            exit(1);
        }

        //Processing of the received message
        if ((priority == 0) && (strncmp(data.buffer, "/append", 7) == 0)) {
            removeSubstring(data.buffer, "/append");
            strncpy(Name_id[count_id].Name, data.buffer, WIDTH_NIKNAME+1);
            Name_id[count_id].Id = data.id;
            count_id++;

            sprintf(data.buffer, "/exet_now");
            sending_messages(count_id,Name_id,&attr,priority,&data); 
            for(int i = 0; i<count_id; i++){
                sprintf(data.buffer, "/exet_step%s",Name_id[i].Name);
                sending_messages(count_id,Name_id,&attr,priority,&data); 
            }
        }
        if ((priority == 0) && (strncmp(data.buffer, "/exit", 5) == 0)) {
            removeSubstring(data.buffer, "/exit");
            strncpy(Name_id[count_id].Name, data.buffer, WIDTH_NIKNAME+1);
            for(int i = 0; i<count_id;i++){
                if(Name_id[i].Id == data.id){
                    for(int j = i; j<count_id;j++){
                        strncpy(Name_id[count_id].Name, Name_id[count_id+1].Name, 
                        WIDTH_NIKNAME+1);
                        Name_id[count_id].Id = Name_id[count_id+1].Id;
                    }
                    break;
                }
            }
            count_id--;
            sprintf(data.buffer, "/exet_now");
            sending_messages(count_id,Name_id,&attr,priority,&data); 
            for(int i = 0; i<count_id; i++){
                sprintf(data.buffer, "/exet_step2 %s", Name_id[i].Name);
                sending_messages(count_id,Name_id,&attr,priority,&data); 
            }
        }
        if (priority == 1){
            sending_messages(count_id,Name_id,&attr,1,&data); 
        }
        sending_messages(count_id,Name_id,&attr,2,&data); 
        

        // Example of processing command "QUIT"
        if (strncmp(data.buffer, "quit", 4) == 0) {
            must_stop = 1;
        }
    }



    // Close and remove the queue of messages after using
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}
