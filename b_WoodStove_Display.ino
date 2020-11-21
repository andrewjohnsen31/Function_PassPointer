/***************************************************
  This is our library for the Adafruit HX8357D Breakout
  ----> http://www.adafruit.com/products/2050

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


// These are 'flexible' lines that can be changed


// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

void UpDate_BMP() {
  //  noInterrupts();
  // char* dam_conversion = "moon2.bmp";
  // Serial.println(dam_conversion);
  tft.setRotation(0);
  bmpDraw(BMP_File_Name_Table (Weather_Symbol[0]), 285, 220);
  tft.setRotation(1);
  bmpDraw("IGL50_75.bmp", 5, 275);
  //  interrupts();
}

void UpDate_Display () {

  switch (menu_position) {
    case 0:     // Normal Screen
      if (millis() - Display_millis >= sec10_millis ) {
        UpDate_Display_Normal();
        Display_millis = millis();
      }

      //     Serial.println ("menu position");
      break;
    case 1:     // Set Down Stairs Desired temperature
      Display_Set_DownStairs();
      break;
    case 2:
      Display_Stack_Max();
      break;
    case 3:
      Display_Stack_Min();
      break;
    case 4:
      Display_Wood_Load_Timer();
      break;
    case 8:     // Manual Damper Overide
      Display_Manual_Overide();
      //     Serial.println ("over ride");
      break;
    case 9:     // Debug TFT Text
      Display_Debug();

      break;
    case 10:     //
      Display_Weather();
      break;
    case 11:     // Set Up Stairs Desired temperature
      Display_Set_UpStairs();
      break;
  }


}


void Display_Weather () {
  if (Weather_Disp) {
    tft.fillScreen(HX8357_BLACK);
    tft.setFont(&FreeSansBold12pt7b);// tft.setFont() FreeSansBold12pt7b;
    tft.setTextColor(HX8357_WHITE);
    //========= Back Arrow facing down =======
    tft.fillTriangle (10, 10, 60, 10, 35, 60, HX8357_WHITE);
    tft.setTextSize(2);
    tft.setCursor(80, 50);
    tft.println("Weather");
    tft.drawLine(0, 68, 480, 68, HX8357_WHITE);
    tft.drawLine(0, 64, 480, 64, HX8357_WHITE);


    tft.setFont();
    tft.setTextSize(1);
    tft.setCursor(310, 20);
    tft.println("Weather TimeStamp: ");
    tft.setCursor(310, 45);
    tft.println(Weather_TimeStamp);
    //

    // Serial.println ("==============");
    //  for (int i=5; i<=8; i++){
    //      Serial.print(i); Serial.print (" : "); Serial.print (dateTime_Str[i]); Serial.print ("  "); Serial.print (weatherSymbol[i]); Serial.print ("  "); Serial.println( temperature[i]);
    //  }


    // =========== Forcast ============

    /*

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

    */
    String test_Str = "";


    if (!hr24_Forcast) {

      tft.setCursor(10, 75); tft.setTextSize(3);
      tft.println (Day_of_Week ( Weather_DateTime_1));
      tft.setCursor(10, 105); tft.setTextSize(2);
      if (Weather_Condition_1.length() >= 50)tft.setTextSize(1);
      tft.println (Weather_Condition_1);
      tft.setCursor(200, 75); tft.setTextSize(3);
      tft.println (Weather_TempHigh[1] / 10);
      tft.setCursor(300, 75); tft.setTextSize(3);
      tft.println (Weather_TempLow[1] / 10);
      tft.setRotation(0);
      bmpDraw(BMP_File_Name_Table (Weather_Symbol[1]), 195, 400);
      tft.setRotation(1);


      tft.setCursor(10, 135); tft.setTextSize(3);
      tft.println (Day_of_Week ( Weather_DateTime_2));
      tft.setCursor(10, 165); tft.setTextSize(2);
      if (Weather_Condition_2.length() >= 50)tft.setTextSize(1);
      tft.println (Weather_Condition_2);
      tft.setCursor(200, 135); tft.setTextSize(3);
      tft.println (Weather_TempHigh[2] / 10);
      tft.setCursor(300, 135); tft.setTextSize(3);
      tft.println (Weather_TempLow[2] / 10);

      tft.setRotation(0);
      bmpDraw(BMP_File_Name_Table (Weather_Symbol[2]), 135, 400);
      tft.setRotation(1);


      tft.setCursor(10, 195); tft.setTextSize(3);
      tft.println (Day_of_Week ( Weather_DateTime_3));
      tft.setCursor(10, 225); tft.setTextSize(2);
      if (Weather_Condition_3.length() >= 50)tft.setTextSize(1);
      tft.println (Weather_Condition_3);
      tft.setCursor(200, 195); tft.setTextSize(3);
      tft.println (Weather_TempHigh[3] / 10);
      tft.setCursor(300, 195); tft.setTextSize(3);
      tft.println (Weather_TempLow[3] / 10);
      tft.setRotation(0);
      bmpDraw(BMP_File_Name_Table (Weather_Symbol[3]), 75, 400);
      tft.setRotation(1);


      tft.setCursor(10, 255); tft.setTextSize(3);
      tft.println (Day_of_Week ( Weather_DateTime_4));
      tft.setCursor(10, 285); tft.setTextSize(2);
      if (Weather_Condition_4.length() >= 50)tft.setTextSize(1);
      tft.println (Weather_Condition_4);
      tft.setCursor(200, 255); tft.setTextSize(3);
      tft.println (Weather_TempHigh[4] / 10);
      tft.setCursor(300, 255); tft.setTextSize(3);
      tft.println (Weather_TempLow[4] / 10);

      tft.setRotation(0);
      bmpDraw(BMP_File_Name_Table (Weather_Symbol[4]), 15, 400);
      tft.setRotation(1);
    }
    else {

      tft.setCursor(10, 75); tft.setTextSize(3);
      test_Str = Weather_DateTime_5;
      test_Str = test_Str.substring(11, 13);
      Serial.println (test_Str);
      int x;
      x = test_Str.toInt();
      Serial.println(x);
      if (x >= 0   && x < 6) tft.println ("Overnight");
      else if (x >= 6  && x < 12) tft.println ("Morning");
      else if (x >= 12  && x < 18) tft.println ("Afternoon");
      else  tft.println ("Evening");
      tft.setCursor(10, 105); tft.setTextSize(2);
      tft.println (Weather_Condition_5);
      tft.setCursor(200, 75); tft.setTextSize(3);
      tft.println (Weather_Temperature[5] / 10);

      tft.setRotation(0);
      bmpDraw(BMP_File_Name_Table (Weather_Symbol[5]), 195, 400);
      tft.setRotation(1);

      //     bmpDraw("Partly.bmp", 300, 140);
      tft.setCursor(10, 135); tft.setTextSize(3);
      test_Str = Weather_DateTime_6;
      test_Str = test_Str.substring(11, 13);
      Serial.println(test_Str);
      x = test_Str.toInt();
      Serial.println(x);
      if (x >= 0   && x < 6) tft.println ("Overnight");
      else if (x >= 6  && x < 12) tft.println ("Morning");
      else if (x >= 12  && x < 18) tft.println ("Afternoon");
      else  tft.println ("Evening");
      tft.setCursor(10, 165); tft.setTextSize(2);
      tft.println (Weather_Condition_6);
      tft.setCursor(200, 135); tft.setTextSize(3);
      tft.println (Weather_Temperature[6] / 10);

      tft.setRotation(0);
      bmpDraw(BMP_File_Name_Table (Weather_Symbol[6]), 135, 400);
      tft.setRotation(1);

      tft.setCursor(10, 195); tft.setTextSize(3);
      test_Str = Weather_DateTime_7;
      test_Str = test_Str.substring(11, 13);
      Serial.println(test_Str);
      x = test_Str.toInt();
      Serial.println(x);
      if (x >= 0   && x < 6) tft.println ("Overnight");
      else if (x >= 6  && x < 12) tft.println ("Morning");
      else if (x >= 12  && x < 18) tft.println ("Afternoon");
      else  tft.println ("Evening");
      tft.setCursor(10, 225); tft.setTextSize(2);
      tft.println (Weather_Condition_7);
      tft.setCursor(200, 195); tft.setTextSize(3);
      tft.println (Weather_Temperature[7] / 10);

      tft.setRotation(0);
      bmpDraw(BMP_File_Name_Table (Weather_Symbol[7]), 75, 400);
      tft.setRotation(1);

      //      bmpDraw("Partly.bmp", 300, 260);
      tft.setCursor(10, 255); tft.setTextSize(3);
      test_Str = Weather_DateTime_8;
      test_Str = test_Str.substring(11, 13);
      Serial.println(test_Str);
      x = test_Str.toInt();
      Serial.println(x);
      if (x >= 0   && x < 6) tft.println ("Overnight");
      else if (x >= 6  && x < 12) tft.println ("Morning");
      else if (x >= 12  && x < 18) tft.println ("Afternoon");
      else  tft.println ("Evening");
      tft.setCursor(10, 285); tft.setTextSize(2);
      tft.println (Weather_Condition_8);
      tft.setCursor(200, 255); tft.setTextSize(3);
      tft.println (Weather_Temperature[8] / 10);

      tft.setRotation(0);
      bmpDraw(BMP_File_Name_Table (Weather_Symbol[8]), 15, 400);
      tft.setRotation(1);
    }


    //    bmpDraw("IGL50_75.bmp", 5, 275);
    tft.drawRoundRect(Weather_Arrow.x, Weather_Arrow.y, Weather_Arrow.Width, Weather_Arrow.Height , Weather_Arrow.Round, HX8357_GREEN); //Touch_Line_Color);
    Weather_Disp = false;
  }
}

