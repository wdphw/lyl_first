#include <ui.h>
#include <page_manager.h>

static void my_btn_return_event_cb(lv_event_t * e)
{
    lv_event_code_t code_e = lv_event_get_code(e);
    if(code_e == LV_EVENT_CLICKED) {

        Page("main")->Run(NULL);
    }
}

static lv_obj_t * music_page = NULL;

LV_FONT_DECLARE(m_but);
#define FONTAWESOME_SYMBOL_next "\xef\x81\x91"
#define FONTAWESOME_SYMBOL_previous "\xef\x81\x88"
#define FONTAWESOME_SYMBOL_play "\xef\x81\x8b"
#define FONTAWESOME_SYMBOL_stop "\xef\x81\x8c"

static bool is_playing = false;
static lv_obj_t * img_cover;
static const char * cover_images[] = {"S:/image/z.png", // 替换为你的具体图片路径
                                      "S:/image/x.png", "S:/image/tt.png"};
static int current_cover_index     = 0;
static const char * mp3_files[]    = {"music/1.mp3", "music/2.mp3", "music/3.mp3"};
int count                          = 0;
lv_anim_t a;
// 播放音乐
static void play_music(const char * filename)
{

    is_playing = true;
    char command[50];
    sprintf(command, "madplay %s &", filename);
    system(command);
    printf("Playing: %s\n", filename);
}

// 暂停音乐
static void pause_music()
{
    is_playing = false;
    system("killall -STOP madplay");
    printf("Music paused\n");
}

// 播放/暂停功能
static void play_pause_event(lv_event_t * e)
{

    lv_obj_t * label = lv_event_get_user_data(e);
    static lv_style_t style_i;                                 // 创建样式
    lv_style_init(&style_i);                                   // 初始化图标样式
    lv_style_set_text_font(&style_i, &m_but);                  // 初始化图标样式字体
    lv_style_set_text_color(&style_i, lv_color_hex(0xffffff)); // 初始化图标样式颜色，白色
    count += 1;
    if(is_playing) {
        // 暂停
        is_playing = false;
        lv_label_set_text(label, FONTAWESOME_SYMBOL_play);   // 添加图标
        lv_obj_add_style(label, &style_i, LV_STATE_DEFAULT); // 添加样式，白色图标以及字体设置
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
        pause_music();
        // 停止动画
        lv_anim_del(img_cover, lv_img_set_angle);
    } else {
        // 播放
        lv_anim_start(&a);
        lv_label_set_text(label, FONTAWESOME_SYMBOL_stop);   // 添加图标
        lv_obj_add_style(label, &style_i, LV_STATE_DEFAULT); // 添加样式，白色图标以及字体设置
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
        if(count == 1) {
            play_music(mp3_files[current_cover_index]);
        } else {
            system("killall -CONT madplay &");
        }
        is_playing = true;
        // 启动旋转动画
    }
}

// 切换封面到下一张
static void next_cover_event(lv_event_t * e)
{
    lv_anim_start(&a);
    lv_obj_t * label = lv_event_get_user_data(e);
    static lv_style_t style_i;                                 // 创建样式
    lv_style_init(&style_i);                                   // 初始化图标样式
    lv_style_set_text_font(&style_i, &m_but);                  // 初始化图标样式字体
    lv_style_set_text_color(&style_i, lv_color_hex(0xffffff)); // 初始化图标样式颜色，白色
    lv_label_set_text(label, FONTAWESOME_SYMBOL_stop);         // 添加图标
    lv_obj_add_style(label, &style_i, LV_STATE_DEFAULT);       // 添加样式，白色图标以及字体设置
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    system("killall -9 madplay");
    current_cover_index = (current_cover_index + 1) % (sizeof(cover_images) / sizeof(cover_images[0]));
    lv_img_set_src(img_cover, cover_images[current_cover_index]);
    play_music(mp3_files[current_cover_index]);
}

// 切换封面到上一张
static void previous_cover_event(lv_event_t * e)
{
    lv_anim_start(&a);
    lv_obj_t * label = lv_event_get_user_data(e);
    static lv_style_t style_i;                                 // 创建样式
    lv_style_init(&style_i);                                   // 初始化图标样式
    lv_style_set_text_font(&style_i, &m_but);                  // 初始化图标样式字体
    lv_style_set_text_color(&style_i, lv_color_hex(0xffffff)); // 初始化图标样式颜色，白色
    lv_label_set_text(label, FONTAWESOME_SYMBOL_stop);         // 添加图标
    lv_obj_add_style(label, &style_i, LV_STATE_DEFAULT);       // 添加样式，白色图标以及字体设置
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    system("killall -9 madplay");
    current_cover_index = (current_cover_index - 1 + (sizeof(cover_images) / sizeof(cover_images[0]))) %
                          (sizeof(cover_images) / sizeof(cover_images[0]));
    lv_img_set_src(img_cover, cover_images[current_cover_index]);
    play_music(mp3_files[current_cover_index]);
}

