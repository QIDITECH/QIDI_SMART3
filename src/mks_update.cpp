#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "../include/event.h"
#include "../include/send_msg.h"
#include "../include/MakerbaseSerial.h"

extern int tty_fd;                          // main.cpp 里面的变量
extern bool is_download_to_screen;          // main.cpp 里面的变量

int copy_fd;

bool detected_soc_data;
bool detected_mcu_data;
bool detected_ui_data;

bool detected_printer_cfg;
bool detected_MKS_THR_cfg;

//1.1.7 CLL 工厂更新
bool detected_gcode;

std::ifstream tftfile;
int tft_buff = 4096;
int tft_start;
int tft_end;
std::string tft_s;
std::string tft_data;
int tft_len;
int filesize;

int tft_index;
char tft_buffer[4096] = {0};

int u_disk_update() {
    int fd = 1;
    char PATH[64];
    char cmd[64];

    char chOrder[50];
    char chOrder1[50];

    int i = 0;
    int j = 0;

    char ch[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 8; i++) {
            sprintf(PATH, "/home/mks/gcode_files/sd%c%d/QD_Update/QD_Smart_SOC", ch[i], j);
            fd = access(PATH, F_OK);
            if (fd == 0) {
                printf("检测到U盘下存在目录%s，正在检测更新文件\n", PATH);
                sprintf(cmd, "dpkg -i %s; rm %s -f", PATH, PATH);
                system(cmd);
                break;
            } else {
                // printf("没有检测到U盘相关的内容\n");
                continue;
            }
        }
        if (fd == 0) {
            break;
        }
    }

    if (fd == -1) {
        printf("无法打开u盘\n");
    } else {
        close(fd);
    }
    return 0;
}

bool detect_update() {

    int fd_soc_data;
    int fd_mcu_data;
    int fd_ui_data;
    int fd_printer_cfg;
    int fd_mks_thr_cfg;

    //1.1.7 CLL 工厂更新
    int fd_gcode;

    fd_soc_data = access("/home/mks/gcode_files/sda1/QD_Update/QD_Smart_SOC", F_OK);
    if (fd_soc_data == 0) {
        detected_soc_data = true;
    } else {
        detected_soc_data = false;
    }

    fd_mcu_data = access("/home/mks/gcode_files/sda1/QD_MCU/MCU", F_OK);
    if (fd_mcu_data == 0) {
        detected_mcu_data = true;
    } else {
        detected_mcu_data = false;
    }

    // fd_ui_data = access("/home/mks/gcode_files/sda1/QD_Update/QD_Smart_UI5.0", F_OK);
    fd_ui_data = access("/home/mks/gcode_files/sda1/QD_Update/QD_Smart3_UI4.3", F_OK);
    // fd_ui_data = access("/root/800_480.tft", F_OK);

    if (fd_ui_data == 0) {
        detected_ui_data = true;
    } else {
        detected_ui_data = false;
    }

    fd_printer_cfg = access("/home/mks/gcode_files/sda1/QD_Update/printer.cfg", F_OK);
    if (fd_printer_cfg == 0) {
        detected_printer_cfg = true;
    } else {
        detected_printer_cfg = false;
    }

    fd_mks_thr_cfg = access("/home/mks/gcode_files/sda1/QD_Update/MKS_THR.cfg", F_OK);
    if (fd_mks_thr_cfg == 0) {
        detected_MKS_THR_cfg = true;
    } else {
        detected_MKS_THR_cfg = false;
    }

    //1.1.7 CLL 工厂更新
    fd_gcode = access("/home/mks/gcode_files/sda1/QD_Update/QD_Gcode",F_OK);
    if (fd_gcode == 0){
        detected_gcode = true;
    }else{
        detected_gcode = false;
    }

    // std::cout << "detected_soc_data " << detected_soc_data << std::endl;
    // std::cout << "detected_mcu_data " << detected_mcu_data << std::endl;
    // std::cout << "detected_ui_data " << detected_ui_data << std::endl;
    // std::cout << "detected_printer_cfg " << detected_printer_cfg << std::endl;
    // std::cout << "detected_MKS_THR_cfg " << detected_MKS_THR_cfg << std::endl;
    //1.1.7 CLL 工厂更新
    return (detected_soc_data | detected_mcu_data | detected_ui_data | detected_printer_cfg | detected_MKS_THR_cfg | detected_gcode);
}

