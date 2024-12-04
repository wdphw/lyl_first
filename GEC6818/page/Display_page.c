#include <ui.h>
#include <page_manager.h>
#include <serial.h>
#include <serial.h>
#include <sqlite3.h>
static lv_obj_t * display_page = NULL;
char t_sbuf[50];
char thl1[10];
char thl2[15];
char thl3[15];
float num1;
int num2, num3;
lv_obj_t * obj;
sqlite3 * db;
pthread_t t_id;
pthread_mutex_t lvgl_mutex;

static void my_btn_return_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);
    if(code_e == LV_EVENT_CLICKED) {
        int ret = pthread_cancel(t_id);
        if(ret != 0) {
            perror("线程销毁失败");
            return;
        }
        int s = pthread_join(t_id, NULL);
        if(s != 0) {
            printf("join thread:error\n");
            return;
        }
        Page("main")->Run(NULL);
    }
}

void query_event_cb(lv_event_t * e)
{
    lv_obj_t * db = lv_event_get_user_data(e);
    char * errmsg = 0;
    char sql[128] = {};
    char ** resultp;
    int nrow, ncol, i, j, index;
    sprintf(sql, "SELECT * FROM edata;");
    if(sqlite3_get_table(db, sql, &resultp, &nrow, &ncol, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "Query failed: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return;
    }

    char text[10000] = "";
    index            = 0;
    for(i = 0; i < nrow; i++) {
        for(j = 0; j < ncol; j++) {
            printf("%-14s", resultp[index]);
            if(j > 0) strcat(text, ", ");
            strcat(text, resultp[index]);
            index++;
        }
        printf("\n");
        if(strlen(text) + 1 < sizeof(text)) { // 检查是否有足够的空间添加换行符
            strcat(text, "\n");
        } else {
            break;
        }
    }

    lv_obj_t * mbox = lv_msgbox_create(lv_screen_active());
    lv_msgbox_add_title(mbox, "查询结果");
    lv_obj_t * title = lv_msgbox_get_title(mbox);
    lv_style_t style_font;
    lv_style_init(&style_font);
    lv_style_set_text_font(&style_font, font);
    lv_obj_add_style(title, &style_font, 0);
    lv_obj_t * cont  = lv_msgbox_get_content(mbox);
    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(cont, 400);
    lv_obj_set_height(cont, 400);
    lv_obj_align(mbox, LV_ALIGN_CENTER, 0, 0);
    lv_msgbox_add_close_button(mbox);
    lv_label_set_text(label, text);
    sqlite3_free_table(resultp); // 释放结果内存
}

void * tunc()
{
    char buf[64];
    char * errmsg = 0;
    // int rc;
    char sql[128]             = {};
    lv_obj_t * s_label        = lv_obj_get_child(obj, 0);
    lv_obj_t * temp_label     = lv_obj_get_child(obj, 2);
    lv_obj_t * light_label    = lv_obj_get_child(obj, 3);
    lv_obj_t * humidity_label = lv_obj_get_child(obj, 5);

    while(1) {
        // 从串口读取温T 湿H 度的值，保存在7个字节数组里
        memset(t_sbuf, 0, sizeof(t_sbuf));
        memset(thl1, 0, sizeof(thl1));
        memset(thl2, 0, sizeof(thl2));
        memset(thl3, 0, sizeof(thl3));
        memset(sql, 0, sizeof(sql));
        printf("======Get  Data Now======\n");
        read(t_fd, t_sbuf, sizeof(t_sbuf));
        printf("shuju = %s\n", t_sbuf);

        time_t now;
        time(&now);
        struct tm * time_info = localtime(&now);
        strftime(buf, sizeof(buf), "%H:%M:%S", time_info);

        if(sscanf(t_sbuf, "T%fH%dL%d", &num1, &num2, &num3) == 3) {
            printf("Extracted numbers: %f, %d, %d\n", num1, num2, num3);
            sprintf(sql, "INSERT INTO edata(temperature, humidity, light_intensity) VALUES(%.1f, %d, %d);", num1, num2,
                    num3);
            if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
                printf("%s\n", errmsg);
            } else {
                printf("insert success\n");
            }
            sprintf(thl1, "%.1f°C", num1);
            sprintf(thl2, "光照:%dlux", num3);
            sprintf(thl3, "湿度:%d%%", num2);
            pthread_mutex_lock(&lvgl_mutex);
            lv_label_set_text(s_label, buf);
            lv_label_set_text(temp_label, thl1);
            lv_label_set_text(light_label, thl2);
            lv_label_set_text(humidity_label, thl3);
            pthread_mutex_unlock(&lvgl_mutex);

        } else {
            printf("Error: Unable to extract numbers.\n");
        }
        sleep(1);
    }

    pthread_exit(NULL);
}

static void DisplayPageCreate()
{
    display_page = lv_obj_create(NULL);
}