void Display_Debug () {


  //  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_GREEN);    tft.setTextSize(3);

  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");


  bmpDraw("IGL50_75.bmp", 5, 275);
}


void Display_Wood_Load_Timer () {

  if (Stack_Min_Set_Disp != EEPROM.read(EE_Load_Wood_addr)) {
    tft.setFont(&FreeSansBold12pt7b);// tft.setFont() FreeSansBold12pt7b;
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    tft.fillRoundRect(30, 30, 125, 94, 10, HX8357_BLACK);
    tft.setCursor(40, 88);
    tft.println(EEPROM.read(EE_Load_Wood_addr));
    Stack_Min_Set_Disp = EEPROM.read(EE_Load_Wood_addr);
  }


  //======== Arrows ============

  tft.fillTriangle ((UpArrow.x + UpArrow.Width / 2), UpArrow.y, UpArrow.x, (UpArrow.y + UpArrow.Height), (UpArrow.x + UpArrow.Width), (UpArrow.y + UpArrow.Height), HX8357_WHITE);
  tft.fillRect((UpArrow.x + (UpArrow.Width / 2) - 13), (UpArrow.y + UpArrow.Height), 26, 40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, UpArrow.y, UpArrow.Width, UpArrow.Height , UpArrow.Round, Touch_Line_Color);
  tft.fillTriangle ((DownArrow.x + DownArrow.Width / 2), (DownArrow.y + DownArrow.Height), DownArrow.x, (DownArrow.y), (DownArrow.x + DownArrow.Width), (DownArrow.y), HX8357_WHITE);
  tft.fillRect((DownArrow.x + (DownArrow.Width / 2) - 13), (DownArrow.y ), 26, -40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, DownArrow.y, DownArrow.Width, DownArrow.Height , DownArrow.Round, Touch_Line_Color);

  bmpDraw("IGL50_75.bmp", 5, 275);
}

