#include <page_manager.h>
#include <ui.h>

static lv_obj_t * main_page = NULL;

LV_FONT_DECLARE(my_t01);
#define FONTAWESOME_SYMBOL_music "\xef\x80\x81"
#define FONTAWESOME_SYMBOL_display "\xef\x80\x82"
#define FONTAWESOME_SYMBOL_control "\xef\x89\x96"

static void my_Music_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e); // 获取当前部件(对象)触发的事件代码

    if(code_e == LV_EVENT_CLICKED) {
        Page("music")->Run(NULL);
    }
}

static void my_Display_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e); // 获取当前部件(对象)触发的事件代码

    if(code_e == LV_EVENT_CLICKED) {
        Page("display")->Run(NULL);
    }
}

static void my_Control_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e); // 获取当前部件(对象)触发的事件代码

    if(code_e == LV_EVENT_CLICKED) {
        Page("control")->Run(NULL);
    }
}

static void MainPageCreate()
{
    // 创建主界面
    main_page = lv_obj_create(NULL);
}

static void MainPageInit(void * User_data)
{

    LV_IMG_DECLARE(bg);
    lv_obj_t * img = lv_img_create(main_page);
    lv_img_set_src(img, &bg);
    // 按钮
    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_radius(&style_def, 20);
    lv_style_set_width(&style_def, 150); // 设置样式的宽度
    lv_style_set_height(&style_def, 150);
    lv_style_set_bg_color(&style_def, lv_color_hex(0x99ccff));
    lv_style_set_text_color(&style_def, lv_color_hex(0xffffff));
    FT_font_Set(30);
    // 字体
    static lv_style_t style_font;
    lv_style_init(&style_font);
    lv_style_set_text_font(&style_font, font);
    // 图标
    static lv_style_t style_image;                                 // 创建样式
    lv_style_init(&style_image);                                   // 初始化图标样式
    lv_style_set_text_font(&style_image, &my_t01);                 // 初始化图标样式字体
    lv_style_set_text_color(&style_image, lv_color_hex(0xffffff)); // 初始化图标样式颜色，白色

    lv_obj_t * Btn_music = lv_btn_create(img);
    lv_obj_align(Btn_music, LV_ALIGN_LEFT_MID, 90, 0);
    lv_obj_add_style(Btn_music, &style_def, LV_STATE_DEFAULT);
    lv_obj_t * Label_music = lv_label_create(img);                 // 使用新添加的图标（symbol）
    lv_label_set_text(Label_music, FONTAWESOME_SYMBOL_music);      // 添加图标
    lv_obj_add_style(Label_music, &style_image, LV_STATE_DEFAULT); // 添加样式，白色图标以及字体设置
    lv_obj_align_to(Label_music, Btn_music, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t * Label_music_str = lv_label_create(img);
    lv_obj_add_style(Label_music_str, &style_font, 0);
    lv_obj_set_style_text_color(Label_music_str, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text_fmt(Label_music_str, "音乐"); // 添加名字
    lv_obj_align_to(Label_music_str, Btn_music, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

    lv_obj_t * Btn_dispaly = lv_btn_create(img);
    lv_obj_align(Btn_dispaly, LV_ALIGN_LEFT_MID, 330, 0);
    lv_obj_add_style(Btn_dispaly, &style_def, LV_STATE_DEFAULT);
    lv_obj_t * Label_display = lv_label_create(img);                 // 使用新添加的图标（symbol）
    lv_label_set_text(Label_display, FONTAWESOME_SYMBOL_display);    // 添加图标
    lv_obj_add_style(Label_display, &style_image, LV_STATE_DEFAULT); // 添加样式，白色图标以及字体设置
    lv_obj_align_to(Label_display, Btn_dispaly, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t * Label_display_str = lv_label_create(img);
    lv_obj_add_style(Label_display_str, &style_font, 0);
    lv_obj_set_style_text_color(Label_display_str, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text_fmt(Label_display_str, "显示"); // 添加名字
    lv_obj_align_to(Label_display_str, Btn_dispaly, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

    lv_obj_t * Btn_control = lv_btn_create(img);
    lv_obj_align(Btn_control, LV_ALIGN_LEFT_MID, 570, 0);
    lv_obj_add_style(Btn_control, &style_def, LV_STATE_DEFAULT);
    lv_obj_t * Label_control = lv_label_create(img);                 // 使用新添加的图标（symbol）
    lv_label_set_text(Label_control, FONTAWESOME_SYMBOL_control);    // 添加图标
    lv_obj_add_style(Label_control, &style_image, LV_STATE_DEFAULT); // 添加样式，白色图标以及字体设置
    lv_obj_align_to(Label_control, Btn_control, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t * Label_control_str = lv_label_create(img);
    lv_obj_add_style(Label_control_str, &style_font, 0);
    lv_obj_set_style_text_color(Label_control_str, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text_fmt(Label_control_str, "控制"); // 添加名字
    lv_obj_align_to(Label_control_str, Btn_control, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);

    lv_obj_add_event_cb(Btn_music, my_Music_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(Btn_dispaly, my_Display_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(Btn_control, my_Control_event_cb, LV_EVENT_CLICKED, NULL);
}

static void MainPageRun(void * pParams)
{
    lv_obj_clean(lv_scr_act());
    MainPageInit(NULL);
    lv_disp_load_scr(main_page);
}

static PageAction Main_Page = {
    .name   = "main",
    .Create = MainPageCreate,
    .Run    = MainPageRun,
};

void Main_Page_Registered(void)
{
    Registered_Page(&Main_Page);
}