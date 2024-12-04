#include <ui.h>
#include <page_manager.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <serial.h>
#include <errno.h>
#include <fcntl.h>

int i              = 0;
char * mp3_files[] = {"music/1.mp3", "music/2.mp3", "music/3.mp3"};
int sockfd;

void * check()
{
    int fd;
    int is_awake            = 0;
    unsigned char buf[1024] = {0};
    sockfd                  = socket(AF_INET, SOCK_STREAM, 0);

    if(-1 == sockfd) {
        perror("socket create error");
        return -1;
    }

    struct sockaddr_in tclientaddr;

    tclientaddr.sin_family      = AF_INET;
    tclientaddr.sin_port        = htons(50000);
    tclientaddr.sin_addr.s_addr = inet_addr("192.168.137.3");
    memset(tclientaddr.sin_zero, 0, 8);
    int ret = connect(sockfd, (struct sockaddr *)&tclientaddr, sizeof(tclientaddr));
    if(-1 == ret) {
        perror("connect error");
        return -2;
    }
    while(1) {
        system("arecord -d4 -r16000 -c1 -fS16_LE lyl.wav");
        fd = open("lyl.wav", O_RDWR);
        if(-1 == fd) {
            perror("open wav error");
            exit(10);
        }
        int filesize = lseek(fd, 0x00, SEEK_END);
        lseek(fd, 0x00, SEEK_SET); // 偏移到开头
        send(sockfd, &filesize, 4, 0);
        recv(sockfd, buf, 9, 0);
        printf("11111 %s\n", buf);
        if(strcmp(buf, "next!!!!")) {
            printf("服务器错误了\n");
        }
        while(1) {
            int r = read(fd, buf, 1024);
            if(-1 == r) {
                perror("read pcm error");
                break;
            } else if(0 == r) {
                printf("over\n");
                break;
            } else {
                send(sockfd, buf, r, 0);
            }
        }
        close(fd);
        char rbuf[256];
        recv(sockfd, rbuf, 256, 0);
        printf("%s\n", rbuf);
        if(!is_awake) {
            if(strstr(rbuf, "yl") != NULL) {
                system("madplay music/x.mp3 &");
                is_awake = 1;
                sleep(4);
                memset(rbuf, 0, 256);
            }
        }

        if(is_awake) {
            if(strstr(rbuf, "打开 空调") != NULL) {
                write(t_fd, "A1", 2);
                system("madplay music/kaikong.mp3 &");
                sleep(2);
            } else if(strstr(rbuf, "关闭 空调") != NULL) {
                write(t_fd, "A0", 2);
                system("madplay music/bikong.mp3 &");
                sleep(2);
            } else if(strstr(rbuf, "打开 灯光") != NULL) {
                write(t_fd, "L1", 2);
                system("madplay music/kaideng.mp3 &");
                sleep(2);
            } else if(strstr(rbuf, "关闭 灯光") != NULL) {
                write(t_fd, "L0", 2);
                system("madplay music/bideng.mp3 &");
                sleep(2);
            } else if(strstr(rbuf, "打开 窗户") != NULL) {
                write(t_fd, "W1", 2);
                system("madplay music/kaichuang.mp3 &");
                sleep(2);
            } else if(strstr(rbuf, "关闭 窗户") != NULL) {
                write(t_fd, "W0", 2);
                system("madplay music/bichuang.mp3 &");
                sleep(2);
            } else if(strstr(rbuf, "关闭 报警") != NULL) {
                write(t_fd, "B0", 2);
                system("madplay music/bifeng.mp3 &");
                sleep(2);
            } else if(strstr(rbuf, "播放") != NULL) {
                char command[50];
                sprintf(command, "madplay %s &", mp3_files[i]);
                system(command);
                sleep(2);
            } else if(strstr(rbuf, "下") != NULL) {
                system("killall -9 madplay");
                i = (i + 1) % (sizeof(mp3_files) / sizeof(mp3_files[0]));
                char command[50];
                sprintf(command, "madplay %s &", mp3_files[i]);
                system(command);

                sleep(2);
            } else if((strstr(rbuf, "上") != NULL) || (strstr(rbuf, "放") != NULL)) {
                system("killall -9 madplay");
                i = (i - 1 + (sizeof(mp3_files) / sizeof(mp3_files[0]))) % (sizeof(mp3_files) / sizeof(mp3_files[0]));
                char command[50];
                sprintf(command, "madplay %s &", mp3_files[i]);
                system(command);
                sleep(2);
            } else if((strstr(rbuf, "暂停") != NULL) || strstr(rbuf, "急") != NULL) {

                system("killall -STOP madplay");

                sleep(2);
            } else if(strstr(rbuf, "继续") != NULL) {

                system("killall -CONT madplay &");

                sleep(2);
            } else if(strstr(rbuf, "退出") != NULL) {
                is_awake = 0;
                system("madplay music/t.mp3 &");
                sleep(4);
            }
        }
        memset(rbuf, 0, 256);
    }
    close(sockfd);      // 正常退出前关闭套接字
    pthread_exit(NULL); // 正常退出，返回NULL
}

int main(void)
{
    init_serial();
    pthread_t jc;
    pthread_create(&jc, NULL, check, 0);
    pthread_mutex_init(&lvgl_mutex, NULL);
    PageSystemInit();
    Page("main")->Run(NULL);

    /*Handle LVGL tasks*/
    while(1) {
        pthread_mutex_lock(&lvgl_mutex);
        lv_timer_handler();
        pthread_mutex_unlock(&lvgl_mutex);
        usleep(5000);
    }
    pthread_mutex_destroy(&lvgl_mutex);
    return 0;
}
