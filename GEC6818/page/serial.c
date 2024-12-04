#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <serial.h>

#define DEV_PATH1 "/dev/ttySAC1" // COM2
int t_fd;
void init_serial(void)
{

    // 1.打开串口
    t_fd = open(
        DEV_PATH1,
        O_RDWR |
            O_NOCTTY); // O_NOCTTY 标志用于通知
                       // Linux系统，该参数不会使打开的文件成为这个进程的控制终端。如果没有指定这个标志，那么任何一个输入（诸如键盘中止信号等）都将会影响用户的进程
    if(t_fd == -1) {
        printf("serial open fail!\n");
        // return -1;
    }
    // 声明设置串口的结构体
    struct termios new_cfg;
    // 先清空该结构体
    bzero(&new_cfg, sizeof(new_cfg));

    // 获取了文件描述符init_fd当前的终端I/O配置，保存原有配置
    // tcgetattr(t_fd, &old_cfg);

    // CLOCAL和CREAD分别用于本地连接和接受使能，因此，首先要通过位掩码的方式激活这两个选项。
    new_cfg.c_cflag |= CLOCAL | CREAD;

    //	cfmakeraw()设置终端属性，就是设置termios结构中的各个参数。
    cfmakeraw(&new_cfg);
    // 3.设置原始模式和波特率
    cfsetispeed(&new_cfg, B115200);
    cfsetospeed(&new_cfg, B115200);

    // 通过掩码设置数据位为8位
    new_cfg.c_cflag &= ~CSIZE;
    new_cfg.c_cflag |= CS8;

    // 无校验
    new_cfg.c_cflag &= ~PARENB;

    // 设置一个停止位
    new_cfg.c_cflag &= ~CSTOPB;

    // 可设置接收字符和等待时间，无特殊要求可以将其设置为0
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN]  = 0;

    // 清除串口缓冲即对尚未处理的输入输出数据进行清空处理。
    tcflush(t_fd, TCIOFLUSH);

    // 激活配置tcsetattr
    if(tcsetattr(t_fd, TCSANOW, &new_cfg)) {
        printf("Setting the serial1 failed!\n");
    }

    // 恢复原来的串口设置
    // tcsetattr(init_fd, TCSANOW, &old_cfg);
}