static void DisplayPageInit(void * User_data)
{
    //  创建数据库
    char * errmsg = 0;
    int rc;
    const char * sql;

    rc = sqlite3_open("user.db", &db);
    if(rc) {
        fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    } else {
        printf("已成功打开数据库\n");
    }

    if(sqlite3_exec(db,
                    "CREATE TABLE IF NOT EXISTS edata (temperature DECIMAL(3, "
                    "1),humidity INT, light_intensity INT,timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);",
                    NULL, NULL, &errmsg) != SQLITE_OK) {
        printf("%s\n", errmsg);
    } else {
        printf("Edata Table created successfully.\n");
    }

    FT_font_Set(30);
    static lv_style_t e_style;
    lv_style_init(&e_style);
    lv_style_set_bg_opa(&e_style, LV_OPA_80);
    lv_style_set_border_width(&e_style, 1);
    lv_style_set_border_color(&e_style, lv_color_make(22, 10, 10));
    lv_style_set_size(&e_style, 300, 300);
    lv_style_set_text_font(&e_style, font);

    FT_font_Set(50);
    static lv_style_t w_style;
    lv_style_init(&w_style);
    lv_style_set_text_font(&w_style, font);

    LV_IMG_DECLARE(xs);
    lv_obj_t * img = lv_img_create(display_page);
    lv_img_set_src(img, &xs);
    obj = lv_obj_create(img);
    lv_obj_align(obj, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_style(obj, &e_style, 0);

    // 创建返回按钮
    lv_obj_t * btn_return = lv_btn_create(img);
    lv_obj_set_size(btn_return, 100, 100);
    lv_obj_align(btn_return, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_add_event_cb(btn_return, my_btn_return_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_return = lv_label_create(btn_return);

    FT_font_Set(30);
    static lv_style_t style_font;
    lv_style_init(&style_font);
    lv_style_set_text_font(&style_font, font);
    lv_obj_add_style(label_return, &style_font, 0);
    lv_label_set_text(label_return, "返回");
    lv_obj_align(label_return, LV_ALIGN_CENTER, 0, -4);

    lv_obj_t * button = lv_button_create(img);
    lv_obj_set_size(btn_return, 120, 100);
    lv_obj_t * query_label = lv_label_create(button); // 创建按钮上的 Label
    lv_obj_add_style(query_label, &style_font, 0);
    lv_label_set_text(query_label, "查询");
    lv_obj_align(button, LV_ALIGN_BOTTOM_RIGHT, -90, -80);
    lv_obj_add_event_cb(button, query_event_cb, LV_EVENT_CLICKED, db);

    // id0
    lv_obj_t * s_label = lv_label_create(obj);
    lv_label_set_text(s_label, "00:00:00");
    lv_obj_align(s_label, LV_ALIGN_TOP_RIGHT, 0, 0);
    // id1
    lv_obj_t * sz = lv_image_create(obj);
    lv_img_set_src(sz, "S:/image/sz.png");
    lv_obj_align_to(sz, s_label, LV_ALIGN_OUT_LEFT_MID, 0, 0);

    // 创建温度显示标签 id2
    lv_obj_t * temp_label = lv_label_create(obj);

    lv_label_set_text(temp_label, "25.1°C");
    lv_obj_align(temp_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(temp_label, &w_style, 0);

    // 创建其他信息显示标签 id3
    lv_obj_t * light_label = lv_label_create(obj);

    lv_label_set_text(light_label, "光照: 0lux");
    lv_obj_align(light_label, LV_ALIGN_CENTER, -80, 90);
    // id4
    lv_obj_t * gz = lv_image_create(obj);
    lv_img_set_src(gz, "S:/image/gz.png");
    lv_obj_align_to(gz, light_label, LV_ALIGN_OUT_TOP_MID, 0, 0);
    // id5
    lv_obj_t * humidity_label = lv_label_create(obj);
    lv_label_set_text(humidity_label, "湿度: 64%");
    lv_obj_align(humidity_label, LV_ALIGN_CENTER, 80, 90);
    // id6
    lv_obj_t * sd = lv_image_create(obj);
    lv_img_set_src(sd, "S:/image/sd.png");
    lv_obj_align_to(sd, humidity_label, LV_ALIGN_OUT_TOP_MID, 0, 0);

    lv_obj_t * d_label = lv_label_create(obj);
    lv_label_set_text(d_label, "湛江");
    lv_obj_align(d_label, LV_ALIGN_TOP_LEFT, 30, 0);

    lv_obj_t * wz = lv_image_create(obj);
    lv_img_set_src(wz, "S:/image/wz.png");
    lv_obj_align_to(wz, d_label, LV_ALIGN_OUT_LEFT_MID, 0, 0);

    pthread_create(&t_id, NULL, tunc, 0);
}

static void DisplayPageRun(void * pParams)
{
    lv_obj_clean(lv_scr_act());
    DisplayPageInit(NULL);
    lv_disp_load_scr(display_page);
}

static PageAction Display_Page = {
    .name   = "display",
    .Create = DisplayPageCreate,
    .Run    = DisplayPageRun,
};

void Display_Page_Registered(void)
{
    Registered_Page(&Display_Page);
}