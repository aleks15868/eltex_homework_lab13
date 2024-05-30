#include "chat.h"


void print_all_chat(parametr_thread* param)
{
    attron(COLOR_PAIR(BACKGROUND_COLOR_DARK_GREEN));
    for (int i=0; i<param->size.height; i++){
        if (i == 0 || i == param->size.height-1){
            mvprintw(i, 0, "+");
            mvhline(i, 1, '-', param->size.width-WIDTH_NIKNAME_GRAPHICALLY-1);
            mvprintw(i, param->size.width-WIDTH_NIKNAME_GRAPHICALLY, "+");
            mvhline(i, param->size.width-WIDTH_NIKNAME_GRAPHICALLY+1, '-', 
            WIDTH_NIKNAME_GRAPHICALLY-2);
            mvprintw(i, param->size.width-1, "+");
        }
        else{
            if(i == param->size.height-2){
                mvhline(i, 1, ' ', param->size.width-WIDTH_NIKNAME_GRAPHICALLY-1);
                mvprintw(i, 0, ">");

            }else{
                mvprintw(i, 0, "|");
                mvhline(i, 1, ' ', param->size.width-WIDTH_NIKNAME_GRAPHICALLY-1);
            }
            mvprintw(i, param->size.width-WIDTH_NIKNAME_GRAPHICALLY, "|");
            mvhline(i, param->size.width-WIDTH_NIKNAME_GRAPHICALLY+1, ' ', 
            WIDTH_NIKNAME_GRAPHICALLY-2);
            mvprintw(i, param->size.width-1, "|");
        }
        
        
    }
};

void print_window_input_name(parametr_thread* param)
{
    char Input;
    int cout = 0;
    Message message;
    message.id = param->id;
    attron(COLOR_PAIR(BACKGROUND_COLOR_WHITE_GREEN));
    mvhline(param->size.height/2, param->size.width/2-WIDTH_NIKNAME/2+3, ' ', 
    WIDTH_NIKNAME+1);
    mvprintw(param->size.height/2, param->size.width/2-WIDTH_NIKNAME/2-3, "Name: ");    
    move(param->size.height/2, param->size.width/2-WIDTH_NIKNAME/2+3);
    while(1)
    {
        Input = getch();
        if(Input==7 && cout>=0){
            cout--;
            param->Name[cout]='\0';
        }
        else{
            if((cout<WIDTH_NIKNAME) && (isalnum(Input) || ispunct(Input))){
            param->Name[cout]=Input;
            param->Name[cout+1]='\0';
            cout++;
            }
        }
        mvhline(param->size.height/2, param->size.width/2-WIDTH_NIKNAME/2+3, ' ', 
        WIDTH_NIKNAME+1);
        mvprintw(param->size.height/2, param->size.width/2-WIDTH_NIKNAME/2+3, "%s", 
        param->Name);
        if (Input==10){
            memset(message.buffer, 0, sizeof(message.buffer));
            sprintf(message.buffer, "/append%s", param->Name);
            if (mq_send(param->mq, (const char*)&message, sizeof(Message), 0) == -1) {
                perror("mq_send");
                exit(1);
            }
            break;
        }

        if (Input==27){
            param->exit_programm = 1;
            break;
        }
    }
};