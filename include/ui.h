#ifndef UI_H
#define UI_H

#define TJC_PAGE_LOGO                       0

#define TJC_PAGE_RESTART                    1

#define TJC_PAGE_SHUTDOWN                   2

#define TJC_PAGE_POP_1                      3
#define TJC_PAGE_POP_1_YES                  0x01
#define TJC_PAGE_POP_1_NO                   0x00

#define TJC_PAGE_POP_2                      4
#define TJC_PAGE_POP_2_YES                  0x00

#define TJC_PAGE_POP_3                      5

#define TJC_PAGE_OPEN_LANGUAGE              6
#define TJC_PAGE_OPEN_LANGUAGE_NEXT         0x00
#define TJC_PAGE_OPEN_LANGUAGE_SKIP         0x07

#define TJC_PAGE_OPEN_VIDEO_1               80
#define TJC_PAGE_OPEN_VIDEO_1_NEXT          0x00

#define TJC_PAGE_OPEN_COMPENSAT             8

#define TJC_PAGE_OPEN_VIDEO2                81
#define TJC_PAGE_OPEN_VIDEO2_NEXT           0

#define TJC_PAGE_MOVE_TIP                   82
#define TJC_PAGE_MOVE_TIP_BACK              0x00
#define TJC_PAGE_MOVE_TIP_ENTER             0x01

#define TJC_PAGE_OPEN_VIDEO_3               83
#define TJC_PAGE_OPEN_VIDEO_3_NEXT          0x00

#define TJC_PAGE_OPEN_FILAMENTVIDEO_1       84
#define TJC_PAGE_OPEN_FILAMENTVIDEO_1_NEXT  0x00

#define TJC_PAGE_OPEN_FILAMENTVIDEO_2       85
#define TJC_PAGE_OPEN_FILAMENTVIDEO_2_NEXT  0x00
#define TJC_PAGE_OPEN_FILAMENTVIDEO_2_TARGET    0x09
#define TJC_PAGE_OPEN_FILAMENTVIDEO_2_DECREASE      0x02
#define TJC_PAGE_OPEN_FILAMENTVIDEO_2_INCREASE      0x03

#define TJC_PAGE_OPEN_FILAMENTVIDEO_3       86
#define TJC_PAGE_OPEN_FILAMENTVIDEO_3_NEXT  0x00
#define TJC_PAGE_OPEN_FILAMENTVIDEO_3_EXTRUDE   0x01

#define TJC_PAGE_OPEN_LEVEL_INIT_3              89

#define TJC_PAGE_OPEN_AUTO_LEVEL_2              90
#define TJC_PAGE_OPEN_AUTO_LEVEL_2_001              0x03
#define TJC_PAGE_OPEN_AUTO_LEVEL_2_005              0x04
#define TJC_PAGE_OPEN_AUTO_LEVEL_2_01               0x05
#define TJC_PAGE_OPEN_AUTO_LEVEL_2_1                0x06
#define TJC_PAGE_OPEN_AUTO_LEVEL_UP                 0x00
#define TJC_PAGE_OPEN_AUTO_LEVEL_DOWN               0x01
#define TJC_PAGE_OPEN_AUTO_LEVEL_ENTER              0x02

#define TJC_PAGE_OPEN_FILAMENT              10

#define TJC_PAGE_OPEN_LEVELMORE             11
#define TJC_PAGE_OPEN_LEVELMORE_ENTER               0x00

#define TJC_PAGE_OPEN_CHOOSE                12

#define TJC_PAGE_SAVING_2                           91

#define TJC_PAGE_MAIN                       13
#define TJC_PAGE_MAIN_BTN_HOME              0x03
#define TJC_PAGE_MAIN_BTN_FILE              0x04
#define TJC_PAGE_MAIN_BTN_TOOL              0x05
#define TJC_PAGE_MAIN_BTN_SERVICE           0x06
#define TJC_PAGE_MAIN_LIGHT                 0x00
#define TJC_PAGE_MAIN_VOICE                 0x01
#define TJC_PAGE_MAIN_LOCK                  0x02

#define TJC_PAGE_RESTARTING                 14

#define TJC_PAGE_CLEAR_X_POP                15
#define TJC_PAGE_CLEAR_X_POP_YES            0x00
#define TJC_PAGE_CLEAR_X_POP_NO             0x01