void Display_Stack_Min () {

  if (Stack_Min_Set_Disp != EEPROM.read(EE_StackMin_addr)) {
    tft.setFont(&FreeSansBold12pt7b);// tft.setFont() FreeSansBold12pt7b;
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    tft.fillRoundRect(30, 30, 125, 94, 10, HX8357_BLACK);
    tft.setCursor(40, 88);
    tft.println(EEPROM.read(EE_StackMin_addr) * 10);
    Stack_Min_Set_Disp = EEPROM.read(EE_StackMin_addr);
  }


  //======== Arrows ============

  tft.fillTriangle ((UpArrow.x + UpArrow.Width / 2), UpArrow.y, UpArrow.x, (UpArrow.y + UpArrow.Height), (UpArrow.x + UpArrow.Width), (UpArrow.y + UpArrow.Height), HX8357_WHITE);
  tft.fillRect((UpArrow.x + (UpArrow.Width / 2) - 13), (UpArrow.y + UpArrow.Height), 26, 40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, UpArrow.y, UpArrow.Width, UpArrow.Height , UpArrow.Round, Touch_Line_Color);
  tft.fillTriangle ((DownArrow.x + DownArrow.Width / 2), (DownArrow.y + DownArrow.Height), DownArrow.x, (DownArrow.y), (DownArrow.x + DownArrow.Width), (DownArrow.y), HX8357_WHITE);
  tft.fillRect((DownArrow.x + (DownArrow.Width / 2) - 13), (DownArrow.y ), 26, -40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, DownArrow.y, DownArrow.Width, DownArrow.Height , DownArrow.Round, Touch_Line_Color);

  bmpDraw("IGL50_75.bmp", 5, 275);
}



