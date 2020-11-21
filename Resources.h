
#include <ArduinoJson.h>
#include <SPI.h>
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <MemoryFree.h>
#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>
#include <SD.h>    // testing out switching from SD.h to SdFat.h says it is better
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <TouchScreen.h>
#include <Servo.h>
#include <FlashAsEEPROM.h>    for nano 33

using namespace std;            // Namespaces are used to organize code
int i = 0;                      // generic counter
int t = 0;                      // generic counter

class Button_Class {
  public:
    int16_t x, y, Width, Height, Round;

};

//========== Network Secrets =================
#define SECRET_SSID "160BayRd"
#define SECRET_PASS "31Winthrop"
#define SECRET_Client_ID "dRc7CqSwIxKI7pNp3ZMDqJGPY7aT9akm"
#define SECRET_Seed_Refresh_Token "STc4tohqdo2M8hWmqmxtNoP6zIpEP2Wx"

//========== Hardware Pins ===================
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset
#define SD_CS 4   // SD card Chip Select

// These are the four touchscreen analog pins
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 8   // can be a digital pin

//============ Color palate ===================
#define HX8357_BLACK       0x0000  ///<   0,   0,   0
#define HX8357_NAVY        0x000F  ///<   0,   0, 123
#define HX8357_DARKGREEN   0x03E0  ///<   0, 125,   0
#define HX8357_DARKCYAN    0x03EF  ///<   0, 125, 123
#define HX8357_MAROON      0x7800  ///< 123,   0,   0
#define HX8357_PURPLE      0x780F  ///< 123,   0, 123
#define HX8357_OLIVE       0x7BE0  ///< 123, 125,   0
#define HX8357_LIGHTGREY   0xC618  ///< 198, 195, 198
#define HX8357_DARKGREY    0x7BEF  ///< 123, 125, 123
#define HX8357_BLUE        0x001F  ///<   0,   0, 255
#define HX8357_GREEN       0x07E0  ///<   0, 255,   0
#define HX8357_CYAN        0x07FF  ///<   0, 255, 255
#define HX8357_RED         0xF800  ///< 255,   0,   0
#define HX8357_MAGENTA     0xF81F  ///< 255,   0, 255
#define HX8357_YELLOW      0xFFE0  ///< 255, 255,   0
#define HX8357_WHITE       0xFFFF  ///< 255, 255, 255
#define HX8357_ORANGE      0xFD20  ///< 255, 165,   0
#define HX8357_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define HX8357_PINK        0xFC18  ///< 255, 130, 198



//========= Global Variables ============



String Weather_Location = "Sanborntonn New Hampshire";
String Weather_TimeStamp = "2020-07-02 21:03:22"; //""timestamp":
String Weather_TimeStamp_prev = "1020-07-02 21:03:22";
// ====  0, Outside; 1-4 next days, 5-8 next 6 hour forcasts =========

String Weather_DateTime_0 = "2000-07-17 12:00:00";
String Weather_DateTime_1 = "2000-07-17 12:00:00";
String Weather_DateTime_2 = "2000-07-17 12:00:00";
String Weather_DateTime_3 = "2000-07-17 12:00:00";
String Weather_DateTime_4 = "2000-07-17 12:00:00";
String Weather_DateTime_5 = "2000-07-17 12:00:00";
String Weather_DateTime_6 = "2000-07-17 12:00:00";
String Weather_DateTime_7 = "2000-07-17 12:00:00";
String Weather_DateTime_8 = "2000-07-17 12:00:00";
String Weather_Condition_0 = "Partly cloudy throughout the day.";
String Weather_Condition_1 = "Partly cloudy throughout the day.";
String Weather_Condition_2 = "Partly cloudy throughout the day.";
String Weather_Condition_3 = "Partly cloudy throughout the day.";
String Weather_Condition_4 = "Partly cloudy throughout the day.";
String Weather_Condition_5 = "Partly cloudy throughout the day.";
String Weather_Condition_6 = "Partly cloudy throughout the day.";
String Weather_Condition_7 = "Partly cloudy throughout the day.";
String Weather_Condition_8 = "Partly cloudy throughout the day.";
int Weather_Temperature[9] = {0};
int Weather_TemperatureDisp = 2;
int Weather_TempHigh[9] = {0};
int Weather_TempLow[9] = {0};
int Weather_Symbol[9] = {0};
String BMP_File_Name = "";