#define TJC_PAGE_CLEAR_X_VIDEO              16
#define TJC_PAGE_CLEAR_X_VIDEO_NEXT         0x00

#define TJC_PAGE_DISK_DETECT                17
#define TJC_PAGE_DISK_DETECT_HOME           0x03
#define TJC_PAGE_DISK_DETECT_TOOL           0x05
#define TJC_PAGE_DISK_DETECT_SERVICE        0x06

#define TJC_PAGE_FILE_LIST_1                18
#define TJC_PAGE_FILE_LIST_1_BTN_1          0x7
#define TJC_PAGE_FILE_LIST_1_BTN_2          0x8
#define TJC_PAGE_FILE_LIST_1_BTN_3          0x9
#define TJC_PAGE_FILE_LIST_1_BTN_4          0x0a
#define TJC_PAGE_FILE_LIST_1_BTN_5          0x0b
#define TJC_PAGE_FILE_LIST_1_BTN_6          0x0c
#define TJC_PAGE_FILE_LIST_1_BTN_PREVIOUS   0x00
#define TJC_PAGE_FILE_LIST_1_BTN_NEXT       0x01
#define TJC_PAGE_FILE_LIST_1_BTN_HOME       0x03
#define TJC_PAGE_FILE_LIST_1_BTN_FILE       0x04
#define TJC_PAGE_FILE_LIST_1_BTN_TOOL       0x05
#define TJC_PAGE_FILE_LIST_1_BTN_SERVICE    0x06

#define TJC_PAGE_FILE_LIST_2                19
#define TJC_PAGE_FILE_LIST_2_BTN_1          0x07
#define TJC_PAGE_FILE_LIST_2_BTN_2          0x08
#define TJC_PAGE_FILE_LIST_2_BTN_3          0x09
#define TJC_PAGE_FILE_LIST_2_BTN_4          0x0a
#define TJC_PAGE_FILE_LIST_2_BTN_5          0x0b
#define TJC_PAGE_FILE_LIST_2_BTN_6          0x0c
#define TJC_PAGE_FILE_LIST_2_BTN_BACK       0x00
#define TJC_PAGE_FILE_LIST_2_BTN_PREVIOUS   0x01
#define TJC_PAGE_FILE_LIST_2_BTN_NEXT       0x02
#define TJC_PAGE_FILE_LIST_2_BTN_HOME       0x03
#define TJC_PAGE_FILE_LIST_2_BTN_FILE       0x04
#define TJC_PAGE_FILE_LIST_2_BTN_TOOL       0x05
#define TJC_PAGE_FILE_LIST_2_BTN_SERVICE    0x06

#define TJC_PAGE_PREVIEW                    20
#define TJC_PAGE_PREVIEW_BTN_START          0x00
#define TJC_PAGE_PREVIEW_BTN_DELETE         0x01
#define TJC_PAGE_PREVIEW_BTN_BACK           0x02
//1.1.4 CLL 新增打印热床调平
#define TJC_PAGE_PREVIEW_BED_LEVELING       0x03

#define TJC_PAGE_DELETE_FILE                21
#define TJC_PAGE_DELETE_FILE_YES            0x01
#define TJC_PAGE_DELETE_FILE_NO             0x00

//1.1.6 CLL 新增fan3
#define TJC_PAGE_PRINTING                   22
#define TJC_PAGE_PRINTING_BTN_LIGHT         0x00
#define TJC_PAGE_PRINTING_BTN_EXTRUDER      0x01
#define TJC_PAGE_PRINTING_BTN_HEATER_BED    0x02
#define TJC_PAGE_PRINTING_BTN_FAN_0         0x03
#define TJC_PAGE_PRINTING_BTN_FAN_3         0x04
#define TJC_PAGE_PRINTING_BTN_SPEED         0x05
#define TJC_PAGE_PRINTING_BTN_FLOW          0x06
#define TJC_PAGE_PRINTING_BTN_Z_OFFSET      0x07
#define TJC_PAGE_PRINTING_BTN_PAUSE_RESUME  0x0a
#define TJC_PAGE_PRINTING_BTN_STOP          0x0b

