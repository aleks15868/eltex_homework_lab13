#include "server.h"

void array_duplication(Array_Id* array, int num, int *count, char* Name) {
    int i;
    int flag = 1;
    // We check if there is an element with the same ID in the array
    for (i = 0; i < *count; i++) {
        if (array[i].Id == num) {
            flag = 0;
            break;
        }
    }

    if (flag == 1) {
        if (*count < MAX_CLIENT) { // The maximum number of elements should be max_client
             // We use the current Count as an index for a new element
            array[*count].Id = num;
            // Copy the line in NAME
            strncpy(array[*count].Name, Name, sizeof(array[*count].Name) - 1); 
            // We are convinced that the line ends with a zero symbol
            array[*count].Name[sizeof(array[*count].Name) - 1] = '\0'; 
            (*count)++; // We increase the counter of the elements of the array
        }
    }
}


void removeSubstring(char *s, const char *toremove) {
    while ((s = strstr(s, toremove))) {
        memmove(s, s + strlen(toremove), strlen(s + strlen(toremove)) + 1);
    }
}