static void MusicPageCreate()
{
    music_page = lv_obj_create(NULL);
}

static void MusicPageInit(void * User_data)
{
    lv_obj_set_style_bg_color(music_page, lv_color_hex(0x000000), 0);
    // 按钮
    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_radius(&style_def, 20);
    lv_style_set_width(&style_def, 100); // 设置样式的宽度
    lv_style_set_height(&style_def, 100);
    lv_style_set_bg_color(&style_def, lv_color_hex(0x000000));
    lv_style_set_text_color(&style_def, lv_color_hex(0xffffff));
    // 图标
    static lv_style_t style_image;                                 // 创建样式
    lv_style_init(&style_image);                                   // 初始化图标样式
    lv_style_set_text_font(&style_image, &m_but);                  // 初始化图标样式字体
    lv_style_set_text_color(&style_image, lv_color_hex(0xffffff)); // 初始化图标样式颜色，白色

    // 创建返回按钮
    lv_obj_t * btn_return = lv_btn_create(music_page);
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

    // 创建圆形图片显示框
    img_cover = lv_img_create(music_page);
    lv_img_set_src(img_cover, cover_images[current_cover_index]);
    lv_obj_align(img_cover, LV_ALIGN_CENTER, 0, 0);

    // 动画
    lv_anim_init(&a);                                              /* 初始化动画对象 */
    lv_anim_set_var(&a, img_cover);                                /* 关联动画对象和图片对象 */
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_angle); /* 设置动画执行的回调函数 */
    lv_anim_set_values(&a, 0, 3600); /* 设置动画的起始和结束角度（0到3600度） */
    lv_anim_set_time(&a, 5000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    lv_obj_t * Btn_next = lv_btn_create(music_page);
    lv_obj_align(Btn_next, LV_ALIGN_BOTTOM_MID, 150, 0);
    lv_obj_add_style(Btn_next, &style_def, LV_STATE_DEFAULT);

    lv_obj_t * Label_next = lv_label_create(Btn_next);
    lv_label_set_text(Label_next, FONTAWESOME_SYMBOL_next);       // 添加图标
    lv_obj_add_style(Label_next, &style_image, LV_STATE_DEFAULT); // 添加样式，白色图标以及字体设置
    lv_obj_align(Label_next, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * Btn_previous = lv_btn_create(music_page);
    lv_obj_align(Btn_previous, LV_ALIGN_BOTTOM_MID, -150, 0);
    lv_obj_add_style(Btn_previous, &style_def, LV_STATE_DEFAULT);

    lv_obj_t * Label_previous = lv_label_create(Btn_previous);
    lv_label_set_text(Label_previous, FONTAWESOME_SYMBOL_previous);   // 添加图标
    lv_obj_add_style(Label_previous, &style_image, LV_STATE_DEFAULT); // 添加样式，白色图标以及字体设置
    lv_obj_align(Label_previous, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * Btn_play = lv_btn_create(music_page);
    lv_obj_align(Btn_play, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_style(Btn_play, &style_def, LV_STATE_DEFAULT);

    lv_obj_t * Label_play = lv_label_create(Btn_play);
    lv_label_set_text(Label_play, FONTAWESOME_SYMBOL_play);       // 添加图标
    lv_obj_add_style(Label_play, &style_image, LV_STATE_DEFAULT); // 添加样式，白色图标以及字体设置
    lv_obj_align(Label_play, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(Btn_play, play_pause_event, LV_EVENT_CLICKED, Label_play);
    lv_obj_add_event_cb(Btn_previous, previous_cover_event, LV_EVENT_CLICKED, Label_play);
    lv_obj_add_event_cb(Btn_next, next_cover_event, LV_EVENT_CLICKED, Label_play);
}

static void MusicPageRun(void * pParams)
{
    lv_obj_clean(lv_scr_act());
    MusicPageInit(NULL);
    lv_disp_load_scr(music_page);
}

static PageAction Music_Page = {
    .name   = "music",
    .Create = MusicPageCreate,
    .Run    = MusicPageRun,
};

void Music_Page_Registered(void)
{
    Registered_Page(&Music_Page);
}