void Display_Stack_Max () {

  if (Stack_Min_Set_Disp != EEPROM.read(EE_StackMax_addr)) {
    tft.setFont(&FreeSansBold12pt7b);// tft.setFont() FreeSansBold12pt7b;
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    tft.fillRoundRect(30, 30, 125, 94, 10, HX8357_BLACK);
    tft.setCursor(40, 88);
    tft.println(EEPROM.read(EE_StackMax_addr) * 10);
    Stack_Min_Set_Disp = EEPROM.read(EE_StackMax_addr);
  }


  //======== Arrows ============

  tft.fillTriangle ((UpArrow.x + UpArrow.Width / 2), UpArrow.y, UpArrow.x, (UpArrow.y + UpArrow.Height), (UpArrow.x + UpArrow.Width), (UpArrow.y + UpArrow.Height), HX8357_WHITE);
  tft.fillRect((UpArrow.x + (UpArrow.Width / 2) - 13), (UpArrow.y + UpArrow.Height), 26, 40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, UpArrow.y, UpArrow.Width, UpArrow.Height , UpArrow.Round, Touch_Line_Color);
  tft.fillTriangle ((DownArrow.x + DownArrow.Width / 2), (DownArrow.y + DownArrow.Height), DownArrow.x, (DownArrow.y), (DownArrow.x + DownArrow.Width), (DownArrow.y), HX8357_WHITE);
  tft.fillRect((DownArrow.x + (DownArrow.Width / 2) - 13), (DownArrow.y ), 26, -40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, DownArrow.y, DownArrow.Width, DownArrow.Height , DownArrow.Round, Touch_Line_Color);

  bmpDraw("IGL50_75.bmp", 5, 275);
}



