#include "chat.h"

char **messages;
char messages_name[MAX_CLENT][WIDTH_NIKNAME_GRAPHICALLY];
int message_count = 0;
int message_count_name = 0;
pthread_mutex_t message_mutex, message_mutex_two,message_mutex_tree;
char Input_Message[MAX_MESSAGE_SIZE];
ssize_t bytes_read;

void removeSubstring(char *s, const char *toremove) {
    while ((s = strstr(s, toremove))) {
        memmove(s, s + strlen(toremove), strlen(s + strlen(toremove)) + 1);
    }
}

void* display_thread(void* arg) {
    parametr_thread* param = (parametr_thread*) arg;
    while (!param->exit_programm) {
        pthread_mutex_lock(&message_mutex);
        param = (parametr_thread*) arg;
        clear();
        print_all_chat(param);
        for (int i = 0; i < message_count; i++) {
            mvprintw((param->size.height-message_count) - 2+i, 1, "%s", messages[i]);
        }
        for (int j = 0; j < message_count_name; j++) {
            mvprintw(j + 1, param->size.width-WIDTH_NIKNAME_GRAPHICALLY+1, "%s", 
            messages_name[j]);
        }
        mvprintw((param->size.height) - 2, 1, "%s", Input_Message);
        usleep(10000);
        refresh();
        pthread_mutex_unlock(&message_mutex);
    }
    return NULL;
}

void* input_thread(void* arg) {
    int count_input = 0;
    char Input;
    parametr_thread* param = (parametr_thread*) arg;
    int Gap = (param->size.width) - WIDTH_NIKNAME_GRAPHICALLY - 1;
    Message message;
    message.id = param->id;
    Input_Message[0] = '\0';


    while (!param->exit_programm) {
        pthread_mutex_lock(&message_mutex_two);
        Input = getch();
        if (Input == 7 && count_input > 0) {
            count_input--;
            Input_Message[count_input] = '\0';
        } else {
            if (count_input < Gap && (isalnum(Input) || ispunct(Input) || Input == ' ')) {
                Input_Message[count_input] = Input;
                Input_Message[count_input + 1] = '\0';
                count_input++;
            }
        }
        if (Input == 10) {
            strncpy(message.buffer, param->Name, MAX_MESSAGE_SIZE);
            strncat(message.buffer, ": ", MAX_MESSAGE_SIZE);
            strncat(message.buffer, Input_Message, MAX_MESSAGE_SIZE);
            message.buffer[strlen(param->Name)+2+count_input]='\0';
            if (mq_send(param->mq, (const char*)&message, sizeof(Message), 1) == -1) {
                perror("mq_send Enter");
                exit(1);
            }
            count_input = 0;
            Input_Message[0] = '\0';

        }
        if (Input == 27) {
            memset(message.buffer, 0, sizeof(message.buffer));
            strncpy(message.buffer, "/exit", strlen("/exit")+1);
            if (mq_send(param->mq, (const char*)&message, sizeof(Message), 0) == -1) {
                perror("mq_send ESC");
                exit(1);
            }
            param->exit_programm = 1;
        }
        pthread_mutex_unlock(&message_mutex_two);
    }
    return NULL;
}

void* message_processor_thread(void* arg) {
    unsigned int priority;
    parametr_thread* param = (parametr_thread*) arg;
    Message message;
    char clientName[sizeof(CLIENT_NAME)+11];
    struct mq_attr attr;
    attr.mq_msgsize = 1024;
    
    int Gap = (param->size.width) - WIDTH_NIKNAME_GRAPHICALLY - 1;
    messages = (char **)malloc(param->size.height * sizeof(char *));
    if (messages == NULL) {
        perror("Error: Failed to allocate memory");
        exit(1);
    }

    for (int i = 0; i < param->size.height; i++) {
        // +1 for the final zero symbol
        messages[i] = (char *)malloc((Gap + 1) * sizeof(char));
        if (messages[i] == NULL) {
            // Processing the error of memory release for line I
            perror("Error: Failed to allocate memory for row");
            // The release of memory allocated before
            for (int j = 0; j < i; j++) {
                free(messages[j]);
            }
            free(messages);
            exit(1);
        }
        // Initialization of the line zeros (optional, but recommended)
    }
    
    sprintf(clientName, "%s%d",CLIENT_NAME, param->id);
    mqd_t mq_client = mq_open(clientName, O_CREAT | O_NONBLOCK, S_IRUSR | S_IWUSR, 
    &param->attr);
    if (mq_client == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }
    while (!param->exit_programm) {
        pthread_mutex_lock(&message_mutex_tree);
        bytes_read = mq_receive(mq_client, (char *)&message, attr.mq_msgsize, &priority);
        // We check if there was an error when reading from the queue of messages
        if (bytes_read == -1) {
            // If an error, check Errno
            if (errno != EAGAIN){
                // In other cases, we display an error message
                perror("mq_receive_client");
                exit(1);
            }
        } else {
            // Processing of successful message reading
            if (priority == 1){
                if (param->size.height - 4 < message_count){
                    for(int i=0; i<message_count-1;i++){
                        strncpy(messages[i], messages[i+1], Gap+1);
                    }
                    strncpy(messages[message_count-1], message.buffer, Gap+1);
                }else{
                    strncpy(messages[message_count], message.buffer, Gap+1);
                    message_count++;
                }
                
            }
            if (priority == 0){
                if (strncmp(message.buffer, "/exet_now", 9) == 0) message_count_name = 0;
                if (strncmp(message.buffer, "/exet_step", 10) == 0){
                    removeSubstring(message.buffer, "/exet_step");
                    strncpy(messages_name[message_count_name], message.buffer, 
                    WIDTH_NIKNAME_GRAPHICALLY+1);
                    message_count_name++;
                }
            }
            
        }
        pthread_mutex_unlock(&message_mutex_tree);
    }
    for (int i = 0; i < param->size.height; i++) {
        free(messages[i]);
    }

    free(messages);
    mq_close(mq_client);
    return NULL;
}