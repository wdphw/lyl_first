#include <ui.h>
#include <page_manager.h>
#include <serial.h>

static lv_obj_t * control_page = NULL;

lv_obj_t * sw1;
lv_obj_t * sw2;
lv_obj_t * sw3;
lv_obj_t * sw4;
char buf[2];
static void my_btn_return_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);
    if(code_e == LV_EVENT_CLICKED) {

        Page("main")->Run(NULL);
    }
}

static void event_handler(lv_event_t * e)
{
    lv_obj_t * obj_e       = lv_event_get_target(e); // 获取触发事件的部件(对象)
    lv_event_code_t code_e = lv_event_get_code(e);   // 获取当前部件(对象)触发的事件代码

    if(code_e == LV_EVENT_VALUE_CHANGED) {
        const char * cmd;
        int bytes_written;

        if(obj_e == sw1) {
            if(lv_obj_has_state(obj_e, LV_STATE_CHECKED)) // 状态“1” ，打开灯光
            {
                cmd = "L1";
                printf("L1\n");
            } else {
                cmd = "L0";
                printf("L0\n");
            }
        } else if(obj_e == sw2) {
            if(lv_obj_has_state(obj_e, LV_STATE_CHECKED)) // 状态“1” ，打开蜂鸣器
            {
                cmd = "B1";
                printf("B1\n");
            } else {
                cmd = "B0";
                printf("B0\n");
            }
        } else if(obj_e == sw3) {
            if(lv_obj_has_state(obj_e, LV_STATE_CHECKED)) // 状态“1” ，打开窗户
            {
                cmd = "W1";
                printf("W1\n");
            } else {
                cmd = "W0";
                printf("W0\n");
            }
        } else if(obj_e == sw4) {
            if(lv_obj_has_state(obj_e, LV_STATE_CHECKED)) // 状态“1” ，打开空调
            {
                cmd = "A1";
                printf("A1\n");
            } else {
                cmd = "A0";
                printf("A0\n");
            }
        } else {
            return; // 如果不是我们感兴趣的对象，直接返回
        }

        // 发送命令字符串
        bytes_written = write(t_fd, cmd, 2);
        if(bytes_written < 0) {
            // 处理错误
            perror("Write failed");
        } else if(bytes_written != 2) {
            // 处理写入字节数不匹配的情况
            fprintf(stderr, "Expected to write 2 bytes, but only wrote %d\n", bytes_written);
        } else {
            // 添加延迟，确保数据完全发送
            usleep(10000); // 10毫秒延迟
        }
    }
}

static void ControlPageCreate()
{
    control_page = lv_obj_create(NULL);
}

static void ControlPageInit(void * User_data)
{

    LV_IMG_DECLARE(kz);
    lv_obj_t * img = lv_img_create(control_page);
    lv_img_set_src(img, &kz);

    // 创建返回按钮
    lv_obj_t * btn_return = lv_btn_create(control_page);
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

    sw1 = lv_switch_create(img);
    lv_obj_add_event_cb(sw1, event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_size(sw1, 100, 60);
    lv_obj_align(sw1, LV_ALIGN_LEFT_MID, 80, 0);
    lv_obj_t * Label_sw1_str = lv_label_create(img);
    lv_obj_add_style(Label_sw1_str, &style_font, 0);
    lv_obj_set_style_text_color(Label_sw1_str, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text_fmt(Label_sw1_str, "灯光"); // 添加名字
    lv_obj_align_to(Label_sw1_str, sw1, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

    sw2 = lv_switch_create(img);
    lv_obj_add_event_cb(sw2, event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_size(sw2, 100, 60);
    lv_obj_align(sw2, LV_ALIGN_LEFT_MID, 260, 0);
    lv_obj_t * Label_sw2_str = lv_label_create(img);
    lv_obj_add_style(Label_sw2_str, &style_font, 0);
    lv_obj_set_style_text_color(Label_sw2_str, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text_fmt(Label_sw2_str, "告警"); // 添加名字
    lv_obj_align_to(Label_sw2_str, sw2, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

    sw3 = lv_switch_create(img);
    lv_obj_add_event_cb(sw3, event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_size(sw3, 100, 60);
    lv_obj_align(sw3, LV_ALIGN_LEFT_MID, 420, 0);
    lv_obj_t * Label_sw3_str = lv_label_create(img);
    lv_obj_add_style(Label_sw3_str, &style_font, 0);
    lv_obj_set_style_text_color(Label_sw3_str, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text_fmt(Label_sw3_str, "窗户"); // 添加名字
    lv_obj_align_to(Label_sw3_str, sw3, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

    sw4 = lv_switch_create(img);
    lv_obj_add_event_cb(sw4, event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_size(sw4, 100, 60);
    lv_obj_align(sw4, LV_ALIGN_LEFT_MID, 580, 0);
    lv_obj_t * Label_sw4_str = lv_label_create(img);
    lv_obj_add_style(Label_sw4_str, &style_font, 0);
    lv_obj_set_style_text_color(Label_sw4_str, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text_fmt(Label_sw4_str, "空调"); // 添加名字
    lv_obj_align_to(Label_sw4_str, sw4, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
}

static void ControlPageRun(void * pParams)
{
    lv_obj_clean(lv_scr_act());
    ControlPageInit(NULL);
    lv_disp_load_scr(control_page);
}

static PageAction Control_Page = {
    .name   = "control",
    .Create = ControlPageCreate,
    .Run    = ControlPageRun,
};

void Control_Page_Registered(void)
{
    Registered_Page(&Control_Page);
}