void start_update() {

    if (detected_soc_data == true) {
        if (access("/home/mks/gcode_files/sda1/QD_factory_mode.txt", F_OK) == 0) {
            std::cout << "检测到qidi文件" << std::endl;
            system("mv /home/mks/gcode_files/sda1/QD_Update/QD_Smart_SOC /home/mks/gcode_files/sda1/QD_Update/mks.deb; dpkg -i /home/mks/gcode_files/sda1/QD_Update/mks.deb; mv /home/mks/gcode_files/sda1/QD_Update/mks.deb /home/mks/gcode_files/sda1/QD_Update/QD_Smart_SOC; sync;");
        } else {
            if (access("/home/mks/gcode_files/sda1/QD_Update/QD_factory_mode.txt", F_OK) == 0) {
                std::cout << "检测到qidi文件" << std::endl;
            system("mv /home/mks/gcode_files/sda1/QD_Update/QD_Smart_SOC /home/mks/gcode_files/sda1/QD_Update/mks.deb; dpkg -i /home/mks/gcode_files/sda1/QD_Update/mks.deb; mv /home/mks/gcode_files/sda1/QD_Update/mks.deb /home/mks/gcode_files/sda1/QD_Update/QD_Smart_SOC; sync;");
            } else {
                std::cout << "没有检测到qidi文件" << std::endl;
                system("mv /home/mks/gcode_files/sda1/QD_Update/QD_Smart_SOC /home/mks/gcode_files/sda1/QD_Update/mks.deb; dpkg -i /home/mks/gcode_files/sda1/QD_Update/mks.deb; mv /home/mks/gcode_files/sda1/QD_Update/mks.deb /home/mks/gcode_files/sda1/QD_Update/QD_Smart_SOC.bak; sync;");
            }
        }
    }

    if (detected_mcu_data == true) {
        if (access("/home/mks/gcode_files/sda1/QD_factory_mode.txt", F_OK) == 0) {
            // std::cout << "检测到qidi文件" << std::endl;
            system("cp /home/mks/gcode_files/sda1/QD_MCU/MCU /root/klipper.bin;");
            // reset_firmware();
            close_mcu_port();
            // sleep(1);
            // system("service klipper stop; /root/hid-flash /root/klipper.bin ttyS0;");
            system("service klipper stop; /root/hid-flash /root/klipper.bin ttyS0; systemctl start klipper; ");
        } else {
            if (access("/home/mks/gcode_files/sda1/QD_Update/QD_factory_mode.txt", F_OK) == 0) {
                // std::cout << "检测到qidi文件" << std::endl;
                system("cp /home/mks/gcode_files/sda1/QD_MCU/MCU /root/klipper.bin;");
                // reset_firmware();
                close_mcu_port();
                // sleep(1);
                // system("service klipper stop; /root/hid-flash /root/klipper.bin ttyS0;");
                system("service klipper stop; /root/hid-flash /root/klipper.bin ttyS0; systemctl start klipper; ");
            } else {
                // std::cout << "没有检测到qidi文件" << std::endl;
                system("cp /home/mks/gcode_files/sda1/QD_MCU/MCU /root/klipper.bin;");
                // reset_firmware();
                close_mcu_port();
                // sleep(2);
                system("service klipper stop; /root/hid-flash /root/klipper.bin ttyS0; systemctl start klipper; mv /home/mks/gcode_files/sda1/QD_MCU/MCU /home/mks/gcode_files/sda1/QD_MCU/MCU.bak");
            }
            
        }

    }


    if (detected_ui_data == true) {
        if (access("/home/mks/gcode_files/sda1/QD_factory_mode.txt", F_OK) == 0) {
            system("cp /home/mks/gcode_files/sda1/QD_Update/QD_Smart3_UI4.3 /root/800_480.tft; sync");
        } else {
            if (access("/home/mks/gcode_files/sda1/QD_Update/QD_factory_mode.txt", F_OK) == 0) {
                system("cp /home/mks/gcode_files/sda1/QD_Update/QD_Smart3_UI4.3 /root/800_480.tft; sync");
            } else {
                system("cp /home/mks/gcode_files/sda1/QD_Update/QD_Smart3_UI4.3 /root/800_480.tft; mv /home/mks/gcode_files/sda1/QD_Update/QD_Smart3_UI4.3 /home/mks/gcode_files/sda1/QD_Update/QD_Smart3_UI4.3.bak; sync");
            }
        }
        /*
        if (access("/home/mks/gcode_files/sda1/QD_Update/800_480.tft", F_OK) == 0) {
            tft_data.clear();
            tftfile.open("/home/mks/gcode_files/sda1/QD_Update/800_480.tft");
            struct stat tft_stat;
            stat("/home/mks/gcode_files/sda1/QD_Update/800_480.tft", &tft_stat);
            filesize = tft_stat.st_size;
            std::cout << "文件大小为: " << filesize << std::endl;
            send_cmd_download(tty_fd, filesize);
            std::ostringstream temp;
            temp << tftfile.rdbuf();
            tft_data = temp.str();
            tft_start = 0;
            tft_len = tft_data.length();
            tft_end = tft_buff;
            tftfile.close();
        }
        */
    }

    if (detected_printer_cfg == true) {
        if (access("/home/mks/gcode_files/sda1/QD_factory_mode.txt", F_OK) == 0) {
            system("cp /home/mks/gcode_files/sda1/QD_Update/printer.cfg /home/mks/klipper_config/printer.cfg; chmod 777 /home/mks/klipper_config/printer.cfg; sync");
        } else {
            if (access("/home/mks/gcode_files/sda1/QD_Update/QD_factory_mode.txt", F_OK) == 0) {
                system("cp /home/mks/gcode_files/sda1/QD_Update/printer.cfg /home/mks/klipper_config/printer.cfg; chmod 777 /home/mks/klipper_config/printer.cfg; sync");
            } else {
                system("cp /home/mks/gcode_files/sda1/QD_Update/printer.cfg /home/mks/klipper_config/printer.cfg; chmod 777 /home/mks/klipper_config/printer.cfg; mv /home/mks/gcode_files/sda1/QD_Update/printer.cfg /home/mks/gcode_files/sda1/QD_Update/printer.cfg.bak; sync");
            }
        }
    }

    if (detected_MKS_THR_cfg == true) {
        if (access("/home/mks/gcode_files/sda1/QD_factory_mode.txt", F_OK) == 0) {
            system("cp /home/mks/gcode_files/sda1/QD_Update/MKS_THR.cfg /home/mks/klipper_config/MKS_THR.cfg; chmod 777 /home/mks/klipper_config/MKS_THR.cfg; sync");
        } else {
            if (access("/home/mks/gcode_files/sda1/QD_Update/QD_factory_mode.txt", F_OK) == 0) {
                system("cp /home/mks/gcode_files/sda1/QD_Update/MKS_THR.cfg /home/mks/klipper_config/MKS_THR.cfg; chmod 777 /home/mks/klipper_config/MKS_THR.cfg; sync"); 
            } else {
                system("cp /home/mks/gcode_files/sda1/QD_Update/MKS_THR.cfg /home/mks/klipper_config/MKS_THR.cfg; chmod 777 /home/mks/klipper_config/MKS_THR.cfg; mv /home/mks/gcode_files/sda1/QD_Update/MKS_THR.cfg /home/mks/gcode_files/sda1/QD_Update/MKS_THR.cfg.bak; sync");
            }
        }
    }

    //1.1.7 CLL 工厂更新
    if (detected_gcode == true) {
        //先清空gcode_files文件夹中的所有其他文件,会保留下sda1目录和其中的文件，使用-rf指令，更改路径需要谨慎！！！
        system("rm /home/mks/gcode_files/*");
        system("find /home/mks/gcode_files -maxdepth 1 -type d ! -name sd* -a ! -name '.*' | grep gcode_files/ | xargs rm -rf");
        system("cp /home/mks/gcode_files/sda1/QD_Update/QD_gcode/*.gcode /home/mks/gcode_files; chmod 777 /home/mks/gcode_files/*.gcode; sync");
    }

    update_finished_tips();
}