#define TJC_PAGE_KEYDB                          23
#define TJC_PAGE_KEYDB_BACK                     0xfb
#define TJC_PAGE_KEYDB_PAUSE_RESUME             0x0a
#define TJC_PAGE_KEYDB_STOP                     0x0b

#define TJC_PAGE_PRINT_BABYSTEP                 24
#define TJC_PAGE_PRINT_BABYSTEP_001             0x00
#define TJC_PAGE_PRINT_BABYSTEP_005             0x01
#define TJC_PAGE_PRINT_BABYSTEP_01              0x02
#define TJC_PAGE_PRINT_BABYSTEP_1               0x03
#define TJC_PAGE_PRINT_BABYSTEP_UP              0x04
#define TJC_PAGE_PRINT_BABYSTEP_DOWN            0x05
#define TJC_PAGE_PRINT_BABYSTEP_PAUSE_RESUME    0x0a
#define TJC_PAGE_PRINT_BABYSTEP_STOP            0x0b
#define TJC_PAGE_PRINT_BABYSTEP_BACK            0x06

#define TJC_PAGE_PRINT_FILAMENT                 25
#define TJC_PAGE_PRINT_FILAMENT_PAUSE_RESUME    0x0a
#define TJC_PAGE_PRINT_FILAMENT_STOP            0x0b
#define TJC_PAGE_PRINT_FILAMENT_TARGET_UP       0x04
#define TJC_PAGE_PRINT_FILAMENT_TARGET_DOWN     0x03
#define TJC_PAGE_PRINT_FILAMENT_RETRACT         0x05
#define TJC_PAGE_PRINT_FILAMENT_EXTRUDE         0x06
#define TJC_PAGE_PRINT_FILAMENT_10              0x02
#define TJC_PAGE_PRINT_FILAMENT_20              0x01
#define TJC_PAGE_PRINT_FILAMENT_50              0x00
#define TJC_PAGE_PRINT_FILAMENT_TARGET          0x08

#define TJC_PAGE_FILAMENT_POP_3                    26
#define TJC_PAGE_FILAMENT_POP_3_OK              0x00

#define TJC_PAGE_PRINT_FINISH                   27
#define TJC_PAGE_PRINT_FINISH_YES               0x00
#define TJC_PAGE_PRINT_FINISH_INIT              0x01

#define TJC_PAGE_PRINT_STOP                     28
#define TJC_PAGE_PRINT_STOP_NO                  0x00
#define TJC_PAGE_PRINT_STOP_YES                 0x01

#define TJC_PAGE_PRINT_STOPING              29
#define TJC_PAGE_PRINT_STOPING_INIT         0x00

#define TJC_PAGE_PRINT_NO_FIL             30
#define TJC_PAGE_PRINT_NO_FIL_YES               0x00

#define TJC_PAGE_MOVE                       31
#define TJC_PAGE_MOVE_FILAMENT              0x01
#define TJC_PAGE_MOVE_NEXT                  0x02
// #define TJC_PAGE_MOVE_LEVEL_MODE            0x0e
// #define TJC_PAGE_MOVE_NETWORK               0x0f
#define TJC_PAGE_MOVE_01                    0x07
#define TJC_PAGE_MOVE_1                     0x08
#define TJC_PAGE_MOVE_10                    0x09
#define TJC_PAGE_MOVE_X_UP                  0x0b
#define TJC_PAGE_MOVE_X_DOWN                0x0a
#define TJC_PAGE_MOVE_Y_DOWN                0x0d
#define TJC_PAGE_MOVE_Y_UP                  0x0c
#define TJC_PAGE_MOVE_HOME                  0x10
#define TJC_PAGE_MOVE_Z_UP                  0x0e
#define TJC_PAGE_MVOE_Z_DOWN                0x0f
#define TJC_PAGE_MOVE_BTN_HOME              0x03
#define TJC_PAGE_MOVE_BTN_FILE              0x04
#define TJC_PAGE_MOVE_BTN_SERVICE           0x06
#define TJC_PAGE_MOVE_BTN_STOP              0x11
#define TJC_PAGE_MOVE_M84                   0x11

#define TJC_PAGE_MOVE_POP                   32
#define TJC_PAGE_MOVE_POP_YES               0x01
#define TJC_PAGE_MOVE_POP_NO                0x00

