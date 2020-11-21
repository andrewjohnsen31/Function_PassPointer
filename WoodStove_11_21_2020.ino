// first running of upstairs reguation 10/28/2020

#include "Resources.h"


//========= Debug Items =================
#define Touch_Line_Color HX8357_BLACK

boolean Switch_Pressed = false;

int Pixel_Y = 0;
int Pixel_X = 0;
#define BUFFPIXEL 20

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

Servo Damper_Servo;



Button_Class Upstairs;
Button_Class Downstairs;
Button_Class Outside;
Button_Class UpstairsSet;
Button_Class DownstairsSet;
Button_Class IGL;
Button_Class Weather;
Button_Class Fan;
Button_Class Debug;
Button_Class Damper;
Button_Class Timer;
Button_Class LoadWood;
Button_Class Stack;
Button_Class UpArrow;
Button_Class DownArrow;
Button_Class Weather_Arrow;
File SD_Refresh_File;




void setup() {
  delay (10000);
  Serial.begin(9600);
  Serial.println ("Starting Setup");


  Damper_Servo.attach(Damper_Pin , DampPositionMin, DampPositionMax);
  Damp_Position = DampPositionMax;              //start out fully open
  Damper_Servo.write(Damp_Position);

  zero_cross = false ;
  Phase_Cut_Setup();


  // If the EEPROM is empty then isValid() is false
  if (!EEPROM.isValid()) {

    Serial.println("EEPROM is empty, filling");
    EEPROM.write(EEaddr, 83);       // Set Room Temp init to 70 degF
    EEPROM.write((EE_StackMax_addr), 39); // Stack Max Temp & Fan full speed (scaled by 10)
    EEPROM.write((EE_StackMin_addr), 18); //  Fan Starts to come on
    EEPROM.write((EE_Load_Wood_addr), 30); // Load wood time
    EEPROM.write((EE_DesiredRooomTemp_addr ), 71); // Set UpStairs Set point via ecobee input
    EEPROM.commit();
    Serial.println("Done!");
  }

  pinMode (SD_CS, OUTPUT);
  delay (500);
  digitalWrite(SD_CS, HIGH);
  delay (100);
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
  }



  Downstairs.x = 83;
  Downstairs.y = 113;
  Downstairs.Width = 129;
  Downstairs.Height = 98;
  Downstairs.Round = 10;

  Upstairs.x = 83;
  Upstairs.y = 9;
  Upstairs.Width = 129;
  Upstairs.Height = 98;
  Upstairs.Round = 10;

  Outside.x = 83;
  Outside.y = 217;
  Outside.Width = 129;
  Outside.Height = 98;
  Outside.Round = 10;

  IGL.x = 0;
  IGL.y = 275;
  IGL.Width = 80;
  IGL.Height = 44;
  IGL.Round = 10;

  Weather.x = 212;
  Weather.y = 230;
  Weather.Width = 79;
  Weather.Height = 75;
  Weather.Round = 10;

  UpstairsSet.x = 212;
  UpstairsSet.y = 60;
  UpstairsSet.Width = 79;
  UpstairsSet.Height = 48;
  UpstairsSet.Round = 10;

  DownstairsSet.x = 212;
  DownstairsSet.y = 117;
  DownstairsSet.Width = 79;
  DownstairsSet.Height = 48;
  DownstairsSet.Round = 10;

  Debug.x = 420;
  Debug.y = 200;
  Debug.Width = 58;
  Debug.Height = 119;
  Debug.Round = 10;

  Fan.x = 390;
  Fan.y = 200;
  Fan.Width = 45;
  Fan.Height = 119;
  Fan.Round = 10;

  Damper.x = 315;
  Damper.y = 200;
  Damper.Width = 45;
  Damper.Height = 119;
  Damper.Round = 10;

  Timer.x = 320;
  Timer.y = 50;
  Timer.Width = 60;
  Timer.Height = 40;
  Timer.Round = 10;

  LoadWood.x = 324;
  LoadWood.y = 98;
  LoadWood.Width = 130;
  LoadWood.Height = 45;
  LoadWood.Round = 5;


  Stack.x = 400;
  Stack.y = 50;
  Stack.Width = 80;
  Stack.Height = 40;
  Stack.Round = 10;

  UpArrow.x = 350;
  UpArrow.y = 20;
  UpArrow.Width = 80;
  UpArrow.Height = 90;
  UpArrow.Round = 10;

  DownArrow.x = 350;
  DownArrow.y = 220;
  DownArrow.Width = 89;
  DownArrow.Height = 90;
  DownArrow.Round = 10;

  Weather_Arrow.x = 0;
  Weather_Arrow.y = 0;
  Weather_Arrow.Width = 70;
  Weather_Arrow.Height = 70;
  Weather_Arrow.Round = 10;

  // Initalize Arrays Room Temp & Stack Temp
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    ThermostatValue[thisReading] = 251;
    StackValue[thisReading] = 449;
  }
  tft.begin();

  tft.setRotation(1);
  //=================
  tft.fillScreen(HX8357_DARKCYAN);

  UpDate_BMP();
  // refresh all numbers
  Fan_SpeedDisp = 5;
  Damp_PositionDisp = 5;
  ThermostatdegFDisp = 5 ;
  StackdegFDisp = 5;
  DesiredRoomTempDisp = 5;
  Weather_TemperatureDisp = 5;
  LivingRoomTempDisp = 5;
  Load_wood_timerDisp = 5;
  Set_DownStairsDisp = 5;
  LoadWood_Disp = false;
  LivingRooomTemp_BackRound = HX8357_BLACK ;
  ThermostatdegF_BackRound =  HX8357_MAGENTA;
  Load_wood_timer_BackRound = HX8357_BLACK;
  Stack_Temp_BackRound = HX8357_BLACK ;

  UpDate_Display_Normal();

  Connect_to_Network();

  EcoBee_millis = millis() - min16_7_millis ;
  regulation_millis = millis();

}

