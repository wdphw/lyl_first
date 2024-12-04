#include <ui.h>

lv_font_t * font = NULL;

static const char * getenv_default(const char * name, const char * dflt)
{
    return getenv(name) ?: dflt;
}

static void lv_linux_disp_init(void)
{
    const char * device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
    lv_display_t * disp = lv_linux_fbdev_create();

    lv_linux_fbdev_set_file(disp, device);
}

int UI_Init()
{
    lv_init();
    /*Linux display device init*/
    lv_linux_disp_init();

    /*Linux touch device init*/
    lv_indev_t * ts = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event0");

    lv_evdev_set_calibration(ts, 0, 0, 1024, 600);

    lv_freetype_init(8, 64, 64);
    /*FreeType uses C standard file system, so no driver letter is required.*/
    font = lv_freetype_font_create("/etc/SIMHEI.TTF", LV_FREETYPE_FONT_RENDER_MODE_BITMAP, 24,
                                   LV_FREETYPE_FONT_STYLE_NORMAL);

    if(!font) {
        LV_LOG_ERROR("freetype font create failed.");
        return;
    }
    return 0;
}

void FT_font_Set(uint8_t size)
{
    font = lv_freetype_font_create("/etc/SIMHEI.TTF", LV_FREETYPE_FONT_RENDER_MODE_BITMAP, size,
                                   LV_FREETYPE_FONT_STYLE_NORMAL);
}