#define TJC_PAGE_FILAMENT                               33
#define TJC_PAGE_FILAMENT_MOVE                          0x00
#define TJC_PAGE_FILAMENT_BTN_HOME                      0x03
#define TJC_PAGE_FILAMENT_BTN_FILE                      0x04
#define TJC_PAGE_FILAMENT_BTN_SERVICE                   0x06
#define TJC_PAGE_FILAMENT_UNLOAD                        0x0e
#define TJC_PAGE_FILAMENT_BTN_10                        0x7
#define TJC_PAGE_FILAMENT_BTN_20                        0x8
#define TJC_PAGE_FILAMENT_BTN_50                        0x9
#define TJC_PAGE_FILAMENT_BTN_EXTRUDE                   0x0f
#define TJC_PAGE_FILAMENT_BTN_EXTRUDER                  0x0d
#define TJC_PAGE_FILAMENT_BTN_HEATER_BED                0x10
#define TJC_PAGE_FILAMENT_BTN_FAN_1                     0x14
//1.1.6 CLL 新增fan3    
#define TJC_PAGE_FILAMENT_BTN_FAN_3                     0x15
#define TJC_PAGE_FILAMENT_BTN_FILAMENT_SENSOR           0x0a
#define TJC_PAGE_FILAMENT_EXTRUDER                      0x0b
#define TJC_PAGE_FILAMENT_HEATER_BED                    0x0c
#define TJC_PAGE_FILAMENT_NEXT                          0x02

#define TJC_PAGE_FILAMENT_KB        34
#define TJC_PAGE_FILAMENT_KB_BACK                       0x00

#define TJC_PAGE_FILAMENT_ERROR     35
#define TJC_PAGE_FILAMENT_ERROR_OK        0x00

#define TJC_PAGE_FILAMENT_POP       36
#define TJC_PAGE_FILAMENT_POP_YES   0x00

#define TJC_PAGE_FILAMENT_POP_2     37
#define TJC_PAGE_FILAMENT_POP_2_YES         0x00

#define TJC_PAGE_UNLOAD_FINISH     38 //2023.5.12 新增退料完成页面
#define TJC_PAGE_UNLOAD_FINISH_YES 0x00

#define TJC_PAGE_LEVEL_MODE_1                       39
#define TJC_PAGE_LEVEL_MODE_1_BTN_HOME              0x03
#define TJC_PAGE_LEVEL_MODE_1_BTN_FILE              0x04
#define TJC_PAGE_LEVEL_MODE_1_BTN_TOOL              0x05
#define TJC_PAGE_LEVEL_MODE_1_BTN_SERVICE           0x06
#define TJC_PAGE_LEVEL_MODE_1_PREVIOUS              0x02
#define TJC_PAGE_LEVEL_MODE_1_NETWORK               0x01
#define TJC_PAGE_LEVEL_MODE_1_AUTO_LEVEL            0x07
#define TJC_PAGE_LEVEL_MODE_1_MANUAL_LEVEL          0x08
#define TJC_PAGE_LEVEL_MODE_1_NEXT                  0x0a
#define TJC_PAGE_LEVEL_MODE_1_ZOFFSET               0x00
#define TJC_PAGE_LEVEL_MODE_1_SYNTONY               0x09

#define TJC_PAGE_LEVEL_MODE_2                       40
#define TJC_PAGE_LEVEL_MODE_2_BTN_HOME              0x03
#define TJC_PAGE_LEVEL_MODE_2_BTN_FILE              0x04
#define TJC_PAGE_LEVEL_MODE_2_BTN_SERVICE           0x06
#define TJC_PAGE_LEVEL_MODE_2_BTN_TOOL              0x05
#define TJC_PAGE_LEVEL_MODE_2_PREVIOUS              0x02              
#define TJC_PAGE_LEVEL_MODE_2_NETWORK               0x01
#define TJC_PAGE_LEVEL_MODE_2_PRINT_LEVEL           0x07
#define TJC_PAGE_LEVEL_MODE_2_SYNTONY               0x08
#define TJC_PAGE_LEVEL_MODE_2_PID                   0x09
#define TJC_PAGE_LEVEL_MODE_2_FIRST                 0x0a