void download_to_screen() {
    std::cout << "tft_start == " << tft_start << std::endl;
    if (tft_start < tft_len) {
        if (tft_end > tft_len) {
            tft_s = tft_data.substr(tft_start, tft_len - tft_start);
            std::cout << "发送下载数据 == " << tft_start << "/" << filesize <<std::endl;  
            send_cmd_download_data(tty_fd, tft_s);
            // send_cmd_download_data(copy_fd, tft_s);         // 将发送的文件
            // set_option(tty_fd, 115200, 8, 'N', 1);
            // close(copy_fd);
        }
        tft_s = tft_data.substr(tft_start, tft_buff);
        std::cout << tft_s.length() << " 发送下载数据 == " << tft_start << "/" << filesize <<std::endl;  
        tft_start = tft_end;
        tft_end = tft_end + tft_buff;
        send_cmd_download_data(tty_fd, tft_s);
        // send_cmd_download_data(copy_fd, tft_s);         // 将发送的文件
    }
}

void init_download_to_screen() {
    if (access("/root/800_480.tft", F_OK) == 0) {
        tft_data.clear();
        tftfile.open("/root/800_480.tft");
        struct stat tft_stat;
        stat("/root/800_480.tft", &tft_stat);
        filesize = tft_stat.st_size;
        std::cout << "文件大小为: " << filesize << std::endl;
        std::ostringstream temp;
        temp << tftfile.rdbuf();
        tft_data = temp.str();
        std::cout << "读取的字符串长度为：" << tft_data.length() << std::endl;

        tft_len = tft_data.length();
        tft_end = tft_buff;

        tftfile.close();
        /* 将数据发送到电脑 */
        
        // copy_fd = open("/root/rv.tft", O_RDWR | O_APPEND);

        /*
        while(tft_start < tft_end) {
            std::cout << "tft_start == " << tft_start << std::endl;
            if (tft_start < tft_len) {
                if (tft_end > tft_len) {
                    tft_s = tft_data.substr(tft_start, tft_len - tft_start);
                    std::cout << "发送下载数据 == " << tft_start << "/" << filesize <<std::endl;  
                    // send_cmd_download_data(tty_fd, tft_s);
                    send_cmd_download_data(copy_fd, tft_s);
                    close(copy_fd);
                    break;
                }
                tft_s = tft_data.substr(tft_start, tft_buff);
                std::cout << tft_s.length() << " 发送下载数据 == " << tft_start << "/" << filesize <<std::endl;  
                tft_start = tft_end;
                tft_end = tft_end + tft_buff;
                // send_cmd_download_data(tty_fd, tft_s);
                send_cmd_download_data(copy_fd, tft_s);
            }
        }
        */
        
        /**/
    }
}

void back_to_screen_old() {
    if (access("/root/800_480.tft.bak", F_OK) == 0) {
        tft_data.clear();
        tftfile.open("/root/800_480.tft.bak");
        struct stat tft_stat;
        stat("/root/800_480.tft.bak", &tft_stat);
        filesize = tft_stat.st_size;
        std::cout << "文件大小为: " << filesize << std::endl;

        send_cmd_download(tty_fd, filesize);

        std::ostringstream temp;
        temp << tftfile.rdbuf();
        tft_data = temp.str();

        tft_len = tft_data.length();
        tft_end = tft_buff;

        tftfile.close();
    }
}