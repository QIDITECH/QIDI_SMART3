#include "../include/ui.h"
#include "../include/send_msg.h"
#include "../include/mks_log.h"
#include "../include/event.h"
#include "../include/mks_file.h"
#include "../include/mks_preview.h"
#include "../include/mks_update.h"
#include "../include/mks_wpa_cli.h"

#include "../include/MakerbaseNetwork.h"
#include "../include/MakerbaseSerial.h"
#include "../include/MakerbaseWiFi.h"

#include <stack>

// #define DEFAULT_DIR "gcodes/sda1/"
#define DEFAULT_DIR "gcodes/"

int tty_fd = -1;

// 补偿值
extern bool fresh_page_set_zoffset_data;

// update
extern bool detected_soc_data;
extern bool detected_mcu_data;
extern bool detected_ui_data;

// bool start_to_printing = false;
// preview
extern std::vector<std::string> gimage;
extern std::vector<std::string> simage;

extern std::string str_gimage;

// extern std::vector<std::string> gimage_temp;
// extern std::vector<std::string> simage_temp;
// preview

// wifi start
extern int page_wifi_ssid_list_pages;
extern int page_wifi_current_pages;
// wifi end

extern int page_files_pages;
extern int page_files_current_pages;
extern int page_files_folder_layers;

extern std::string page_files_list_name[8];                                     // 文件列表显示文件名称
extern std::string page_files_list_show_name[8];                                // 文件列表名称
extern std::string page_files_list_show_type[8];   

extern std::stack<std::string> page_files_path_stack;          // 路径栈
extern std::string page_files_root_path;                       // Klippy根目录
extern std::string page_files_previous_path;                   // 之前的路径
extern std::string page_files_path;                            // 文件所在路径
extern std::string page_files_print_files_path;                // 要打印的文件路径

int current_page_id;                    // 当前页面的id号
int previous_page_id;                   // 上一页面的id号
int next_page_id;                       // 下一页面的id号

int event_id;
int page_id;
int widget_id;
int type_id;

int level_mode;                         // 调平模式

bool show_preview_complete = false;
bool show_preview_gimage_completed = false;

bool printing_keyboard_enabled = false;
// bool filament_keyboard_enabled = false;
bool auto_level_button_enabled = true;
bool manual_level_button_enabled = true;

bool printing_wifi_keyboard_enabled = false;

extern std::string printer_idle_timeout_state;
extern std::string printer_print_stats_state;

// 打印调平
int level_mode_printing_extruder_target = 0;
int level_mode_printing_heater_bed_target = 0;

bool level_mode_printing_is_printing_level = false;

bool page_wifi_list_ssid_button_enabled[3] = {false};

// page print filament
bool page_print_filament_extrude_restract_button = false;
bool page_filament_extrude_button = false;

// page reset
// bool page_reset_to_about = false;

extern bool mks_file_parse_finished;

//2023.4.24-2 实现过打印文件标红
std::string printed_file_path;

//2023.4.28 修改退料流程
extern int printer_extruder_temperature;
extern int printer_extruder_target;

extern std::string printer_webhooks_state;

//2023.5.10 CLL 修复页面跳转bug
extern bool printer_ready;

//2023.5.12 CLL 新增退料完成页面
bool page_filament_unload_button = false;

//1.1.4 CLL 新增打印热床调平
bool printer_bed_leveling = true;

void parse_cmd_msg_from_tjc_screen(char *cmd) {
    event_id = cmd[0];
    MKSLOG_BLUE("#########################%s", cmd);
    MKSLOG_RED("0x%x", cmd[0]);
    MKSLOG_RED("0x%x", cmd[1]);
    MKSLOG_RED("0x%x", cmd[2]);
    MKSLOG_RED("0x%x", cmd[3]);
    switch (event_id)
    {
    case 0x03:
        // MKSLOG_BLUE("读取到屏幕固件数据发生错误，进入到恢复屏幕的模式");
        break;

    case 0x05:
        std::cout << "可以开始发送数据了" << std::endl;
        // set_option(tty_fd, 921600, 8, 'N', 1);
        // set_option(tty_fd, 230400, 8, 'N', 1);
        download_to_screen();
        // 收到0x05消息，意味着可以开始发送串口屏幕的信息
        break;

    case 0x1a:
        std::cout << "变量名称无效" << std::endl;
        break;

    case 0x65:
        page_id = cmd[1];
        widget_id = cmd[2];
        type_id = cmd[3];
        tjc_event_clicked_handler(page_id, widget_id, type_id);
        break;

    case 0x66:
        break;

    case 0x67:
        break;

    case 0x68:
        break;
    
    case 0x70:
        MKSLOG_RED("0x%x", event_id);
        // page_id = cmd[1];
        // widget_id = cmd[2];
        // type_id = cmd[3];
        tjc_event_keyboard(cmd);
        break;

    case 0x71:
        page_id = cmd[1];
        tjc_event_setted_handler(cmd[1], cmd[2], cmd[3], cmd[4]);
        break;

    case 0x86:
    case 0x87:
    case 0x88:
    case 0x89:
        MKSLOG_RED("0x%x", event_id);
        break;

    case 0x91:
        current_page_id = TJC_PAGE_LOGO;
        page_to(TJC_PAGE_UPDATED);
        break;

    case 0xfd:
        break;
    case 0xfe:
        set_option(tty_fd, 230400, 8, 'N', 1);
        download_to_screen();
        break;

    case 0xff:
        MKSLOG_RED("0x%x", event_id);
        break;
    
    default:
        break;
    }
    cmd = NULL;
}

void page_to(int page_id) {
    previous_page_id = current_page_id;
    current_page_id = page_id;
    send_cmd_page(tty_fd, std::to_string(page_id));
}