#define TJC_PAGE_LEVEL_INIT_1                       41
#define TJC_PAGE_LEVEL_INIT_1_BTN_HOME              0x03
#define TJC_PAGE_LEVEL_INIT_1_BTN_FILE              0x04
#define TJC_PAGE_LEVEL_INIT_1_BTN_SERVICE           0x06
#define TJC_PAGE_LEVEL_INIT_1_BTN_BACK              0x02
#define TJC_PAGE_LEVEL_INIT_1_ENTER                 0x00

#define TJC_PAGE_LEVEL_INIT_2       42

#define TJC_PAGE_LEVEL_INIT_SHOUDONG    93
#define TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_HOME       0x03
#define TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_FILE       0x04
#define TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_SERVICE    0x06
#define TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_BACK       0x02
#define TJC_PAGE_LEVEL_INIT_SHOUDONG_BTN_ENTER      0x00

#define TJC_PAGE_AUTO_LEVEL         43
#define TJC_PAGE_AUTO_LEVEL_001                     0x03
#define TJC_PAGE_AUTO_LEVEL_005                     0x04
#define TJC_PAGE_AUTO_LEVEL_01                      0x05
#define TJC_PAGE_AUTO_LEVEL_1                       0x06
#define TJC_PAGE_AUTO_LEVEL_ENTER                   0x02
#define TJC_PAGE_AUTO_LEVEL_UP                0x00
#define TJC_PAGE_AUTO_LEVEL_DOWN              0x01

#define TJC_PAGE_AUTO_LEVELING      44

#define TJC_PAGE_AUTO_FINISH        88
// #define TJC_PAGE_AUTO_FINISH        45
#define TJC_PAGE_AUTO_FINISH_YES                    0x00

#define TJC_PAGE_MANUAL_LEVEL       46
#define TJC_PAGE_MANUAL_LEVEL_001                   0x03
#define TJC_PAGE_MANUAL_LEVEL_005                   0x04
#define TJC_PAGE_MANUAL_LEVEL_01                    0x05
#define TJC_PAGE_MANUAL_LEVEL_1                     0x06
#define TJC_PAGE_MANUAL_LEVEL_ENTER                       0x02
#define TJC_PAGE_MANUAL_LEVEL_UP                          0x00
#define TJC_PAGE_MANUAL_LEVEL_DOWN                        0x01

#define TJC_PAGE_MANUAL_MOVE_2      47

#define TJC_PAGE_MANUAL_FINISH                      92

// #define TJC_PAGE_MANUAL_FINISH             48
#define TJC_PAGE_MANUAL_FINISH_YES                  0x00

#define TJC_PAGE_SAVING             49

#define TJC_PAGE_LEVEL_PR_POP       50

#define TJC_PAGE_SET_TEMP_LEVEL     51
#define TJC_PAGE_SET_TEMP_LEVEL_HOME        0x03
#define TJC_PAGE_SET_TEMP_LEVEL_FILE        0x04
#define TJC_PAGE_SET_TEMP_LEVEL_TOOL        0x05
#define TJC_PAGE_SET_TEMP_LEVEL_SERVICE     0x06
#define TJC_PAGE_SET_TEMP_LEVEL_BACK        0x02
#define TJC_PAGE_SET_TEMP_LEVEL_EXTRUDER    0x0d
#define TJC_PAGE_SET_TEMP_LEVEL_HOT_BED     0x10
#define TJC_PAGE_SET_TEMP_LEVEL_ENTER       0x00

#define TJC_PAGE_LEVELING_KB        52

#define TJC_PAGE_LEVEL_PRINT        53

#define TJC_PAGE_LEVEL_STOP         54

#define TJC_PAGE_CHOOSE_MODEL       55

#define TJC_PAGE_SYNTONY_MOVE       56

#define TJC_PAGE_SYNTONY_FINISH     57

#define TJC_PAGE_WAIT_POP           58
#define TJC_PAGE_WAIT_POP_OK                0x00

#define TJC_PAGE_PAGE0              59

#define TJC_PAGE_PIDING             60

#define TJC_PAGE_PID_FINISH         61

#define TJC_PAGE_WIFI_LIST_1                62

