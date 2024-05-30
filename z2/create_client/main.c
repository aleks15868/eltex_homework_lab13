#include "chat.h"



int main(void) {
    // The initialization of the screen
    initscr();
    cbreak(); // Entering without waiting \ n
    noecho(); //Do not display the introduced symbols
    curs_set(0);
    keypad(stdscr, TRUE); // Enable support for functional keys
    struct mq_attr attr;
    attr.mq_msgsize = 1024;
    parametr_thread param;
    param.exit_programm = 0;
    param.id = getpid();
    // Getting the size of the window
    getmaxyx(stdscr, param.size.height, param.size.width);

    start_color();

    // Initialization of the color scheme
    init_color(COLOR_DARK_GREEN, 0, 149, 86); // rgb value for light green
    init_color(COLOR_WHITE_GREEN, 701, 862, 682);
    init_pair(BACKGROUND_COLOR_DARK_GREEN, COLOR_WHITE, COLOR_DARK_GREEN);   // 
    init_pair(BACKGROUND_COLOR_WHITE_GREEN, COLOR_WHITE, COLOR_WHITE_GREEN); 


    pthread_t display_tid,input_tid,message_tid;

    param.mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &attr);

    // endless cycle for reading keys
    
    print_all_chat(&param);
    print_window_input_name(&param); 
    if(param.exit_programm == 0){
        pthread_create(&display_tid, NULL, display_thread,(void *)&param);
        pthread_create(&input_tid, NULL, input_thread,(void *)&param);
        pthread_create(&message_tid, NULL, message_processor_thread,(void *)&param);

        pthread_join(display_tid, NULL);
        pthread_join(input_tid, NULL);
        pthread_join(message_tid, NULL);
    }
    
    // Completion of work with ncurses and the release of memory
    endwin();
    return 0;
}

