#include <page_manager.h>
#include <string.h>

static PPageAction g_ptPages = NULL;

void Registered_Page(PPageAction ptPageAction)
{
    ptPageAction->Create();

    ptPageAction->ptNext = g_ptPages; // 头插法
    g_ptPages            = ptPageAction;
}

PPageAction Page(char * name)
{
    PPageAction ptTmp = g_ptPages;

    while(ptTmp) {
        if(strcmp(name, ptTmp->name) == 0) return ptTmp;
        ptTmp = ptTmp->ptNext;
    }

    return NULL;
}

void PageSystemInit(void)
{
    // LVGL图形库初始化
    UI_Init();

    extern void Main_Page_Registered(void);
    Main_Page_Registered();

    extern void Music_Page_Registered(void);
    Music_Page_Registered();

    extern void Control_Page_Registered(void);
    Control_Page_Registered();

    extern void Display_Page_Registered(void);
    Display_Page_Registered();
}