#define TJC_PAGE_WIFI_LIST_2                63
#define TJC_PAGE_WIFI_LIST_2_BTN_HOME       0x03
#define TJC_PAGE_WIFI_LIST_2_BTN_FILE       0x04
#define TJC_PAGE_WIFI_LIST_2_BTN_SERVICE    0x06
#define TJC_PAGE_WIFI_LIST_2_BTN_TOOL       0x05
#define TJC_PAGE_WIFI_LIST_2_LEVEL          0x00
#define TJC_PAGE_WIFI_LIST_2_PREVIOUS       0x07
#define TJC_PAGE_WIFI_LIST_2_NEXT           0x08
#define TJC_PAGE_WIFI_LIST_2_SSID_1         0x0b
#define TJC_PAGE_WIFI_LIST_2_SSID_2         0x0c
#define TJC_PAGE_WIFI_LIST_2_SSID_3         0x0d
#define TJC_PAGE_WIFI_LIST_2_REFRESH        0x09
#define TJC_PAGE_WIFI_LIST_2_BACK           0x02

#define TJC_PAGE_KEYDBA             64

#define TJC_PAGE_WIFI_CONNECT       65

#define TJC_PAGE_WIFI_SUCCESS       66
#define TJC_PAGE_WIFI_SUCCESS_YES           0x00

#define TJC_PAGE_WIFI_SAVING        67

#define TJC_PAGE_WIFI_FAILED                68
#define TJC_PAGE_WIFI_FAILED_YES            0x00

#define TJC_PAGE_INTERNET                   69
#define TJC_PAGE_INTERNET_BTN_HOME          0x03
#define TJC_PAGE_INTERNET_BTN_FILE          0x04
#define TJC_PAGE_INTERNET_AUTO_LEVEL        0x05
#define TJC_PAGE_INTERNET_BTN_SERVICE       0x06
#define TJC_PAGE_INTERNET_BTN_PREVIOUS      0x02

#define TJC_PAGE_LANGUAGE           70
#define TJC_PAGE_LANGUAGE_BTN_HOME          0x03
#define TJC_PAGE_LANGUAGE_BTN_FILE          0x04
#define TJC_PAGE_LANGUAGE_BTN_TOOL          0x05
#define TJC_PAGE_LANGUAGE_SERVICE           0x01
#define TJC_PAGE_LANGUAGE_NEXT              0x02

#define TJC_PAGE_SERVICE            71
#define TJC_PAGE_SERVICE_BTN_HOME           0x03
#define TJC_PAGE_SERVICE_BTN_FILE           0x04
#define TJC_PAGE_SERVICE_BTN_TOOL           0x05
#define TJC_PAGE_SERVICE_LANGUAGE           0x00
#define TJC_PAGE_SERVICE_NEXT               0x02

#define TJC_PAGE_SYS_ERROR          72

#define TJC_PAGE_SYS_OK             73
#define TJC_PAGE_SYS_OK_HOME                0x03
#define TJC_PAGE_SYS_OK_FILE                0x04
#define TJC_PAGE_SYS_OK_TOOL                0x05
#define TJC_PAGE_SYS_OK_ABOUT               0x01
#define TJC_PAGE_SYS_OK_PREVIOUS            0x02

#define TJC_PAGE_RESET              74
#define TJC_PAGE_RESET_BTN_HOME             0x03
#define TJC_PAGE_RESET_BTN_FILE             0x04
#define TJC_PAGE_RESET_BTN_TOOL             0x05
#define TJC_PAGE_RESET_ABOUT                0x01
#define TJC_PAGE_RESET_PREVIOUS             0x02
#define TJC_PAGE_RESET_RESTART_KLIPPER      0x07
#define TJC_PAGE_RESET_FIRMWARE_RESTART     0x08

#define TJC_PAGE_ABOUT              75
#define TJC_PAGE_ABOUT_BTN_HOME             0x03
#define TJC_PAGE_ABOUT_BTN_FILE             0x04
#define TJC_PAGE_ABOUT_BTN_TOOL             0x05
#define TJC_PAGE_ABOUT_RESET                0x00
#define TJC_PAGE_ABOUT_OOBE                 0x01
#define TJC_PAGE_ABOUT_BACK                 0x02
#define TJC_PAGE_ABOUT_S_BTN                0x08