/*
            "dateTime": "2020-06-29 12:00:00",
            "condition": "Possible Drizzle",
            "temperature": 663,
            "pressure": 1009,
            "relativeHumidity": 97,
            "dewpoint": 656,
            "visibility": 16000,
            "windSpeed": 7,
            "windGust": -5002,
            "windDirection": "NE",
            "windBearing": 36,
            "pop": 40,
            "tempHigh": -5002,
            "tempLow": -5002,
            "sky": 5
            no_symbol = -2
  sunny = 0
  few_clouds = 1
  partly_cloudy = 2
  mostly_cloudy = 3
  overcast = 4
  drizzle = 5
  rain = 6
  freezing_rain = 7
  showers = 8
  hail = 9
  snow = 10
  flurries = 11
  freezing_snow = 12
  blizzard = 13
  pellets = 14
  thunderstorm = 15
  windy = 16
  tornado = 17
  fog = 18
  haze = 19
  smoke = 20
  dust = 21
*/

int LivingRoomTemp = 1;
int LivingRoomTempDisp = 0;
int DesiredRoomTemp = 1;
int DesiredRoomTempDisp = 0;

int DownStairs_Position_Set_Disp = 0;
int UpStairs_Position_Set_Disp = 0;

// Phase cut Fan Speed Control
int Zero_Cross_Pin = 2;
int Triac_Driver_Pin = 5;  // debug pin for overflow led
const int Fan_Speed_min = 0;          // max brightness
const int Fan_Speed_max = 120;        // min brightness reduced from 128
int Fan_Speed = Fan_Speed_max;
int Fan_SpeedDisp = 50; // seed with number for initial screen
int Fan_Speed_i;
bool zero_cross = true;

//Servo Damper;
const int Damper_Pin = 3;
const int DampPositionMin = 1050;
const int DampPositionMax = 2000;
int Damp_Position = DampPositionMax;
int Damp_PositionDisp = 0;
int Damp_Position_Set_Disp = 0;
int Stack_Max_Set_Disp = 0;
int Stack_Min_Set_Disp = 0;

bool share_to_Debug = false;
bool Weather_Disp = false;
bool hr24_Forcast = false;


// Room Temperature Sensor
const int ThermostatSensor = A3;

const int numReadings = 10;

int IndexCount;
int ThermostatValue [numReadings];

int ThermostatIndex = 0;


int ThermostatAvg = 0;

int Set_DownStairsDisp = 0;
int ThermostatdegF ;

int ThermostatdegFDisp ;

unsigned int ThermostatTotal = 0;
// Stack Sensor: Operating Range 250-550 degF
const int StackSensor = A0;
int StackValue [numReadings];
int StackIndex = 0;
unsigned int StackTotal = 0;
int StackAvg = 0;

int StackdegF = 100 ;
int StackdegFDisp = 0;

// Real time clock Timing variables millis()
unsigned long End_load_wood_millis = 0;
unsigned long End_loop_millis = 0;
unsigned long current_millis = 0;
unsigned long regulation_millis = 0;
unsigned long EcoBee_millis = 0;
unsigned long Display_millis = 0;

const unsigned long sec10_millis = 10000;
const unsigned long min1_67_millis = 100000;
const unsigned long min16_7_millis = 1000000;




int Load_wood_timerDisp = 0;
boolean LoadWood_Disp = false;

// Loop Control
int error = 0;
int adjust_error_from_Upstairs = 0;

// ===== Operating Modes
int Operating_Mode = 2;
//  ---- Modes ----
//    #1 New Wood, Reg Stack based on Temperature
//    #2 Regulation of Room Temp
//    #3 Regulation of Stack Temp
//    #4 Regulation of UpStairs

// Menu Char array
int menusize = 10;
int menu_position = 0;


// EEprom
byte i_byte = 0;
int EEaddr = 10;    // Base of EE Address (Set_Room_Temp)
int EE_StackMax_addr = EEaddr + 1;
int EE_StackMin_addr = EEaddr + 2;
int EE_Load_Wood_addr = EEaddr + 3;
int EE_DesiredRooomTemp_addr = EEaddr + 4;
int new_eeprom_value = 0;
const int address_refresh_token = 50;// rename after comboining codes to EE_refresh_token_addr
//addr 32 '\0' null termination char
const int address_null_termination = 32;        // used to check if EEProm is initialized

// Second Menu items
int Refresh_Disp;

// Regulation identifiers
unsigned int Stack_Temp_BackRound = HX8357_MAGENTA ;
unsigned int LivingRooomTemp_BackRound = HX8357_MAGENTA ;
unsigned int ThermostatdegF_BackRound = HX8357_MAGENTA ;
unsigned int Load_wood_timer_BackRound = HX8357_MAGENTA ;

//=========== Debug Codes =================
bool Debug_temperature = false;
bool Regulate_Upstairs = false;
