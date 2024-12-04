#ifndef __PAGE_MANAGER_H
#define __PAGE_MANAGER_H

#include <pthread.h>
typedef struct pageAction
{
    char * name;
    void (*Run)(void * pParams);
    void (*Create)(void);
    struct pageAction * ptNext;
} PageAction, *PPageAction;

void Registered_Page(PPageAction ptPageAction);
void PageSystemInit(void);
PPageAction Page(char * name);

extern pthread_mutex_t lvgl_mutex;

#endif