void Display_Set_UpStairs () {

  //=========== Down Stairs Set Point ==========



  if (UpStairs_Position_Set_Disp != EEPROM.read(EE_DesiredRooomTemp_addr)) {
    tft.setFont(&FreeSansBold12pt7b);// tft.setFont() FreeSansBold12pt7b;
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    tft.fillRoundRect(30, 30, 125, 94, 10, HX8357_BLACK);
    tft.setCursor(40, 88);
    tft.println(EEPROM.read(EE_DesiredRooomTemp_addr));
    UpStairs_Position_Set_Disp = EEPROM.read(EE_DesiredRooomTemp_addr);
  }


  //======== Arrows ============

  tft.fillTriangle ((UpArrow.x + UpArrow.Width / 2), UpArrow.y, UpArrow.x, (UpArrow.y + UpArrow.Height), (UpArrow.x + UpArrow.Width), (UpArrow.y + UpArrow.Height), HX8357_WHITE);
  tft.fillRect((UpArrow.x + (UpArrow.Width / 2) - 13), (UpArrow.y + UpArrow.Height), 26, 40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, UpArrow.y, UpArrow.Width, UpArrow.Height , UpArrow.Round, Touch_Line_Color);
  tft.fillTriangle ((DownArrow.x + DownArrow.Width / 2), (DownArrow.y + DownArrow.Height), DownArrow.x, (DownArrow.y), (DownArrow.x + DownArrow.Width), (DownArrow.y), HX8357_WHITE);
  tft.fillRect((DownArrow.x + (DownArrow.Width / 2) - 13), (DownArrow.y ), 26, -40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, DownArrow.y, DownArrow.Width, DownArrow.Height , DownArrow.Round, Touch_Line_Color);

  bmpDraw("IGL50_75.bmp", 5, 275);
}
void Display_Set_DownStairs () {

  //=========== Down Stairs Set Point ==========



  if (DownStairs_Position_Set_Disp != EEPROM.read(EEaddr)) {
    tft.setFont(&FreeSansBold12pt7b);// tft.setFont() FreeSansBold12pt7b;
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    tft.fillRoundRect(30, 30, 125, 94, 10, HX8357_BLACK);
    tft.setCursor(40, 88);
    tft.println(EEPROM.read(EEaddr));
    DownStairs_Position_Set_Disp = EEPROM.read(EEaddr);
  }


  //======== Arrows ============

  tft.fillTriangle ((UpArrow.x + UpArrow.Width / 2), UpArrow.y, UpArrow.x, (UpArrow.y + UpArrow.Height), (UpArrow.x + UpArrow.Width), (UpArrow.y + UpArrow.Height), HX8357_WHITE);
  tft.fillRect((UpArrow.x + (UpArrow.Width / 2) - 13), (UpArrow.y + UpArrow.Height), 26, 40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, UpArrow.y, UpArrow.Width, UpArrow.Height , UpArrow.Round, Touch_Line_Color);
  tft.fillTriangle ((DownArrow.x + DownArrow.Width / 2), (DownArrow.y + DownArrow.Height), DownArrow.x, (DownArrow.y), (DownArrow.x + DownArrow.Width), (DownArrow.y), HX8357_WHITE);
  tft.fillRect((DownArrow.x + (DownArrow.Width / 2) - 13), (DownArrow.y ), 26, -40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, DownArrow.y, DownArrow.Width, DownArrow.Height , DownArrow.Round, Touch_Line_Color);

  bmpDraw("IGL50_75.bmp", 5, 275);
}