void loop() {






  current_millis = millis(); // update General



  if ((millis() - EcoBee_millis) >= min16_7_millis ) { // 16.66 min
    UpDate_Temperatures();
    EcoBee_millis = millis();
  }


  //================================================================
  // read from the Room & Stack sensor:
  ThermostatValue[IndexCount] = analogRead(ThermostatSensor);
  StackValue[IndexCount] = analogRead(StackSensor);

  // advance Room Sensor & Stack to the next position in the array:
  IndexCount = IndexCount + 1;
  if (IndexCount >= numReadings) {
    IndexCount = 0;
  }

  // Update Thermostat & Stack total
  ThermostatTotal = 0;
  StackTotal = 0;
  for (t = 0; t < numReadings; t++)
  {
    ThermostatTotal = ThermostatTotal + ThermostatValue[t];
    StackTotal = StackTotal + StackValue[t];
  }
  // Update Thermostat Average
  ThermostatAvg = ThermostatTotal / numReadings;
  StackAvg = StackTotal / numReadings;
  // Convert to degF
  StackdegF = map(StackAvg, 465, 838, 122, 554); // used numbers from Wood Stove Controler.numbers
  ThermostatdegF = map(ThermostatAvg, 186, 264, 45, 90) - 20; // fudged by 5 deg, not sure why

  //Serial.print ("thermostat   : "); Serial.println (ThermostatdegF);

  if (StackdegF <= 0 || StackdegF >= 500) { // Thermocouple get noisy above 500 degF, not needed info
    StackdegF = 999;
  }

  // linear scale fan top stack temp (Speed in not linear to phase cut on time)
  Fan_Speed = map(StackdegF, (EEPROM.read(EE_StackMin_addr) * 10), (EEPROM.read(EE_StackMax_addr) * 10), 128, 0 );
  if (Fan_Speed <= Fan_Speed_min) {
    Fan_Speed = Fan_Speed_min; // Full on
  }
  if (Fan_Speed >= Fan_Speed_max) {
    Fan_Speed = Fan_Speed_max; // no need for lower fan speed control
  }

  Update_TouchScreen(); // check for user input

  if (StackdegF >= ((EEPROM.read(EE_StackMax_addr)) * 10)) {
    Operating_Mode = 3; // stack hot start reducing damper
    Serial.println ("Stack over Max");
  }

  switch (Operating_Mode) // use top right lcd for modes
  {
    case 1: // New Wood Mode, regulate to stack temp based on time (to be worked out)

      LivingRooomTemp_BackRound = HX8357_BLACK ;
      LivingRoomTempDisp = 5;
      ThermostatdegF_BackRound =  HX8357_BLACK;
      ThermostatdegFDisp = 5 ;
      Load_wood_timer_BackRound = HX8357_MAGENTA;
      Load_wood_timerDisp = 5;
      LoadWood_Disp = false;
      Stack_Temp_BackRound = HX8357_BLACK ;
      StackdegFDisp = 5;




      if (StackdegF >= ( EEPROM.read(EE_StackMin_addr) * 10)) {   //Fire too hot
        Operating_Mode = 3; // Regulating to Stack Temp
        End_load_wood_millis = current_millis ;         // End Timer count down

      }
      else if (current_millis >= End_load_wood_millis)
      {
        regulation_millis = millis() - min1_67_millis;
        Operating_Mode = 2; // Exit New Wood Mode => Room regulation Mode
      }
      //    Load Wood Timer working
      //       Serial.print("millis : "); Serial.println (millis());
      //       Serial.print("End_load_wood_millis: "); Serial.println (End_load_wood_millis-millis());
      break;

    case 2:  // Regulating to Room Temp
      {

        LivingRooomTemp_BackRound = HX8357_BLACK ;
        LivingRoomTempDisp = 5;
        ThermostatdegF_BackRound = HX8357_MAGENTA;
        ThermostatdegFDisp = 5 ;
        Load_wood_timer_BackRound = HX8357_BLACK;
        Load_wood_timerDisp = 5;
        LoadWood_Disp = false;
        Stack_Temp_BackRound = HX8357_BLACK ;
        StackdegFDisp = 5;
        if (Regulate_Upstairs ) { // this is used to adjust temperature above downstairs setting for upstairs heat
          LivingRooomTemp_BackRound = HX8357_MAGENTA ;
          LivingRoomTempDisp = 5;             // used to force update on display refresh
        }
        if ((Regulate_Upstairs) && (Weather_TimeStamp != Weather_TimeStamp_prev ) ) { // this is used to adjust temperature above downstairs setting for upstairs heat

          Weather_TimeStamp_prev = Weather_TimeStamp;

          error = (LivingRoomTemp / 10) - EEPROM.read(EE_DesiredRooomTemp_addr); //LivingRoom
          if (error > 1) {
            adjust_error_from_Upstairs = adjust_error_from_Upstairs/2; // too hot upstairs, Hit it hard to cool down
            Damp_Position = Damp_Position/2;   //choke the fire back
          }

          else if (error <= -1) {
            adjust_error_from_Upstairs = adjust_error_from_Upstairs + 1; //small  step
          }
    

          Serial.println (" ==== In upstairs regulation ===");
          Serial.print("Regulate_Upstairs  true/false         : "); Serial.println (Regulate_Upstairs);
          Serial.print("Error up stairs          : "); Serial.println (error);
          Serial.print("upStairs  temperature                  : "); Serial.print(LivingRoomTemp / 10); Serial.println("F  ");
          Serial.print ("adjust_error_from_Upstairs : "); Serial.println (adjust_error_from_Upstairs);
          Serial.print ("Damper                      :"); Serial.println (Damp_Position);
          Serial.println (" ==== In upstairs regulation ===");
          Serial.println ("");

        }

        if ((millis() - regulation_millis) >= min1_67_millis ) // 1.67 min per itereration (loop update), 30 min full sweep mimnutes adjust loop
        {


          //         Serial.print("Max Stack Temp limit : "); Serial.println(EEPROM.read(EE_StackMax_addr) * 10);
          //         Serial.print("Operating_Mode    : "); Serial.println (Operating_Mode);
          //         Serial.print("Reg Rm            : "); Serial.print(EEPROM.read(EEaddr)); Serial.println("F  ");
          //         Serial.println ("======================================");
          //        Serial.println ("");


          error = ThermostatdegF - (EEPROM.read(EEaddr) + adjust_error_from_Upstairs); //EEPROM.read(EEaddr);

          Serial.print ("Error down stairs: "); Serial.println (error);
          if (error >= 4) {
            Damp_Position = Damp_Position - 200; // big step
          }
          else if (error >= 1)  {
            Damp_Position = Damp_Position - 50; // small step
          }
          if (error <= -1) {
            Damp_Position = Damp_Position + 50; //small  step
          }
          else if (error <= -4) {
            Damp_Position = Damp_Position + 200; // big step
          }
          if (Damp_Position <= DampPositionMin) {
            Damp_Position = DampPositionMin;
          }
          if (Damp_Position >= DampPositionMax) {
            Damp_Position = DampPositionMax;
          }

          regulation_millis = millis(); //reset regulation timer

          //         Serial.print("millis : "); Serial.println (millis());
          //         Serial.print("regulation_millis: "); Serial.println (regulation_millis);
          Serial.print("Damp_Position : "); Serial.println (Damp_Position);

        }

        Damper_Servo.write(Damp_Position);  //update servo

        break;
      }
    case 3:  // Regulating to Stack Temp
      {
        LivingRooomTemp_BackRound = HX8357_BLACK ;
        LivingRoomTempDisp = 5;
        ThermostatdegF_BackRound = HX8357_BLACK;
        ThermostatdegFDisp = 5 ;
        Load_wood_timer_BackRound = HX8357_BLACK;
        Load_wood_timerDisp = 5;
        LoadWood_Disp = false;
        Stack_Temp_BackRound = HX8357_MAGENTA ;
        StackdegFDisp = 5;

        if ((millis() - regulation_millis) >= min1_67_millis || StackdegF >= ((EEPROM.read(EE_StackMax_addr)) * 10) ) // 1.67 min per itereration (loop update), 30 min full sweep mimnutes adjust loop
        {
          // Set Damper position base on Stack temperature  (range: Max -10 to max) {ex 400-500 degF}
          if (StackdegF >= 500) StackdegF = (499);
          Damp_Position = map (StackdegF, ((EEPROM.read(EE_StackMax_addr) - 10) * 10), (EEPROM.read(EE_StackMax_addr) * 10), DampPositionMax, DampPositionMin );

          if (Damp_Position <= DampPositionMin) {
            Damp_Position = DampPositionMin;
          }
          if (Damp_Position >= DampPositionMax) {
            Damp_Position = DampPositionMax;
          }
          Serial.print("StackdegF         : "); Serial.println (StackdegF);
          Serial.print("Operating_Mode    : "); Serial.println (Operating_Mode);
          Serial.print("regulating stack  : "); Serial.println (Damp_Position);
          Serial.print("Max Stack Temp    : "); Serial.println(EEPROM.read(EE_StackMax_addr) * 10);
          Serial.print("Min Stack Temp    : "); Serial.println((EEPROM.read(EE_StackMax_addr) - 10) * 10);

          Damper_Servo.write(Damp_Position);  //update servo

          if (StackdegF <= ( (EEPROM.read(EE_StackMax_addr) - 10) * 10)) { //Fire under control back to regulation
            Operating_Mode = 2; // Regulating to Room Temperature

          }



          regulation_millis = millis(); //reset regulation timer
        }
        break;
      }


  }
  Damper_Servo.write(Damp_Position);  //update servo
  UpDate_Display ();



  while ((millis() - current_millis) <= 200); // works good for key pad response

}