#define TJC_PAGE_ABOUT_UPDATE       76
#define TJC_PAGE_ABOUT_UPDATE_BTN_HOME      0x03
#define TJC_PAGE_ABOUT_UPDATE_BTN_FILE      0x04
#define TJC_PAGE_ABOUT_UPDATE_BTN_TOOL      0x05
#define TJC_PAGE_ABOUT_UPDATE_RESET         0x00
#define TJC_PAGE_ABOUT_UPDATE_OOBE          0x01
#define TJC_PAGE_ABOUT_UPDATE_UPDATE        0x07
#define TJC_PAGE_ABOUT_UPDATE_BACK          0x02
#define TJC_PAGE_ABOUT_UPDATE_S_BTN         0x08

#define TJC_PAGE_UPDATE_SCREEN      77

#define TJC_PAGE_UPDATE_FINISH      78

#define TJC_PAGE_UPDATED                    79
#define TJC_PAGE_UPDATED_ENTER              0x00

#define TJC_PAGE_WIFI_KEYBOARD              87
#define TJC_PAGE_WIFI_KEYBOARD_BACK         0x02
#define TJC_PAGE_WIFI_KEYBOARD_ENTER        0x03

#define TJC_PAGE_ZOFFSET                    94
#define TJC_PAGE_ZOFFSET_YES                0x00

#define TJC_PAGE_FILAMENT_POP_4             95
#define TJC_PAGE_FILAMENT_POP_4_YES         0x00

#define TJC_PAGE_AUTO_INIT_1                96
#define TJC_PAGE_AUTO_INIT_1_DOWN           0x00
#define TJC_PAGE_AUTO_INIT_1_UP             0x01
#define TJC_PAGE_AUTO_INIT_1_HEATER_BED     0x02
#define TJC_PAGE_AUTO_INIT_1_BACK           0x03
#define TJC_PAGE_AUTO_INIT_1_ENTER          0x04

#define TJC_PAGE_AUTO_INIT_2                97
#define TJC_PAGE_AUTO_INIT_2_BACK           0x00
#define TJC_PAGE_AUTO_INIT_2_ENTER          0x01

#define TJC_PAGE_DETECT_ERROR                98
#define TJC_PAGE_DETECT_ERROR_YES            0x00

#define TJC_PAGE_GCODE_ERROR                99
#define TJC_PAGE_GCODE_ERROR_YES            0x00

#define TJC_PAGE_OPEN_VIDEO_4               100  //2023.5.8 CLL 新增开机引导界面
#define TJC_PAGE_OPEN_VIDEO_4_NEXT          0x00

#define TJC_PAGE_OPEN_HEATERBED             101  //2023.5.10 CLL 新增开机引导界面
#define TJC_PAGE_OPEN_HEATERBED_DOWN        0x00
#define TJC_PAGE_OPEN_HEATERBED_UP          0x01
#define TJC_PAGE_OPEN_HEATERBED_HEATER_BED  0x03
#define TJC_PAGE_OPEN_HEATERBED_ENTER       0x02

#define TJC_PAGE_OPEN_LEVELING              102 //2023.5.10 CLL 开机引导自动调平

#define TJC_PAGE_OPEN_LEVEL_FINISH          103
#define TJC_PAGE_OPEN_LEVEL_FINISH_ENTER    0x00

#define TJC_PAGE_OPEN_POP_1                 104 //2023.5.12 CLL 新增开机引导跳过确认界面
#define TJC_PAGE_OPEN_POP_1_NO              0x00
#define TJC_PAGE_OPEN_POP_1_YES             0x01

//1.1.5 CLL 新增"退料中"界面
#define TJC_PAGE_UNLOADING                  105

//1.1.6 CLL 打印前判断耗材种类并弹窗
#define TJC_PAGE_PREVIEW_POP_1               106
#define TJC_PAGE_PREVIEW_POP_1_YES           0x00

#define TJC_PAGE_PREVIEW_POP_2               107
#define TJC_PAGE_PREVIEW_POP_2_YES           0x00


void parse_cmd_msg_from_tjc_screen(char *cmd);
void page_to(int page_id);
void tjc_event_clicked_handler(int page_id, int widget_id, int type_id);
void tjc_event_setted_handler(int page_id, int widget_id, unsigned char first, unsigned char second);
void tjc_event_keyboard(char *cmd);

#endif