void Display_Manual_Overide () {
  int bars_disp;  // used for bar displays

  if (Damp_Position_Set_Disp != Damp_Position) {
    tft.setFont(&FreeSansBold12pt7b);// tft.setFont() FreeSansBold12pt7b;
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    tft.fillRoundRect(30, 30, 125, 94, 10, HX8357_BLACK);
    tft.setCursor(40, 88);
    tft.println(Damp_Position);
  }
  Damp_Position_Set_Disp = Damp_Position;

  tft.setFont();
  tft.setCursor(215, 50);
  tft.setTextColor(HX8357_WHITE);  tft.setTextSize(2);
  tft.println("Damp");
  tft.setCursor(227, 70);
  tft.println("Pos");
  tft.fillRoundRect(220, 103, 35, 114, 10, HX8357_WHITE);
  // tft.drawRoundRect(Damper.x, Damper.y, Damper.Width, Damper.Height , Damper.Round, HX8357_BLACK);


  bars_disp = map( Damp_Position, DampPositionMax, DampPositionMin, 1, 11);
  for (int i = 10; i >= bars_disp ; i--) {
    tft.fillRect (225, (102 + ((i) * 10)), 25, 8, HX8357_RED);

  }

  //======== Arrows ============

  tft.fillTriangle ((UpArrow.x + UpArrow.Width / 2), UpArrow.y, UpArrow.x, (UpArrow.y + UpArrow.Height), (UpArrow.x + UpArrow.Width), (UpArrow.y + UpArrow.Height), HX8357_WHITE);
  tft.fillRect((UpArrow.x + (UpArrow.Width / 2) - 13), (UpArrow.y + UpArrow.Height), 26, 40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, UpArrow.y, UpArrow.Width, UpArrow.Height , UpArrow.Round, Touch_Line_Color);
  tft.fillTriangle ((DownArrow.x + DownArrow.Width / 2), (DownArrow.y + DownArrow.Height), DownArrow.x, (DownArrow.y), (DownArrow.x + DownArrow.Width), (DownArrow.y), HX8357_WHITE);
  tft.fillRect((DownArrow.x + (DownArrow.Width / 2) - 13), (DownArrow.y ), 26, -40 , HX8357_WHITE);
  tft.drawRoundRect(UpArrow.x, DownArrow.y, DownArrow.Width, DownArrow.Height , DownArrow.Round, Touch_Line_Color);


  bmpDraw("IGL50_75.bmp", 5, 275);
}