void tjc_event_clicked_handler(int page_id, int widget_id, int type_id) {
    std::cout << "+++++++++++++++++++" << page_id << std::endl;
    std::cout << "+++++++++++++++++++" << widget_id << std::endl;
    std::cout << "+++++++++++++++++++" << type_id << std::endl;
    switch (page_id)
    {
    // 开箱第一项
    case TJC_PAGE_OPEN_LANGUAGE:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_LANGUAGE_NEXT:
            page_to(TJC_PAGE_OPEN_VIDEO_1);
            get_object_status();
            break;
        
        //2023.4.24-4 增加开机引导跳过
        case TJC_PAGE_OPEN_LANGUAGE_SKIP:
            //2023.5.12 CLL 新增开机引导跳过确认页面
            //page_to(TJC_PAGE_MAIN);
            //set_mks_oobe_enabled(false);
            page_to(TJC_PAGE_OPEN_POP_1);
            break;

        default:
            break;
        }
        break;

    // 开机第二项
    case TJC_PAGE_OPEN_VIDEO_1:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_VIDEO_1_NEXT:
            page_to(TJC_PAGE_OPEN_VIDEO2);
            break;
        
        default:
            break;
        }
        break;

    // 开机第三项
    case TJC_PAGE_OPEN_VIDEO2:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_VIDEO2_NEXT:
            page_to(TJC_PAGE_MOVE_TIP);
            break;

        default:
            break;
        }
        break;

    // 开机第四项
    case TJC_PAGE_MOVE_TIP:
        switch (widget_id)
        {
        case TJC_PAGE_MOVE_TIP_BACK:
            page_to(TJC_PAGE_OPEN_VIDEO2);
            break;

        case TJC_PAGE_MOVE_TIP_ENTER:
            // open_move_tip();
            // page_to(TJC_PAGE_OPEN_VIDEO_3);
            //2023.5.8 新增开机引导界面
            //pre_open_auto_level_init();
            open_heater_bed_up();
            break;

        default:
            break;
        }
        break;

    // 开机第五项
    case TJC_PAGE_OPEN_VIDEO_3:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_VIDEO_3_NEXT:
            // page_to(TJC_PAGE_OPEN_COMPENSAT);
            // open_go_to_syntony_move();
            pre_open_auto_level_init();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_OPEN_LEVEL_INIT_3:
        break;

    //2023.5.10 CLL 开机引导调平
    case TJC_PAGE_OPEN_AUTO_LEVEL_2:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_AUTO_LEVEL_2_001:
            set_auto_level_dist(0.01);
            //oobe_set_intern_zoffset(0.01);
            break;

        case TJC_PAGE_OPEN_AUTO_LEVEL_2_005:
            set_auto_level_dist(0.005);
            //oobe_set_intern_zoffset(0.05);
            break;
        
        case TJC_PAGE_OPEN_AUTO_LEVEL_2_01:
            set_auto_level_dist(0.1);
            //oobe_set_intern_zoffset(0.1);
            break;

        case TJC_PAGE_OPEN_AUTO_LEVEL_2_1:
            set_auto_level_dist(1);
            //oobe_set_intern_zoffset(1);
            break;

        case TJC_PAGE_OPEN_AUTO_LEVEL_UP:
            start_auto_level_dist(false);
            //oobe_set_zoffset(false);
            break;

        case TJC_PAGE_OPEN_AUTO_LEVEL_DOWN:
            start_auto_level_dist(true);
            //oobe_set_zoffset(true);
            break;
        
        case TJC_PAGE_OPEN_AUTO_LEVEL_ENTER:
            start_auto_level();
            page_to(TJC_PAGE_OPEN_LEVELING);
            //open_go_to_syntony_move();
            break;

        default:
            break;
        }
        break;

    // 开机第六项
    case TJC_PAGE_OPEN_COMPENSAT:
        break;

    // 开机第七项
    case TJC_PAGE_OPEN_FILAMENTVIDEO_1:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_FILAMENTVIDEO_1_NEXT:
            page_to(TJC_PAGE_OPEN_FILAMENTVIDEO_2);
            break;

        default:
            break;
        }
        break;

    // 开机第八项
    case TJC_PAGE_OPEN_FILAMENTVIDEO_2:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_FILAMENTVIDEO_2_NEXT:
            page_to(TJC_PAGE_OPEN_FILAMENTVIDEO_3);
            break;

        case TJC_PAGE_OPEN_FILAMENTVIDEO_2_TARGET:
            open_set_print_filament_target();
            break;
        
        case TJC_PAGE_OPEN_FILAMENTVIDEO_2_DECREASE:
            set_filament_extruder_target(false);
            break;
        
        case TJC_PAGE_OPEN_FILAMENTVIDEO_2_INCREASE:
            set_filament_extruder_target(true);
            break;
        
        default:
            break;
        }
        break;

    // 开机第九项
    case TJC_PAGE_OPEN_FILAMENTVIDEO_3:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_FILAMENTVIDEO_3_NEXT:
            page_to(TJC_PAGE_OPEN_LEVELMORE);
            break;

        case TJC_PAGE_OPEN_FILAMENTVIDEO_3_EXTRUDE:
            open_start_extrude();
            break;

        default:
            break;
        }
        break;
    
    case TJC_PAGE_OPEN_LEVELMORE:
        switch (widget_id)
        {
        case TJC_PAGE_OPEN_LEVELMORE_ENTER:
            open_more_level_finish();
            break;
        
        default:
            break;
        }
        break;
    
    case TJC_PAGE_MAIN:
        switch (widget_id)
        {
        case TJC_PAGE_MAIN_BTN_HOME:
            MKSLOG_BLUE("按下Home按钮");
            break;

        case TJC_PAGE_MAIN_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();
                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            
            break;

        case TJC_PAGE_MAIN_BTN_TOOL:
            page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_MAIN_BTN_SERVICE:
            page_to(TJC_PAGE_LANGUAGE);
            // go_to_about();
            break;
        
        case TJC_PAGE_MAIN_LIGHT:
            led_on_off();
            break;

        case TJC_PAGE_MAIN_VOICE:
            beep_on_off();
            break;

        case TJC_PAGE_MAIN_LOCK:
            motors_off();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_FILE_LIST_1:
        switch (widget_id)
        {
        case TJC_PAGE_FILE_LIST_1_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_FILE:
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_TOOL:
            page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_SERVICE:
            // page_to(TJC_PAGE_SERVICE);
            go_to_about();
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_1:
            clear_cp0_image();
            get_sub_dir_files_list(0);
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_2:
            clear_cp0_image();
            get_sub_dir_files_list(1);
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_3:
            clear_cp0_image();
            get_sub_dir_files_list(2);
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_4:
            clear_cp0_image();
            get_sub_dir_files_list(3);
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_5:
            clear_cp0_image();
            get_sub_dir_files_list(4);
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_6:
            clear_cp0_image();
            get_sub_dir_files_list(5);
            break;

        /*
        case TJC_PAGE_FILE_LIST_1_BTN_7:
            clear_cp0_image();
            get_sub_dir_files_list(6);
            break;

        case TJC_PAGE_FILE_LIST_1_BTN_8:
            clear_cp0_image();
            get_sub_dir_files_list(7);
            break;
        */

        case TJC_PAGE_FILE_LIST_1_BTN_PREVIOUS:
            if (page_files_current_pages > 0) {
                // std::cout << "############################" << page_files_current_pages << std::endl;
                // page_files_current_pages = (page_files_current_pages + page_files_pages) % (page_files_pages + 1);
                page_files_current_pages -= 1;
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();
            }
            break;
        
        case TJC_PAGE_FILE_LIST_1_BTN_NEXT:
            if (page_files_current_pages < page_files_pages) {
                // std::cout << "############################" << page_files_current_pages << std::endl;
                // page_files_current_pages = (page_files_current_pages + 1) % (page_files_pages + 1);
                page_files_current_pages += 1;
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();
            }
            break;

        default:
            break; 
        }
        break;

    case TJC_PAGE_FILE_LIST_2:
        switch (widget_id)
        {
        case TJC_PAGE_FILE_LIST_2_BTN_1:
            get_sub_dir_files_list(0);
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_2:
            get_sub_dir_files_list(1);
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_3:
            get_sub_dir_files_list(2);
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_4:
            get_sub_dir_files_list(3);
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_5:
            get_sub_dir_files_list(4);
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_6:
            get_sub_dir_files_list(5);
            break;

        /*
        case TJC_PAGE_FILE_LIST_2_BTN_7:
            get_sub_dir_files_list(6);
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_8:
            get_sub_dir_files_list(7);
            break;
        */

        case TJC_PAGE_FILE_LIST_2_BTN_BACK:
            // page_files_folder_layers--;
            get_parenet_dir_files_list();
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_PREVIOUS:
            if (page_files_current_pages > 0) {
                // std::cout << "############################" << page_files_current_pages << std::endl;
                // page_files_current_pages = (page_files_current_pages + page_files_pages) % (page_files_pages + 1);
                page_files_current_pages -= 1;
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_2();
            }
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_NEXT:
            if (page_files_current_pages < page_files_pages) {
                // std::cout << "############################" << page_files_current_pages << std::endl;
                // page_files_current_pages = (page_files_current_pages + 1) % (page_files_pages + 1);
                page_files_current_pages += 1;
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_2();
            }
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;
        
        case TJC_PAGE_FILE_LIST_2_BTN_FILE:
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_TOOL:
            page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_FILE_LIST_2_BTN_SERVICE:
            // page_to(TJC_PAGE_SERVICE);
            go_to_about();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_DELETE_FILE:
        switch (widget_id)
        {
        case TJC_PAGE_DELETE_FILE_YES:
            delete_file(page_files_root_path + page_files_print_files_path);
            get_parenet_dir_files_list();
            clear_page_preview();
            show_preview_complete = false;
            clear_cp0_image();
            break;

        case TJC_PAGE_DELETE_FILE_NO:
            page_to(TJC_PAGE_PREVIEW);
            show_preview_complete = false;
            break;
        }
        break;
    
    case TJC_PAGE_PREVIEW:
        switch (widget_id)
        {
        case TJC_PAGE_PREVIEW_BTN_BACK:
            if (mks_file_parse_finished == false) {
                get_parenet_dir_files_list();
                clear_page_preview();
                show_preview_complete = false;
                clear_cp0_image();
            } else {
                if (show_preview_complete == true) {        // 当且仅当预览加载完成才可以按下按钮
                    get_parenet_dir_files_list();
                    clear_page_preview();                   // 返回时清除读取的数据
                    show_preview_complete = false;
                    clear_cp0_image();
                }
            }
            break;

        case TJC_PAGE_PREVIEW_BTN_DELETE:
            if (show_preview_complete == true) {        // 当且仅当预览加载完成才可以按下按钮
                page_to(TJC_PAGE_DELETE_FILE);
                // delete_file(page_files_root_path + page_files_print_files_path);
                // get_parenet_dir_files_list();
                // show_preview_complete = false;
                // clear_cp0_image();
            }
            break;

        case TJC_PAGE_PREVIEW_BTN_START:
            // std::cout << "文件路径：" << page_files_print_files_path << std::endl;
            // printer_print_stats_state = "Printing";
            // start_printing(page_files_print_files_path);
            // page_to(TJC_PAGE_PRINTING);
            if (show_preview_complete == true) {        // 当且仅当预览加载完成才可以按下按钮
                //2023.5.8 CLL  去除清理X轴动画
                //if (get_mks_total_printed_time() > 48000) {
                //    page_to(TJC_PAGE_CLEAR_X_POP);
                //} else {
                    if (get_filament_detected_enable() == true) {
                        if (get_filament_detected() == true) {
                            //2023.4.24-2 实现打印过文件标红
                            printed_file_path = page_files_print_files_path;
                            // printer_print_stats_state = "Printing";
                            std::cout << "没有检测到断料" << std::endl;
                            //1.1.6 CLL 打印前判断耗材种类并弹窗
                            check_filament_type();
                            //page_to(TJC_PAGE_PRINTING);
                            printer_print_stats_state = "Printing";
                            print_start();  //2023.5.8 CLL 打印前发送"PRINT_START"指令
                            start_printing(page_files_print_files_path);
                        } else {
                            std::cout << "检测到断料" << std::endl;
                            page_to(TJC_PAGE_FILAMENT_ERROR);
                        }
                    } else {
                        //2023.4.24-2 实现打印过文件标红
                        printed_file_path = page_files_print_files_path;
                        //1.1.6 CLL 打印前判断耗材种类并弹窗
                        check_filament_type();
                        //page_to(TJC_PAGE_PRINTING);
                        printer_print_stats_state = "Printing";
                        print_start();  //2023.5.8 CLL 打印前发送"PRINT_START"指令 
                        start_printing(page_files_print_files_path);
                    }
                //}
                show_preview_complete = false;
            }
            
            break;
        
        //1.1.4 CLL 新增打印热床调平
        case TJC_PAGE_PREVIEW_BED_LEVELING:
            if (printer_bed_leveling == true) {
                bed_leveling_switch(false);
            }else {
                bed_leveling_switch(true);
            }
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_ERROR:
        switch (widget_id)
        {
        case TJC_PAGE_FILAMENT_ERROR_OK:
            page_to(TJC_PAGE_FILAMENT);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINTING:
        switch (widget_id)
        {
        case TJC_PAGE_PRINTING_BTN_EXTRUDER:
            printing_keyboard_enabled = true;
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_BTN_HEATER_BED:
            printing_keyboard_enabled = true;
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_BTN_SPEED:
            printing_keyboard_enabled = true;
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_BTN_FLOW:
            printing_keyboard_enabled = true;
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_BTN_Z_OFFSET:
            page_to(TJC_PAGE_PRINT_BABYSTEP);
            set_intern_zoffset(0.05);
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_BTN_PAUSE_RESUME:
            //2023.5.10 CLL 修复页面跳转bug
            printer_ready = false;
            set_print_pause();
            //2023.4.24-3 使网页操作与UI界面相匹配
            page_to(TJC_PAGE_PRINT_FILAMENT);
            
            clear_page_printing_arg();
            /*
            if (get_print_pause_resume() == false) {
                set_print_pause();
                page_to(TJC_PAGE_PRINT_FILAMENT);
                clear_page_printing_arg();
            } else {
                set_print_resume();
            }
            */
            break;

        case TJC_PAGE_PRINTING_BTN_STOP:
            page_to(TJC_PAGE_PRINT_STOP);
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINTING_BTN_LIGHT:
            led_on_off();
            break;

        case TJC_PAGE_PRINTING_BTN_FAN_0:
            MKSLOG_RED("在这里设置风扇速度");
            printing_keyboard_enabled = true;
            clear_page_printing_arg();
            break;

        //1.1.6 CLL 新增fan3
        case TJC_PAGE_PRINTING_BTN_FAN_3:
            printing_keyboard_enabled = true;
            clear_page_printing_arg();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_BABYSTEP:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_BABYSTEP_001:
            set_intern_zoffset(0.01);
            break;
        
        case TJC_PAGE_PRINT_BABYSTEP_005:
            set_intern_zoffset(0.05);
            break;
        
        case TJC_PAGE_PRINT_BABYSTEP_01:
            set_intern_zoffset(0.1);
            break;
        
        case TJC_PAGE_PRINT_BABYSTEP_1:
            set_intern_zoffset(1);
            break;
        
        case TJC_PAGE_PRINT_BABYSTEP_UP:
            set_zoffset(false);
            break;
        
        case TJC_PAGE_PRINT_BABYSTEP_DOWN:
            set_zoffset(true);
            break;
        
        case TJC_PAGE_PRINT_BABYSTEP_PAUSE_RESUME:
            //2023.5.10 CLL 修复页面跳转bug
            printer_ready = false;
            set_print_pause();
            page_to(TJC_PAGE_PRINT_FILAMENT);
            clear_page_printing_arg();
            break;
        
        case TJC_PAGE_PRINT_BABYSTEP_STOP:
            page_to(TJC_PAGE_PRINT_STOP);
            break;
        
        case TJC_PAGE_PRINT_BABYSTEP_BACK:
            printing_keyboard_enabled = false;
            page_to(TJC_PAGE_PRINTING);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_FILAMENT:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_FILAMENT_PAUSE_RESUME:
            MKSLOG_BLUE("get_filament_detected_enable: %d", get_filament_detected_enable());
            MKSLOG_BLUE("get_filament_detected: %d", get_filament_detected());
            //2023.4.24-3 使网页操作与UI界面相匹配
            printer_ready = false;
            if (get_filament_detected_enable() == true) {
                if (get_filament_detected() == true) {
                    page_to(TJC_PAGE_PRINTING);
                    set_print_resume();
                } else {
                    page_to(TJC_PAGE_PRINT_NO_FIL);
                }
            } else {
                //2023.4.24-3 使网页操作与UI界面相匹配
                page_to(TJC_PAGE_PRINTING);
                set_print_resume();
            }
            // if (get_print_pause_resume() == true) {
            
            // }
            break;

        case TJC_PAGE_PRINT_FILAMENT_STOP:
            page_to(TJC_PAGE_PRINT_STOP);
            clear_page_printing_arg();
            break;

        case TJC_PAGE_PRINT_FILAMENT_TARGET:
            set_print_filament_target();
            break;

        case TJC_PAGE_PRINT_FILAMENT_TARGET_UP:
            set_filament_extruder_target(true);
            break;

        case TJC_PAGE_PRINT_FILAMENT_TARGET_DOWN:
            set_filament_extruder_target(false);
            break;

        case TJC_PAGE_PRINT_FILAMENT_RETRACT:
            printer_idle_timeout_state = "Printing";
            page_print_filament_extrude_restract_button = true;
            start_retract();
            break;

        case TJC_PAGE_PRINT_FILAMENT_EXTRUDE:
            printer_idle_timeout_state = "Printing";
            page_print_filament_extrude_restract_button = true;
            start_extrude();
            break;

        case TJC_PAGE_PRINT_FILAMENT_10:
            set_print_filament_dist(10);
            break;

        case TJC_PAGE_PRINT_FILAMENT_20:
            set_print_filament_dist(20);
            break;

        case TJC_PAGE_PRINT_FILAMENT_50:
            set_print_filament_dist(50);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_NO_FIL:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_NO_FIL_YES:
            //1.1.4 在打印设置zoffset界面检测到断料也会跳转至断料界面
            if (previous_page_id == TJC_PAGE_PRINTING || previous_page_id == TJC_PAGE_PRINT_BABYSTEP) {
                page_to(TJC_PAGE_PRINT_FILAMENT);
            } else if (previous_page_id == TJC_PAGE_PREVIEW) {
                if (get_filament_detected() == false) {
                    page_to(TJC_PAGE_FILAMENT);
                }
            } else if (previous_page_id == TJC_PAGE_PRINT_FILAMENT) {
                page_to(TJC_PAGE_PRINT_FILAMENT);
            }
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_POP_3:
        switch (widget_id)
        {
        case TJC_PAGE_FILAMENT_POP_3_OK:
            page_to(TJC_PAGE_PRINT_FILAMENT);
            // set_print_filament_target();
            //2023.5.13 修复打印暂停界面弹窗与装载界面弹窗不一致
            //set_extruder_target(230);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_MOVE:
        switch (widget_id)
        {
        case TJC_PAGE_MOVE_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();
                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            break;

        case TJC_PAGE_MOVE_BTN_HOME:
            // set_move_dist(0.1);             // 恢复步长为0.1
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_MOVE_BTN_SERVICE:
            // set_move_dist(0.1);             // 恢复步长为0.1
            page_to(TJC_PAGE_LANGUAGE);
            break;

        case TJC_PAGE_MOVE_FILAMENT:
            // set_move_dist(0.1);             // 恢复步长为0.1
            page_to(TJC_PAGE_FILAMENT);
            break;

        case TJC_PAGE_MOVE_NEXT:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            break;

        case TJC_PAGE_MOVE_01:
            set_move_dist(0.1);
            break;

        case TJC_PAGE_MOVE_1:
            set_move_dist(1.0);
            break;

        case TJC_PAGE_MOVE_10:
            set_move_dist(10.0);
            break;

        case TJC_PAGE_MOVE_X_UP:
            move_x_increase();
            break;

        case TJC_PAGE_MOVE_X_DOWN:
            move_x_decrease();
            break;

        case TJC_PAGE_MOVE_Y_UP:
            move_y_increase();
            break;

        case TJC_PAGE_MOVE_Y_DOWN:
            move_y_decrease();
            break;

        case TJC_PAGE_MOVE_HOME:
            move_home();
            break;

        case TJC_PAGE_MOVE_Z_UP:
            move_z_decrease();
            break;

        case TJC_PAGE_MVOE_Z_DOWN:
            move_z_increase();
            break;

        case TJC_PAGE_MOVE_M84:
            move_motors_off();
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_POP_2:
        switch (widget_id)
        {
        case TJC_PAGE_POP_2_YES:
            if (previous_page_id == TJC_PAGE_MOVE) {
                // set_move_dist(0.1);
                page_to(TJC_PAGE_MOVE);
            } else if (previous_page_id == TJC_PAGE_AUTO_LEVEL) {
                std::cout << "按下确定键" << std::endl;
                page_to(TJC_PAGE_MAIN);
            } else if (previous_page_id == TJC_PAGE_PRINTING || current_page_id == TJC_PAGE_PRINT_BABYSTEP || current_page_id == TJC_PAGE_PRINT_FILAMENT) {
                page_to(TJC_PAGE_MAIN);
            } else {
                std::cout << "TJC_PAGE_POP_2_YES" << std::endl;
                page_to(previous_page_id);
            }
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_LEVEL_MODE_1:
        switch (widget_id)
        {
        //2023.4.24-5 修改调平数据显示
        case TJC_PAGE_LEVEL_MODE_1_ZOFFSET:
            page_to(TJC_PAGE_ZOFFSET);
            break;

        case TJC_PAGE_LEVEL_MODE_1_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;
        
        case TJC_PAGE_LEVEL_MODE_1_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();
                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            break;
        
        case TJC_PAGE_LEVEL_MODE_1_BTN_TOOL:
            page_to(TJC_PAGE_MOVE);
            break;
        
        case TJC_PAGE_LEVEL_MODE_1_BTN_SERVICE:
            page_to(TJC_PAGE_LANGUAGE);
            break;

        case TJC_PAGE_LEVEL_MODE_1_PREVIOUS:
            page_to(TJC_PAGE_FILAMENT);
            break;

        case TJC_PAGE_LEVEL_MODE_1_NETWORK:
            // page_to(TJC_PAGE_INTERNET);
            go_to_network();
            break;

        //2023.5.3 修改自动调平跳转
        case TJC_PAGE_LEVEL_MODE_1_AUTO_LEVEL:
            level_mode = TJC_PAGE_AUTO_LEVEL;
            page_to(TJC_PAGE_AUTO_INIT_1);
            break;

        case TJC_PAGE_LEVEL_MODE_1_MANUAL_LEVEL:
            level_mode = TJC_PAGE_MANUAL_LEVEL;
            page_to(TJC_PAGE_LEVEL_INIT_SHOUDONG);
            break;

        case TJC_PAGE_LEVEL_MODE_1_NEXT:
            page_to(TJC_PAGE_LEVEL_MODE_2);
            break;
        
        //2023.4.29 合并两页校准页面
        case TJC_PAGE_LEVEL_MODE_1_SYNTONY:
            go_to_syntony_move();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_LEVEL_MODE_2:
        switch (widget_id)
        {
        case TJC_PAGE_LEVEL_MODE_2_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();
                get_object_status();
            //} else {
            //   page_to(TJC_PAGE_DISK_DETECT);
            //}
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_BTN_SERVICE:
            page_to(TJC_PAGE_LANGUAGE);
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_BTN_TOOL:
            page_to(TJC_PAGE_MOVE);
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_PREVIOUS:
            page_to(TJC_PAGE_FILAMENT);
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_NETWORK:
            // page_to(TJC_PAGE_INTERNET);
            go_to_network();
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_PRINT_LEVEL:
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_SYNTONY:
            go_to_syntony_move();
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_PID:
            break;
        
        case TJC_PAGE_LEVEL_MODE_2_FIRST:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_MOVE_POP:
        switch (widget_id)
        {
        case TJC_PAGE_MOVE_POP_YES:
            //2023.5.9 CLL 修复界面显示bug
            page_to(TJC_PAGE_MOVE);
            move_home();
            set_move_dist(0.1);             // 恢复步长为0.1
            //page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_MOVE_POP_NO:
            //2023.5.9 CLL 修复界面显示bug
            page_to(TJC_PAGE_MOVE);
            set_move_dist(0.1);             // 恢复步长为0.1
            //page_to(TJC_PAGE_MOVE);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT:
        switch (widget_id)
        {
        case TJC_PAGE_FILAMENT_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();
            //} else {
            //   page_to(TJC_PAGE_DISK_DETECT);
            //}
            break;

        case TJC_PAGE_FILAMENT_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_FILAMENT_BTN_SERVICE:
            // page_to(TJC_PAGE_LANGUAGE);
            go_to_about();
            break;
        
        case TJC_PAGE_FILAMENT_MOVE:
            // set_move_dist(0.1);             // 恢复步长为0.1
            page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_FILAMENT_BTN_10:
            set_print_filament_dist(10);
            break;

        case TJC_PAGE_FILAMENT_BTN_20:
            set_print_filament_dist(20);
            break;

        case TJC_PAGE_FILAMENT_BTN_50:
            set_print_filament_dist(50);
            break;

        case TJC_PAGE_FILAMENT_UNLOAD:
            //2023.5.12 CLL 点击退料解锁电机
            move_motors_off();
            //2023.4.28 修改退料流程
            //page_to(TJC_PAGE_FILAMENT_POP_2);
            if ((printer_extruder_temperature >= (printer_extruder_target - 5))&&(printer_extruder_temperature <= (printer_extruder_target + 5))) {
                //2023.5.12 新增退料完成页面
                page_filament_unload_button = true;
                page_to(TJC_PAGE_FILAMENT_POP_4);
            }else {
                page_to(TJC_PAGE_FILAMENT_POP);
            // filament_unload();
            }
            break;

        case TJC_PAGE_FILAMENT_BTN_EXTRUDE:
            printer_idle_timeout_state = "Printing";
            page_filament_extrude_button = true;
            
            start_extrude();
            break;

        case TJC_PAGE_FILAMENT_BTN_EXTRUDER:
            // filament_keyboard_enabled = true;
            page_to(TJC_PAGE_FILAMENT_KB);
            break;

        case TJC_PAGE_FILAMENT_BTN_HEATER_BED:
            // filament_keyboard_enabled = true;
            page_to(TJC_PAGE_FILAMENT_KB);
            break;

        case TJC_PAGE_FILAMENT_BTN_FAN_1:
            std::cout << "按下第一个风扇" << std::endl;
            // filament_keyboard_enabled = true;
            filament_fan0();
            // page_to(TJC_PAGE_KB_FILAMENT_1);
            break;
        
        //1.1.6 CLL 新增fan3
        case TJC_PAGE_FILAMENT_BTN_FAN_3:
            filament_fan3();
            break;

        case TJC_PAGE_FILAMENT_BTN_FILAMENT_SENSOR:
            set_filament_sensor();
            break;

        case TJC_PAGE_FILAMENT_EXTRUDER:
            filament_extruder_target();
            break;

        case TJC_PAGE_FILAMENT_HEATER_BED:
            filament_heater_bed_target();
            break;
        
        case TJC_PAGE_FILAMENT_NEXT:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            break;
        
        }
        break;

    case TJC_PAGE_FILAMENT_KB:
        switch (widget_id)
        {
        case TJC_PAGE_FILAMENT_KB_BACK:
            page_to(TJC_PAGE_FILAMENT);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_POP:
        switch (widget_id)
        {
        
        case TJC_PAGE_FILAMENT_POP_YES:
            /*2023.4.28 修改退料流程
            // set_extruder_target(240);
            if (previous_page_id == TJC_PAGE_FILAMENT_POP_2) {
                filament_unload();
                page_to(TJC_PAGE_FILAMENT);
            } else {
                set_extruder_target(230);       // 按照加热要求加热到240度
                page_filament_extrude_button = false;

                page_to(TJC_PAGE_FILAMENT);
            }
            */
            //1.1.4 CLL 修复点击装料界面下料按钮后动画bug
            printer_idle_timeout_state = "Ready";
            page_to(TJC_PAGE_FILAMENT);
            break;
            
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT_POP_2:
        switch (widget_id)
        {
        case TJC_PAGE_FILAMENT_POP_2_YES:
            // filament_unload();
            // page_to(TJC_PAGE_PRINT_FILAMENT);
            filament_pop_2_yes();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_LEVEL_INIT_1:
        switch (widget_id)
        {
        case TJC_PAGE_LEVEL_INIT_1_BTN_BACK:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            break;

        case TJC_PAGE_LEVEL_INIT_1_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            break;

        case TJC_PAGE_LEVEL_INIT_1_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_LEVEL_INIT_1_BTN_SERVICE:
            page_to(TJC_PAGE_LANGUAGE);
            // go_to_about();
            break;

        case TJC_PAGE_LEVEL_INIT_1_ENTER:
            // page_to(TJC_PAGE_LEVELING_INIT);
            switch (level_mode)
            {
            case TJC_PAGE_AUTO_LEVEL:
                auto_level_button_enabled = true;
                printer_idle_timeout_state = "Printing";
                pre_auto_level_init();
                MKSLOG_BLUE("跳到这里");
                break;

            case TJC_PAGE_MANUAL_LEVEL:
                printer_idle_timeout_state = "Printing";
                pre_manual_level_init();
                break;
            
            default:
                break;
            }
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_LEVEL_INIT_SHOUDONG:
        switch (widget_id)
        {
        case TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_BACK:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            break;
        
        case TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            break;
        
        case TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_ENTER:
            switch (level_mode)
            {
            case TJC_PAGE_AUTO_LEVEL:
                auto_level_button_enabled = true;
                printer_idle_timeout_state = "Printing";
                pre_auto_level_init();
                MKSLOG_BLUE("跳到这里");
                break;

            case TJC_PAGE_MANUAL_LEVEL:
                printer_idle_timeout_state = "Printing";
                pre_manual_level_init();
                break;
            
            default:
                break;
            }
            break;

        case TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_SERVICE:
            page_to(TJC_PAGE_LANGUAGE);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_AUTO_LEVEL:
        switch (widget_id)
        {
        case TJC_PAGE_AUTO_LEVEL_001:
            set_auto_level_dist(0.01);
            break;

        case TJC_PAGE_AUTO_LEVEL_005:
            set_auto_level_dist(0.05);
            break;
        
        case TJC_PAGE_AUTO_LEVEL_01:
            set_auto_level_dist(0.1);
            break;

        case TJC_PAGE_AUTO_LEVEL_1:
            set_auto_level_dist(1);
            break;
        
        case TJC_PAGE_AUTO_LEVEL_UP:
            start_auto_level_dist(false);
            break;

        case TJC_PAGE_AUTO_LEVEL_DOWN:
            start_auto_level_dist(true);
            break;

        case TJC_PAGE_AUTO_LEVEL_ENTER:
            page_to(TJC_PAGE_AUTO_LEVELING);
            start_auto_level();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_AUTO_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_AUTO_FINISH_YES:
            if (auto_level_button_enabled == true) {
                auto_level_button_enabled = false;
                std::cout << "自动调平已完成" << std::endl;
                finish_auto_level();
            }
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_MANUAL_LEVEL:
        switch (widget_id)
        {
        case TJC_PAGE_MANUAL_LEVEL_001:
            set_manual_level_dist(0.01);
            break;
        
        case TJC_PAGE_MANUAL_LEVEL_005:
            set_manual_level_dist(0.05);
            break;
        
        case TJC_PAGE_MANUAL_LEVEL_01:
            set_manual_level_dist(0.1);
            break;
        
        case TJC_PAGE_MANUAL_LEVEL_1:
            set_manual_level_dist(1);
            break;
        
        case TJC_PAGE_MANUAL_LEVEL_ENTER:
            start_manual_level();
            break;
        
        case TJC_PAGE_MANUAL_LEVEL_UP:
            start_manual_level_dist(false);
            break;
        
        case TJC_PAGE_MANUAL_LEVEL_DOWN:
            start_manual_level_dist(true);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_MANUAL_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_MANUAL_FINISH_YES:
            std::cout << "手动调平已完成" << std::endl;
            finish_manual_level();
            break;
        default:
            break;
        }
        break;


    case TJC_PAGE_WIFI_LIST_2:
        switch (widget_id)
        {
        case TJC_PAGE_WIFI_LIST_2_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (0 == detect_disk()) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();

                mks_wpa_cli_close_connection();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            
            break;
        
        case TJC_PAGE_WIFI_LIST_2_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            mks_wpa_cli_close_connection();
            break;

        case TJC_PAGE_WIFI_LIST_2_BTN_SERVICE:
            page_to(TJC_PAGE_LANGUAGE);
            mks_wpa_cli_close_connection();
            break;

        case TJC_PAGE_WIFI_LIST_2_BTN_TOOL:
            page_to(TJC_PAGE_MOVE);
            mks_wpa_cli_close_connection();
            break;

        case TJC_PAGE_WIFI_LIST_2_LEVEL:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            mks_wpa_cli_close_connection();
            break;

        case TJC_PAGE_WIFI_LIST_2_PREVIOUS:
            // std::cout << "page_wifi_current_pages = " << page_wifi_current_pages << std::endl;
            // std::cout << "page_wifi_ssid_list_pages = " << page_wifi_ssid_list_pages << std::endl;
            if (page_wifi_current_pages > 0) {
                std::cout << "page_wifi_current_pages = " << page_wifi_current_pages << std::endl;
                std::cout << "page_wifi_ssid_list_pages = " << page_wifi_ssid_list_pages << std::endl;
                page_wifi_current_pages -= 1;
                set_page_wifi_ssid_list(page_wifi_current_pages);
                refresh_page_wifi_list();
            }
            break;

        case TJC_PAGE_WIFI_LIST_2_NEXT:
            // std::cout << "page_wifi_current_pages = " << page_wifi_current_pages << std::endl;
            // std::cout << "page_wifi_ssid_list_pages = " << page_wifi_ssid_list_pages << std::endl;
            if (page_wifi_current_pages < page_wifi_ssid_list_pages - 1) {
                std::cout << "page_wifi_current_pages = " << page_wifi_current_pages << std::endl;
                std::cout << "page_wifi_ssid_list_pages = " << page_wifi_ssid_list_pages << std::endl;
                page_wifi_current_pages += 1;
                set_page_wifi_ssid_list(page_wifi_current_pages);
                refresh_page_wifi_list();
            }
            break;

        case TJC_PAGE_WIFI_LIST_2_SSID_1:
            if (page_wifi_list_ssid_button_enabled[0] == true) {
                get_wifi_list_ssid(0);
                printing_wifi_keyboard_enabled = true;
                page_to(TJC_PAGE_WIFI_KEYBOARD);
            } else {

            }
            break;

        case TJC_PAGE_WIFI_LIST_2_SSID_2:
            if (true == page_wifi_list_ssid_button_enabled[1]) {
                get_wifi_list_ssid(1);
                printing_wifi_keyboard_enabled = true;
                page_to(TJC_PAGE_WIFI_KEYBOARD);
            }
            break;

        case TJC_PAGE_WIFI_LIST_2_SSID_3:
            if (true == page_wifi_list_ssid_button_enabled[2]) {
                get_wifi_list_ssid(2);
                printing_wifi_keyboard_enabled = true;
                page_to(TJC_PAGE_WIFI_KEYBOARD);
            }
            break;


        case TJC_PAGE_WIFI_LIST_2_REFRESH:
            std::cout << "################## 按下刷新按钮" << std::endl;
            scan_ssid_and_show();
            //1.1.4 CLL 修复wifi页面bug
            std::cout << "等待延时测试3s..." << std::endl;
            sleep(3);
            scan_ssid_and_show();
            break;
            break;

        case TJC_PAGE_WIFI_LIST_2_BACK:
            page_to(TJC_PAGE_MOVE);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_INTERNET:
        switch (widget_id)
        {
        case TJC_PAGE_INTERNET_AUTO_LEVEL:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            break;
        
        case TJC_PAGE_INTERNET_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            
            break;
        
        case TJC_PAGE_INTERNET_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_INTERNET_BTN_SERVICE:
            page_to(TJC_PAGE_LANGUAGE);
            // go_to_about();
            break;

        case TJC_PAGE_INTERNET_BTN_PREVIOUS:
            page_to(TJC_PAGE_MOVE);
            break;

        }
        break;

    case TJC_PAGE_LANGUAGE:
        switch (widget_id)
        {
        case TJC_PAGE_LANGUAGE_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();
                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            break;
        
        case TJC_PAGE_LANGUAGE_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_LANGUAGE_BTN_TOOL:
            page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_LANGUAGE_SERVICE:
            page_to(TJC_PAGE_SERVICE);
            break;

        case TJC_PAGE_LANGUAGE_NEXT:
            go_to_reset();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_SERVICE:
        switch (widget_id)
        {
        case TJC_PAGE_SERVICE_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            break;
        
        case TJC_PAGE_SERVICE_BTN_HOME:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_SERVICE_BTN_TOOL:
            page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_SERVICE_LANGUAGE:
            page_to(TJC_PAGE_LANGUAGE);
            break;

        case TJC_PAGE_SERVICE_NEXT:
            go_to_reset();
            break;
        
        default:
            break;
        }
        break;

    //2023.4.28 修复重启后仍显示加热
    case TJC_PAGE_RESET:
        switch (widget_id)
        {
        case TJC_PAGE_RESET_BTN_FILE:
            /*
            if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();
                get_object_status();
            } else {
                page_to(TJC_PAGE_DISK_DETECT);
            }
            */
            break;
        
        case TJC_PAGE_RESET_BTN_HOME:
            //page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_RESET_BTN_TOOL:
            //page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_RESET_ABOUT:
            go_to_about();
            break;

        case TJC_PAGE_RESET_RESTART_KLIPPER:
            reset_klipper();
            break;

        case TJC_PAGE_RESET_FIRMWARE_RESTART:
            reset_firmware();
            break;

        case TJC_PAGE_RESET_PREVIOUS:
            //page_to(TJC_PAGE_LANGUAGE);
            //get_object_status();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_SYS_OK:
        switch (widget_id)
        {
        case TJC_PAGE_SYS_OK_HOME:
            page_to(TJC_PAGE_MAIN);
            get_object_status();
            break;

        case TJC_PAGE_SYS_OK_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            get_object_status();
            break;

        case TJC_PAGE_SYS_OK_TOOL:
            page_to(TJC_PAGE_MOVE);
            get_object_status();
            break;

        case TJC_PAGE_SYS_OK_ABOUT:
            go_to_about();
            get_object_status();
            break;

        case TJC_PAGE_SYS_OK_PREVIOUS:
            page_to(TJC_PAGE_LANGUAGE);
            get_object_status();
            break;
        
        default:
            break;
        }
        break;

    //2023.4.28 隐藏开机引导
    //2023.5.10 CLL 修复页面跳转bug
    case TJC_PAGE_ABOUT:
        switch (widget_id)
        {
        case TJC_PAGE_ABOUT_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();
            }
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            
            break;
        
        case TJC_PAGE_ABOUT_BTN_HOME:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
                get_object_status();
            }
            break;

        case TJC_PAGE_ABOUT_BTN_TOOL:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MOVE);
                get_object_status();
            }
            break;

        case TJC_PAGE_ABOUT_RESET:
                go_to_reset();
            break;

        case TJC_PAGE_ABOUT_OOBE:
            std::cout << "打开oobe开关" << std::endl;
            if (get_mks_oobe_enabled() == true) {
                set_mks_oobe_enabled(false);
                send_cmd_picc(tty_fd, "b7", "153");
            } else {
                set_mks_oobe_enabled(true);
                send_cmd_picc(tty_fd, "b7", "157");
            }
            break;

        case TJC_PAGE_ABOUT_BACK:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_LANGUAGE);
                get_object_status();
            }
            break;

        case TJC_PAGE_ABOUT_S_BTN:
            set_mks_oobe_enabled(true);
            break;

        default:
            break;
        }
        break;
    
    //2023.4.28 隐藏开机引导
    case TJC_PAGE_ABOUT_UPDATE:
        switch (widget_id)
        {
        case TJC_PAGE_ABOUT_UPDATE_BTN_FILE:
            //2023.5.9 CLL 无论是否插入U盘均显示所有文件
            //if (detect_disk() == 0) {
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_FILE_LIST_1);
                // printer_set_babystep();
                page_files_pages = 0;
                page_files_current_pages = 0;
                page_files_folder_layers = 0;
                page_files_previous_path = "";
                page_files_root_path = DEFAULT_DIR;
                page_files_path = "";
                refresh_page_files(page_files_current_pages);
                refresh_page_files_list_1();

                get_object_status();
            }
            //} else {
            //    page_to(TJC_PAGE_DISK_DETECT);
            //}
            
            break;

        //2023.5.10 CLL 修复页面跳转bug
        case TJC_PAGE_ABOUT_UPDATE_BTN_HOME:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MAIN);
                get_object_status();
            }
            break;

        case TJC_PAGE_ABOUT_UPDATE_BTN_TOOL:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_MOVE);
                get_object_status();
            }
            break;

        case TJC_PAGE_ABOUT_UPDATE_UPDATE:
                disable_page_about_successed();
                start_update();
            break;

        case TJC_PAGE_ABOUT_UPDATE_OOBE:
                std::cout << "开机引导页面" << std::endl;
                if (get_mks_oobe_enabled() == true) {
                    set_mks_oobe_enabled(false);
                    send_cmd_picc(tty_fd, "b7", "153");
                } else {
                    set_mks_oobe_enabled(true);
                    send_cmd_picc(tty_fd, "b7", "157");
                }
            break;

        case TJC_PAGE_ABOUT_UPDATE_RESET:
                go_to_reset();
            break;

        case TJC_PAGE_ABOUT_UPDATE_BACK:
            if (printer_webhooks_state != "shutdown" && printer_webhooks_state != "error") {
                page_to(TJC_PAGE_LANGUAGE);
                get_object_status();
            }
            break;
        
        case TJC_PAGE_ABOUT_UPDATE_S_BTN:
                set_mks_oobe_enabled(true);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_FINISH:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_FINISH_YES:
            page_to(TJC_PAGE_MAIN);
            break;
        
        case TJC_PAGE_PRINT_FINISH_INIT:
            clear_previous_data(); //2023.5.4 CLL 清除原数据
            sleep(5);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_PRINT_STOP:
        switch (widget_id)
        {
        case TJC_PAGE_PRINT_STOP_YES:
            // if (start_to_printing == false) {
                // start_to_printing = true;
                page_to(TJC_PAGE_PRINT_STOPING);
                cancel_print();
                // get_total_time();
            // }
            break;

        case TJC_PAGE_PRINT_STOP_NO:
            page_to(TJC_PAGE_PRINTING);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_UPDATED:
        switch (widget_id)
        {
        case TJC_PAGE_UPDATED_ENTER:
            if (previous_page_id == TJC_PAGE_LOGO) {
                finish_tjc_update();
                page_to(TJC_PAGE_MAIN);
            }
            break;

        default:
            break;
        }

    case TJC_PAGE_SAVING:
        break;

    case TJC_PAGE_SET_TEMP_LEVEL:
        switch (widget_id)
        {
        case TJC_PAGE_SET_TEMP_LEVEL_HOME:
            break;

        case TJC_PAGE_SET_TEMP_LEVEL_FILE:
            break;

        case TJC_PAGE_SET_TEMP_LEVEL_TOOL:
            break;

        case TJC_PAGE_SET_TEMP_LEVEL_SERVICE:
            break;

        case TJC_PAGE_SET_TEMP_LEVEL_BACK:
            break;

        case TJC_PAGE_SET_TEMP_LEVEL_EXTRUDER:
            break;

        case TJC_PAGE_SET_TEMP_LEVEL_HOT_BED:
            break;

        case TJC_PAGE_SET_TEMP_LEVEL_ENTER:
            // level_mode_printing_set_target();
            // level_mode_printing_is_printing_level = true;
            // printer_print_stats_state = "printing";
            // level_mode_printing_print_file();
            // page_to(TJC_PAGE_LEVEL_PRINT);
            break;

        case TJC_PAGE_LEVEL_PRINT:
            break;

        case TJC_PAGE_LEVEL_STOP:
            break;

        case TJC_PAGE_CHOOSE_MODEL:
            switch (widget_id)
            {
            /*
            case TJC_PAGE_CHOOSE_MODEL_01:
                std::cout << "选择了第1个 zoffset" << std::endl;
                break;

            case TJC_PAGE_CHOOSE_MODEL_02:
                std::cout << "选择了第2个 zoffset" << std::endl;
                break;

            case TJC_PAGE_CHOOSE_MODEL_03:
                std::cout << "选择了第3个 zoffset" << std::endl;
                break;

            case TJC_PAGE_CHOOSE_MODEL_04:
                std::cout << "选择了第4个 zoffset" << std::endl;
                break;

            case TJC_PAGE_CHOOSE_MODEL_05:
                std::cout << "选择了第5个 zoffset" << std::endl;
                break;

            case TJC_PAGE_CHOOSE_MODEL_06:
                std::cout << "选择了第6个 zoffset" << std::endl;
                break;

            case TJC_PAGE_CHOOSE_MODEL_07:
                std::cout << "选择了第7个 zoffset" << std::endl;
                break;

            case TJC_PAGE_CHOOSE_MODEL_08:
                std::cout << "选择了第8个 zoffset" << std::endl;
                break;

            case TJC_PAGE_CHOOSE_MODEL_09:
                std::cout << "选择了第9个 zoffset" << std::endl;
                break;
            */

            default:
                break;
            }
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_PIDING:
        break;

    case TJC_PAGE_PID_FINISH:
        break;

    case TJC_PAGE_DISK_DETECT:
        switch (widget_id)
        {
        case TJC_PAGE_DISK_DETECT_HOME:
            page_to(TJC_PAGE_MAIN);
            break;

        case TJC_PAGE_DISK_DETECT_TOOL:
            page_to(TJC_PAGE_MOVE);
            break;

        case TJC_PAGE_DISK_DETECT_SERVICE:
            // page_to(TJC_PAGE_SERVICE);
            go_to_about();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_WIFI_SUCCESS:
        switch (widget_id)
        {
        case TJC_PAGE_WIFI_SUCCESS_YES:
            wifi_save_config();
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_WIFI_FAILED:
        switch (widget_id)
        {
        case TJC_PAGE_WIFI_FAILED_YES:
            page_to(TJC_PAGE_WIFI_LIST_2);
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_CLEAR_X_POP:
        switch (widget_id)
        {
        case TJC_PAGE_CLEAR_X_POP_YES:
            page_to(TJC_PAGE_CLEAR_X_VIDEO);
            break;

        case TJC_PAGE_CLEAR_X_POP_NO:
            do_not_x_clear();
            page_to(TJC_PAGE_PREVIEW);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_CLEAR_X_VIDEO:
        switch (widget_id)
        {
        case TJC_PAGE_CLEAR_X_VIDEO_NEXT:
            // 暂时用page_to(main)
            // page_to(TJC_PAGE_MAIN);
            do_x_clear();

            go_to_syntony_move();       // 按照要求跳转完成后跳到共振补偿
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_WIFI_KEYBOARD:
        switch (widget_id)
        {
        case TJC_PAGE_WIFI_KEYBOARD_BACK:
            page_to(TJC_PAGE_WIFI_LIST_2);
            printing_wifi_keyboard_enabled = false;
            break;

        case TJC_PAGE_WIFI_KEYBOARD_ENTER:
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_WAIT_POP:
        switch (widget_id)
        {
        case TJC_PAGE_WAIT_POP_OK:
            page_to(previous_page_id);
            break;
        
        default:
            break;
        }
        break;

    // case TJC_PAGE_FILAMENT_POP:
    //     switch (widget_id)
    //     {
    //     case TJC_PAGE_FILAMENT_POP_YES:
    //         page_to(TJC_PAGE_PRINT_FILAMENT);
    //         break;

    //     default:
    //         break;
    //     }
    //     break;

    case TJC_PAGE_UPDATE_FINISH:
        switch (widget_id)
        {
        default:
            break;
        }
        break;

    case TJC_PAGE_KEYDB:
        switch (widget_id)
        {
        case TJC_PAGE_KEYDB_BACK:
            printing_keyboard_enabled = false;
            break;

        case TJC_PAGE_KEYDB_PAUSE_RESUME:
            printing_keyboard_enabled = false;
            set_print_pause();
            page_to(TJC_PAGE_PRINT_FILAMENT);
            clear_page_printing_arg();
            break;
        
        case TJC_PAGE_KEYDB_STOP:
            printing_keyboard_enabled = false;
            page_to(TJC_PAGE_PRINT_STOP);
            clear_page_printing_arg();
            break;
        
        default:
            break;
        }
        break;

    //2023.4.24-5 修改调平数据显示
    case TJC_PAGE_ZOFFSET:
        switch (widget_id)
        {
        case TJC_PAGE_ZOFFSET_YES:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            break;        
        
        default:
            break;
        }
        break;

    //2023.4.28 修改退料流程
    case TJC_PAGE_FILAMENT_POP_4:
        switch(widget_id)
        {
        case TJC_PAGE_FILAMENT_POP_4_YES:
            filament_unload();
            //1.1.5 CLL 新增"退料中"界面
            page_to(TJC_PAGE_UNLOADING);
            //page_to(TJC_PAGE_FILAMENT);

        default:
            break;
        }
        break;

    //2023.5.3 修改自动调平页面跳转
    case TJC_PAGE_AUTO_INIT_1:
        switch(widget_id)
        {
        case TJC_PAGE_AUTO_INIT_1_DOWN:
            set_auto_level_heater_bed_target(false);
            break;

        case TJC_PAGE_AUTO_INIT_1_UP:
            set_auto_level_heater_bed_target(true);
            break;
        
        case TJC_PAGE_AUTO_INIT_1_HEATER_BED:
            filament_heater_bed_target();
            break;

        case TJC_PAGE_AUTO_INIT_1_BACK:
            page_to(TJC_PAGE_LEVEL_MODE_1);
            break;

        case TJC_PAGE_AUTO_INIT_1_ENTER:
            page_to(TJC_PAGE_AUTO_INIT_2);
            break;

        default:
            break;
        }
        break;

    //2023.5.3 修改自动调平页面跳转
    case TJC_PAGE_AUTO_INIT_2:
        switch(widget_id)
        {
        case TJC_PAGE_AUTO_INIT_2_BACK:
            page_to(TJC_PAGE_AUTO_INIT_1);
            break;

        case TJC_PAGE_AUTO_INIT_2_ENTER:
            auto_level_button_enabled = true;
            printer_idle_timeout_state = "Printing";
            pre_auto_level_init();
            MKSLOG_BLUE("跳到这里");
            break;

        default:
            break;
        }
        break;

    //2023.5.6 CLL 增加报错跳转页面
    case TJC_PAGE_DETECT_ERROR:
        switch(widget_id)
        {
        case TJC_PAGE_DETECT_ERROR_YES:
            page_to(TJC_PAGE_MAIN);
            break;
        
        default:
            break;
        }
        break;

    //2023.5.5 CLL gcode错误弹窗
    case TJC_PAGE_GCODE_ERROR:
        switch(widget_id)
        {
        case TJC_PAGE_GCODE_ERROR_YES:
            page_to(TJC_PAGE_PRINT_STOPING);
            cancel_print();
            break;
        
        default:
            break;
        }
        break;

    //2023.5.6 CLL 跳转页面后自动清除原数据
    case TJC_PAGE_PRINT_STOPING:
        switch(widget_id)
        {
        case TJC_PAGE_PRINT_STOPING_INIT:
            clear_previous_data();
            sleep(5);
            break;
        
        default:
            break;
        }
        break;

    //2023.5.8 开机引导新增界面
    case TJC_PAGE_OPEN_VIDEO_4:
        switch(widget_id)
        {
        case TJC_PAGE_OPEN_VIDEO_4_NEXT:
            //2023.5.10 CLL 新增开机引导界面
                page_to(TJC_PAGE_OPEN_HEATERBED);
            //pre_open_auto_level_init();
            break;

        default:
            break;
        }
        break;
    
    //2023.5.10 CLL 新增开机引导界面
    case TJC_PAGE_OPEN_HEATERBED:
        switch(widget_id)
        {
        case TJC_PAGE_OPEN_HEATERBED_DOWN:
            set_auto_level_heater_bed_target(false);
            break;
        
        case TJC_PAGE_OPEN_HEATERBED_UP:
            set_auto_level_heater_bed_target(true);
            break;
        
        case TJC_PAGE_OPEN_HEATERBED_HEATER_BED:
            filament_heater_bed_target();
            break;
            
        case TJC_PAGE_OPEN_HEATERBED_ENTER:
            pre_open_auto_level_init();
            break;
        
        default:
            break;
        }
        break;  

    //2023.5.10 CLL 开机引导调平
    case TJC_PAGE_OPEN_LEVEL_FINISH:
        switch(widget_id)
        {
        case TJC_PAGE_OPEN_LEVEL_FINISH_ENTER:
            open_go_to_syntony_move();
            break;

        default:
            break;
        }
        break;

    //2023.5.12 CLL 新增开机引导跳过确认页面
    case TJC_PAGE_OPEN_POP_1:
        switch(widget_id)
        {
        case TJC_PAGE_OPEN_POP_1_NO:
            page_to(TJC_PAGE_OPEN_LANGUAGE);
            break;
        
        case TJC_PAGE_OPEN_POP_1_YES:
            page_to(TJC_PAGE_MAIN);
            set_mks_oobe_enabled(false);
            break;

        default:
            break;
        }
        break;

    //2023.5.12 CLL 新增退料完成页面
    case TJC_PAGE_UNLOAD_FINISH:
        switch(widget_id)
        {
        case TJC_PAGE_UNLOAD_FINISH_YES:
            page_to(TJC_PAGE_FILAMENT);
            break;

        default:
            break;
        }
        break;

    //1.1.6 CLL 打印前判断耗材种类并弹窗
    case TJC_PAGE_PREVIEW_POP_1:
        switch(widget_id)
        {
        case TJC_PAGE_PREVIEW_POP_1_YES:
            page_to(TJC_PAGE_PRINTING);
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_PREVIEW_POP_2:
        switch (widget_id)
        {
        case TJC_PAGE_PREVIEW_POP_2_YES:
            page_to(TJC_PAGE_PRINTING);
            break;
        
        default:
            break;
        }
        break;

    default:
        break;
    }
}

void tjc_event_setted_handler(int page_id, int widget_id, unsigned char first, unsigned char second) {
    std::cout << "!!!" << page_id << std::endl;
    std::cout << "!!!" << widget_id << std::endl;
    std::cout << "!!!" << first << std::endl;
    std::cout << "!!!" << second << std::endl;
    int number = (int)((second << 8) + first);
    switch (page_id)
    {
    case TJC_PAGE_PRINTING:
        switch (widget_id)
        {
        
        case TJC_PAGE_PRINTING_BTN_EXTRUDER:
            //2023.4.28 修改打印时喷头温度上限为350
            if (number > 350) {
                number =350;
            }
            printing_keyboard_enabled = false;
            set_extruder_target(number);
            send_cmd_val(tty_fd, "n0", std::to_string(number));
            set_mks_extruder_target(number);
            break;

        case TJC_PAGE_PRINTING_BTN_HEATER_BED:
            //2023.4.28 修改打印时调整热床温度上限为120
            if (number >120) {
                number =120;
            }
            printing_keyboard_enabled = false;
            set_heater_bed_target(number);
            send_cmd_val(tty_fd, "n1", std::to_string(number));
            set_mks_heater_bed_target(number);
            break;

        
        case TJC_PAGE_PRINTING_BTN_FAN_0:
            if (number > 100) {
                number = 100;
            }
            page_to(TJC_PAGE_PRINTING);
            printing_keyboard_enabled = false;
            set_fan0(number);
            send_cmd_val(tty_fd, "n2", std::to_string(number));
            break;
        
        //1.1.6 CLL 新增fan3
        case TJC_PAGE_PRINTING_BTN_FAN_3:
            if (number > 100) {
                number = 100;
            }
            page_to(TJC_PAGE_PRINTING);
            printing_keyboard_enabled = false;
            set_fan3(number);
            send_cmd_val(tty_fd, "n3", std::to_string(number));
            break;

        case TJC_PAGE_PRINTING_BTN_FLOW:
            //2023.4.28 修改挤出速率上限为150
            if (number > 150) {
                number =150;
            }
            printing_keyboard_enabled = false;
            set_printer_flow(number);
            send_cmd_val(tty_fd, "n5", std::to_string(number));
            break;
        
        case TJC_PAGE_PRINTING_BTN_SPEED:
            //2023.4.28 修改移动速率上限为150
            if (number > 150) {
                number = 150;
            }
            printing_keyboard_enabled = false;
            set_printer_speed(number);
            send_cmd_val(tty_fd, "n4", std::to_string(number));
            break;
        
        default:
            break;
        }
        break;

    case TJC_PAGE_FILAMENT:
        switch (widget_id)
        {
        case TJC_PAGE_FILAMENT_BTN_EXTRUDER:
            set_extruder_target(number);
            set_mks_extruder_target(number);
            page_to(TJC_PAGE_FILAMENT);
            break;

        case TJC_PAGE_FILAMENT_BTN_HEATER_BED:
            set_heater_bed_target(number);
            set_mks_heater_bed_target(number);
            page_to(TJC_PAGE_FILAMENT);
            break;

        case TJC_PAGE_FILAMENT_BTN_FAN_1:
            break;

        case TJC_PAGE_FILAMENT_BTN_FILAMENT_SENSOR:
            break;

        default:
            break;
        }
        break;

    case TJC_PAGE_SET_TEMP_LEVEL:
        switch (widget_id)
        {
        case TJC_PAGE_SET_TEMP_LEVEL_EXTRUDER:
            level_mode_printing_extruder_target = number;
            page_to(TJC_PAGE_SET_TEMP_LEVEL);
            break;

        case TJC_PAGE_SET_TEMP_LEVEL_HOT_BED:
            level_mode_printing_heater_bed_target = number;
            page_to(TJC_PAGE_SET_TEMP_LEVEL);
            break;

        default:
            break;
        }
        break;
    
    default:
        break;
    }
}

void tjc_event_keyboard(char *cmd) {
    MKSLOG("收到的键盘返回的值为 %s\n", cmd);
    MKSLOG("cmd 1  %d\n", cmd[1]);
    MKSLOG("cmd 2  %d\n", cmd[2]);
    char *psk = &cmd[3];
    switch (cmd[1]) {
    case TJC_PAGE_WIFI_LIST_2:
        switch (cmd[2])
        {
        case TJC_PAGE_WIFI_LIST_2_SSID_1:
            MKSLOG_RED("收到密码, %d, 密码是%s", strlen(psk), psk);
            printing_wifi_keyboard_enabled = false;
            page_to(TJC_PAGE_WIFI_CONNECT);
            print_ssid_psk(psk);
            break;

        case TJC_PAGE_WIFI_LIST_2_SSID_2:
            MKSLOG_RED("收到密码, %d, 密码是%s", strlen(psk), psk);
            printing_wifi_keyboard_enabled = false;
            page_to(TJC_PAGE_WIFI_CONNECT);
            print_ssid_psk(psk);
            break;

        case TJC_PAGE_WIFI_LIST_2_SSID_3:
            MKSLOG_RED("收到密码, %d, 密码是%s", strlen(psk), psk);
            printing_wifi_keyboard_enabled = false;
            page_to(TJC_PAGE_WIFI_CONNECT);
            print_ssid_psk(psk);
            break;

        default:
            break;
        }
        break;
    }
}