void UpDate_Display_Normal() {
  int bars_disp;  // used for bar displays

  tft.setFont(&FreeSansBold12pt7b);// tft.setFont() FreeSansBold12pt7b;
  tft.setTextColor(HX8357_WHITE);

  //========= Up Stairs Temp =======
  if (LivingRoomTemp / 10 != LivingRoomTempDisp) {
    tft.setCursor(25, 28);
    tft.setTextSize(1);
    tft.println("Up");
    tft.setCursor(7, 60);
    tft.println("Stairs");
    tft.drawRoundRect(Downstairs.x, Downstairs.y, Downstairs.Width, Downstairs.Height , Downstairs.Round, Touch_Line_Color);
    tft.setTextSize(4);

    tft.fillRoundRect(85, 11, 125, 94, 10, LivingRooomTemp_BackRound);
    tft.setCursor(95, 88);
    tft.println(LivingRoomTemp / 10);
    LivingRoomTempDisp = LivingRoomTemp / 10;
  }


  //========= Down Stairs Temp =======
  if (ThermostatdegF != ThermostatdegFDisp) {
    tft.setCursor(5, 137);
    tft.setTextSize(1);
    tft.println("Down");
    tft.setCursor(5, 167);
    tft.println("Stairs");
    tft.drawRoundRect(Upstairs.x, Upstairs.y, Upstairs.Width, Upstairs.Height , Upstairs.Round, Touch_Line_Color);
    tft.setCursor(110, 192);
    tft.setTextSize(4);
    tft.fillRoundRect(85, 115, 125, 94, 10, ThermostatdegF_BackRound);
    tft.setCursor(95, 192);


    if (ThermostatdegF <= 99) tft.println(ThermostatdegF);
    else
      tft.println(ThermostatdegF-100); //Fix display overflow on TFT 3 digits.

    ThermostatdegFDisp = ThermostatdegF;
  }
  //========= Out-Side Temp =======
  if (Weather_Temperature[0] / 10 != Weather_TemperatureDisp ) {
    tft.setCursor(15, 241);
    tft.setTextSize(1);
    tft.println("Out-");
    tft.setCursor(12, 266);
    tft.println("Side");
    tft.drawRoundRect(Outside.x, Outside.y, Outside.Width, Outside.Height , Outside.Round, Touch_Line_Color);
    tft.setTextSize(4);
    tft.fillRoundRect(85, 219, 125, 94, 10, HX8357_BLACK);
    tft.setCursor(95, 296);

    tft.println(Weather_Temperature[0] / 10);
    Weather_TemperatureDisp = Weather_Temperature[0] / 10;
  }


  tft.drawRoundRect(Weather.x, Weather.y, Weather.Width, Weather.Height , Weather.Round, Touch_Line_Color);

  //=========== IGL ============

  tft.drawRoundRect(IGL.x, IGL.y, IGL.Width, IGL.Height , IGL.Round, Touch_Line_Color);

  //=========== Up Stairs Set Point ==========
  if (DesiredRoomTempDisp != (EEPROM.read(EE_DesiredRooomTemp_addr))) {
    tft.setCursor(228, 25);
    tft.setTextSize(1);
    tft.println("Set");
    tft.setCursor(220, 48);
    tft.println("Point");
    tft.fillRoundRect(UpstairsSet.x, UpstairsSet.y, UpstairsSet.Width, UpstairsSet.Height, UpstairsSet.Round, HX8357_BLACK);
    tft.drawRoundRect(UpstairsSet.x, UpstairsSet.y, UpstairsSet.Width, UpstairsSet.Height , UpstairsSet.Round, Touch_Line_Color);
    tft.setCursor(224, 99);
    tft.setTextSize(2);
    tft.println(EEPROM.read(EE_DesiredRooomTemp_addr));
    DesiredRoomTempDisp = (EEPROM.read(EE_DesiredRooomTemp_addr));
  }
  //=========== Down Stairs Set Point ==========
  if (Set_DownStairsDisp != EEPROM.read(EEaddr)) {
    tft.fillRoundRect(DownstairsSet.x, DownstairsSet.y, DownstairsSet.Width, DownstairsSet.Height, DownstairsSet.Round, HX8357_BLACK);
    tft.drawRoundRect(DownstairsSet.x, DownstairsSet.y, DownstairsSet.Width, DownstairsSet.Height , DownstairsSet.Round, Touch_Line_Color);
    tft.setCursor(224, 156);
    tft.setTextSize(2);
    tft.println(EEPROM.read(EEaddr));
    Set_DownStairsDisp = EEPROM.read(EEaddr);
  }


  //========= Load Timer =======
  if (Load_wood_timerDisp != (End_load_wood_millis - current_millis) / 60000) {
    tft.setFont();
    tft.setCursor(327, 10);
    tft.setTextColor(HX8357_WHITE);  tft.setTextSize(2);
    tft.println("Load");
    tft.setCursor(322, 30);
    tft.println("Timer");
    tft.drawRoundRect(Timer.x, Timer.y, Timer.Width, Timer.Height , Timer.Round, Touch_Line_Color);
    tft.fillRoundRect(322, 50, 60, 40, 10, Load_wood_timer_BackRound );
    tft.setCursor(327, 55);
    tft.setTextColor(HX8357_WHITE);  tft.setTextSize(4);
    if (((End_load_wood_millis - current_millis) / 60000) <= EEPROM.read(EE_Load_Wood_addr)) {
      tft.println ((End_load_wood_millis - current_millis) / 60000);
    }
    else {
      tft.println ("00");
    }

    Load_wood_timerDisp = (End_load_wood_millis - current_millis) / 60000;
  }

  //========= LoadWood =======
  if (!LoadWood_Disp) {
    tft.setFont(&FreeSansBold12pt7b);
    tft.fillRoundRect(LoadWood.x, LoadWood.y, LoadWood.Width, LoadWood.Height , LoadWood.Round, HX8357_WHITE);
    tft.setCursor(327, 135);
    tft.setTextColor(HX8357_BLACK);  tft.setTextSize(2);
    tft.print("Load");
    LoadWood_Disp = true;
  }

  //========= Stack Temp =======
  if (StackdegFDisp != StackdegF) {
    tft.setFont();
    tft.setCursor(408, 10);
    tft.setTextColor(HX8357_WHITE);  tft.setTextSize(2);
    tft.println("Stack");

    tft.setCursor(412, 30);
    tft.println("Temp");

    tft.drawRoundRect(Stack.x, Stack.y, Stack.Width, Stack.Height , Stack.Round, Touch_Line_Color);
    tft.fillRoundRect(400, 50, 80, 40, 10, Stack_Temp_BackRound );
    // shift based on 2 or 3 digits
    if (StackdegF <= 99) {
      tft.setCursor(425, 55);
    }
    else {
      tft.setCursor(405, 55);
    }

    tft.setTextColor(HX8357_WHITE);  tft.setTextSize(4);
    tft.println(StackdegF);

    StackdegFDisp = StackdegF;
  }



  //======== Damper Position Graphic ============
  if (Damp_PositionDisp != Damp_Position) {
    tft.setFont();
    tft.setCursor(315, 150);
    tft.setTextColor(HX8357_WHITE);  tft.setTextSize(2);
    tft.println("Damp");
    tft.setCursor(327, 170);
    tft.println("Pos");
    tft.fillRoundRect(320, 203, 35, 114, 10, HX8357_WHITE);
    tft.drawRoundRect(Damper.x, Damper.y, Damper.Width, Damper.Height , Damper.Round, HX8357_BLACK);


    bars_disp = map( Damp_Position, DampPositionMax, DampPositionMin, 1, 11);
    for (int i = 10; i >= bars_disp ; i--) {
      tft.fillRect (325, (202 + ((i) * 10)), 25, 8, HX8357_RED);
    }
    Damp_PositionDisp = Damp_Position ;
  }

  //======== Fan Speed Graphic ============
  if (Fan_SpeedDisp != Fan_Speed) {
    tft.setCursor(398, 150);
    tft.setTextColor(HX8357_WHITE);  tft.setTextSize(2);
    tft.println("Fan");
    tft.setCursor(380, 170);
    tft.println("Speed");
    tft.fillRoundRect(392, 203, 35, 114, 10, HX8357_WHITE);
    tft.drawRoundRect(Fan.x, Fan.y, Fan.Width, Fan.Height , Fan.Round, HX8357_BLACK);
    //    Serial.println (Fan_Speed);

    bars_disp = map( Fan_Speed, Fan_Speed_min, Fan_Speed_max, 1, 11);
    for (int i = 10; i >= bars_disp ; i--) {
      tft.fillRect (397, (202 + ((i) * 10)), 25, 8, HX8357_RED);
    }
    Fan_SpeedDisp = Fan_Speed ;
  }
  //======== Display Mode ============

  // tft.setCursor(290, 100);
  // tft.setTextColor(HX8357_RED);  tft.setTextSize(2);
  // tft.println("Display Mode");


}
//========
void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if ((x >= tft.width()) || (y >= tft.height())) return;
  /*
    Serial.println();
    Serial.print(F("Loading image '"));
    Serial.print(filename);
    Serial.println('\'');
  */
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    //   Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    //   Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    //    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    //   Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    (read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      //     Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        //       Serial.print(F("Image size: "));
        //       Serial.print(bmpWidth);
        //      Serial.print('x');
        //       Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.startWrite(); // Start TFT transaction
        tft.setAddrWindow(x, y, w, h);

        for (row = 0; row < h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) { // Need seek?
            tft.endWrite(); // End TFT transaction
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
            tft.startWrite(); // Start new TFT transaction
          }

          for (col = 0; col < w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              tft.endWrite(); // End TFT transaction
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
              tft.startWrite(); // Start new TFT transaction
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r, g, b));
          } // end pixel
        } // end scanline
        tft.endWrite(); // End last TFT transaction
        /*
          Serial.print(F("Loaded in "));
          Serial.print(millis() - startTime);
          Serial.println(" ms");
        */
      } // end goodBmp
    }
  }

  bmpFile.close();
  if (!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File & f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File & f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
