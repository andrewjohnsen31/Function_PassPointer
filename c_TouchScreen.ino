/***************************************************
  This is our touchscreen painting example for the Adafruit HX8357 Breakout
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



// This is calibration data for the raw touch data to the screen coordinates
// Calibrated within +/- 10 pixals 05/25/2020
#define TS_MINX 331
#define TS_MINY 191
#define TS_MAXX 823
#define TS_MAXY 871

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 292);

void Update_TouchScreen() {
  // Serial.print("in touch screen");
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || (p.z > MAXPRESSURE) || (p.y == 1023)) {
    return;
  }

  Switch_Pressed = true;
  //  Serial.print("Xraw = "); Serial.println(p.x);
  //  Serial.print("Yraw = "); Serial.println(p.y);
  //   Serial.print("\tPressureraw = "); Serial.println(p.z);

  // Scale from ~0->1000 to tft.width using the calibration #'s
  Pixel_Y = map(p.x, TS_MINX, TS_MAXX, 320, 0); //inverted to get Draw Pixal in same orientation with Touch screen
  Pixel_X = map(p.y, TS_MINY, TS_MAXY, 0, 480);

  //   Serial.print("X = "); Serial.print(Pixel_X);
  //   Serial.print("\tY = "); Serial.println(Pixel_Y);
  //    Serial.print("\tPressure = "); Serial.println(p.z);

  // Check pressed Zone

  // Upstairs
  /*
    Serial.println (Upstairs.x);
    Serial.println ((Upstairs.x + Upstairs.Width));
    Serial.println (Upstairs.y);
    Serial.println ((Upstairs.y + Upstairs.Height));
  */
  switch (menu_position) {
    case 0:     // Normal Screen
      Main_Screen_Buttons();
      //     Serial.println ("menu position");
      break;
    case 1:     // Set Down Stairs Desired temperature
      Set_DownStairs_Buttons();
      break;
    case 2:     // Set Down Stairs Desired temperature
      Set_StackMax_Buttons();
      break;
    case 3:     // Set Down Stairs Desired temperature
      Set_StackMin_Buttons();
      break;
    case 4:
      Set_Wood_Load_Timer_Buttons();
      break;
    case 8:     // Manual Damper Overide
      Set_Manual_Overide_Buttons();
      //     Serial.println ("over ride");
      break;
    case 9:     // Debug TFT text
      Set_Debug_Buttons();
      break;
    case 10:
      Set_Weather_Buttons();
      break;
    case 11:     // Debug TFT text
      Set_UpStairs_Buttons();
      break;
  }
}

void Set_Weather_Buttons () {
  if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL");
    menu_position = 0;

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

    tft.fillScreen(HX8357_DARKCYAN);
    UpDate_BMP();
  }
  else if ((Pixel_X >= Weather_Arrow.x) && (Pixel_X <= (Weather_Arrow.x + Weather_Arrow.Width)) && (Pixel_Y >= Weather_Arrow.y) && (Pixel_Y <= (Weather_Arrow.y + Weather_Arrow.Height))) {
    Serial.println ("Weather_Arrow");
    hr24_Forcast = !hr24_Forcast;
    Weather_Disp = true;

  }
}

void Set_Debug_Buttons () {
  if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL");
    menu_position = 0;

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

    tft.fillScreen(HX8357_DARKCYAN);
    UpDate_BMP();
  }
}

void Set_Wood_Load_Timer_Buttons () {
  if ((Pixel_X >= DownArrow.x) && (Pixel_X <= (DownArrow.x + DownArrow.Width)) && (Pixel_Y >= DownArrow.y) && (Pixel_Y <= (DownArrow.y + DownArrow.Height))) {
    Serial.println ("DownArrow");
    new_eeprom_value = EEPROM.read(EE_Load_Wood_addr) - 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value <= 0) {
      new_eeprom_value = 0;
    }
    EEPROM.write((EE_Load_Wood_addr), new_eeprom_value);
    EEPROM.commit();
  }
  else if ((Pixel_X >= UpArrow.x) && (Pixel_X <= (UpArrow.x + UpArrow.Width)) && (Pixel_Y >= UpArrow.y) && (Pixel_Y <= (UpArrow.y + UpArrow.Height))) {
    Serial.println ("UpArrow")  ;

    new_eeprom_value = EEPROM.read(EE_Load_Wood_addr) + 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value >= 255) {
      new_eeprom_value = 255;
    }
    EEPROM.write((EE_Load_Wood_addr), new_eeprom_value);
    EEPROM.commit();

  }
  else if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL");
    menu_position = 0;

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

    tft.fillScreen(HX8357_DARKCYAN);
    UpDate_BMP();
  }
}

void Set_StackMin_Buttons () {
  if ((Pixel_X >= DownArrow.x) && (Pixel_X <= (DownArrow.x + DownArrow.Width)) && (Pixel_Y >= DownArrow.y) && (Pixel_Y <= (DownArrow.y + DownArrow.Height))) {
    Serial.println ("DownArrow");
    new_eeprom_value = EEPROM.read(EE_StackMin_addr) - 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value <= 0) {
      new_eeprom_value = 0;
    }
    EEPROM.write((EE_StackMin_addr), new_eeprom_value);
    EEPROM.commit();
  }
  else if ((Pixel_X >= UpArrow.x) && (Pixel_X <= (UpArrow.x + UpArrow.Width)) && (Pixel_Y >= UpArrow.y) && (Pixel_Y <= (UpArrow.y + UpArrow.Height))) {
    Serial.println ("UpArrow")  ;

    new_eeprom_value = EEPROM.read(EE_StackMin_addr) + 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value >= 255) {
      new_eeprom_value = 255;
    }
    EEPROM.write((EE_StackMin_addr), new_eeprom_value);
    EEPROM.commit();

  }
  else if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL");
    menu_position = 0;

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

    tft.fillScreen(HX8357_DARKCYAN);
    UpDate_BMP();
  }
}

void Set_StackMax_Buttons () {
  if ((Pixel_X >= DownArrow.x) && (Pixel_X <= (DownArrow.x + DownArrow.Width)) && (Pixel_Y >= DownArrow.y) && (Pixel_Y <= (DownArrow.y + DownArrow.Height))) {
    Serial.println ("DownArrow Max Stack");
    new_eeprom_value = EEPROM.read(EE_StackMax_addr) - 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value <= 0) {
      new_eeprom_value = 0;
    }
    EEPROM.write((EE_StackMax_addr), new_eeprom_value);
    EEPROM.commit();
  }
  else if ((Pixel_X >= UpArrow.x) && (Pixel_X <= (UpArrow.x + UpArrow.Width)) && (Pixel_Y >= UpArrow.y) && (Pixel_Y <= (UpArrow.y + UpArrow.Height))) {
    Serial.println ("UpArrow Max Stack")  ;

    new_eeprom_value = EEPROM.read(EE_StackMax_addr) + 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value >= 255) {
      new_eeprom_value = 255;
    }
    EEPROM.write((EE_StackMax_addr), new_eeprom_value);
    EEPROM.commit();

  }
  else if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL");
    menu_position = 0;

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

    tft.fillScreen(HX8357_DARKCYAN);
    UpDate_BMP();
  }
}



void Set_Manual_Overide_Buttons () {
  if ((Pixel_X >= DownArrow.x) && (Pixel_X <= (DownArrow.x + DownArrow.Width)) && (Pixel_Y >= DownArrow.y) && (Pixel_Y <= (DownArrow.y + DownArrow.Height))) {
    Serial.println ("DownArrow");
    Damp_Position = Damp_Position - 50;
    if (Damp_Position <= DampPositionMin) {
      Damp_Position = DampPositionMin;
    }
  }
  else if ((Pixel_X >= UpArrow.x) && (Pixel_X <= (UpArrow.x + UpArrow.Width)) && (Pixel_Y >= UpArrow.y) && (Pixel_Y <= (UpArrow.y + UpArrow.Height))) {
    Serial.println ("UpArrow")  ;

    Damp_Position = Damp_Position + 50;
    if (Damp_Position >= DampPositionMax) {
      Damp_Position = DampPositionMax;
    }

  }
  else if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL here");
    menu_position = 0;

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

    tft.fillScreen(HX8357_DARKCYAN);
    UpDate_BMP();
  }
}

void Set_DownStairs_Buttons () {
  if ((Pixel_X >= DownArrow.x) && (Pixel_X <= (DownArrow.x + DownArrow.Width)) && (Pixel_Y >= DownArrow.y) && (Pixel_Y <= (DownArrow.y + DownArrow.Height))) {
    Serial.println ("DownArrow");
    new_eeprom_value = EEPROM.read(EEaddr) - 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value <= 0) {
      new_eeprom_value = 0;
    }
    EEPROM.write((EEaddr), new_eeprom_value);
    EEPROM.commit();
  }
  else if ((Pixel_X >= UpArrow.x) && (Pixel_X <= (UpArrow.x + UpArrow.Width)) && (Pixel_Y >= UpArrow.y) && (Pixel_Y <= (UpArrow.y + UpArrow.Height))) {
    Serial.println ("UpArrow")  ;

    new_eeprom_value = EEPROM.read(EEaddr) + 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value >= 255) {
      new_eeprom_value = 255;
    }
    EEPROM.write((EEaddr), new_eeprom_value);
    EEPROM.commit();

  }
  else if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL");
    menu_position = 0;

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

    tft.fillScreen(HX8357_DARKCYAN);
    UpDate_BMP();
  }
}

void Set_UpStairs_Buttons () {
  if ((Pixel_X >= DownArrow.x) && (Pixel_X <= (DownArrow.x + DownArrow.Width)) && (Pixel_Y >= DownArrow.y) && (Pixel_Y <= (DownArrow.y + DownArrow.Height))) {
    Serial.println ("DownArrow");
    new_eeprom_value = EEPROM.read(EE_DesiredRooomTemp_addr) - 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value <= 0) {
      new_eeprom_value = 0;
    }
    EEPROM.write((EE_DesiredRooomTemp_addr), new_eeprom_value);
    EEPROM.commit();
  }
  else if ((Pixel_X >= UpArrow.x) && (Pixel_X <= (UpArrow.x + UpArrow.Width)) && (Pixel_Y >= UpArrow.y) && (Pixel_Y <= (UpArrow.y + UpArrow.Height))) {
    Serial.println ("UpArrow")  ;

    new_eeprom_value = EEPROM.read(EE_DesiredRooomTemp_addr) + 1;
    Serial.print (new_eeprom_value);
    if (new_eeprom_value >= 255) {
      new_eeprom_value = 255;
    }
    EEPROM.write((EE_DesiredRooomTemp_addr), new_eeprom_value);
    EEPROM.commit();

  }
  else if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL");
    menu_position = 0;

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

    tft.fillScreen(HX8357_DARKCYAN);
    UpDate_BMP();
  }
}

void Main_Screen_Buttons () {
  if ((Pixel_X >= Upstairs.x) && (Pixel_X <= (Upstairs.x + Upstairs.Width)) && (Pixel_Y >= Upstairs.y) && (Pixel_Y <= (Upstairs.y + Upstairs.Height))) {
    Serial.println ("Upstairs");

    Regulate_Upstairs = true;   // regulate to upstairs
    adjust_error_from_Upstairs=0;
    
    // refresh regulation numbers
    ThermostatdegFDisp = 0 ;
    StackdegFDisp = 0;
    LivingRoomTempDisp = 0;

  }

  else if ((Pixel_X >= Downstairs.x) && (Pixel_X <= (Downstairs.x + Downstairs.Width)) && (Pixel_Y >= Downstairs.y) && (Pixel_Y <= (Downstairs.y + Downstairs.Height))) {
    Serial.println ("Downstairs");
    Operating_Mode = 2;   // regulate to downstairs

    // refresh regulation numbers
    ThermostatdegFDisp = 0 ;
    StackdegFDisp = 0;
    LivingRoomTempDisp = 0;
    Regulate_Upstairs = false;
    adjust_error_from_Upstairs=0;

  }

  else if ((Pixel_X >= Outside.x) && (Pixel_X <= (Outside.x + Outside.Width)) && (Pixel_Y >= Outside.y) && (Pixel_Y <= (Outside.y + Outside.Height))) {
    Serial.println ("Outside");
  }
  else if ((Pixel_X >= IGL.x) && (Pixel_X <= (IGL.x + IGL.Width)) && (Pixel_Y >= IGL.y) && (Pixel_Y <= (IGL.y + IGL.Height))) {
    Serial.println ("IGL");
    menu_position = 0;
  }
  else if ((Pixel_X >= Weather.x) && (Pixel_X <= (Weather.x + Weather.Width)) && (Pixel_Y >= Weather.y) && (Pixel_Y <= (Weather.y + Weather.Height))) {
    Serial.println ("Weather");
    menu_position = 10;
    hr24_Forcast = true;
    Weather_Disp = true;

  }

  else if ((Pixel_X >= UpstairsSet.x) && (Pixel_X <= (UpstairsSet.x + UpstairsSet.Width)) && (Pixel_Y >= UpstairsSet.y) && (Pixel_Y <= (UpstairsSet.y + UpstairsSet.Height))) {
    Serial.println ("Up Stairs Set");
    menu_position = 11;
    tft.fillScreen(HX8357_DARKCYAN);
    Refresh_Disp = 0;

  }

  else if ((Pixel_X >= DownstairsSet.x) && (Pixel_X <= (DownstairsSet.x + DownstairsSet.Width)) && (Pixel_Y >= DownstairsSet.y) && (Pixel_Y <= (DownstairsSet.y + DownstairsSet.Height))) {
    Serial.println ("Down Stairs Set");
    menu_position = 1;
    tft.fillScreen(HX8357_DARKCYAN);
    Refresh_Disp = 0;
  }
  else if ((Pixel_X >= Timer.x) && (Pixel_X <= (Timer.x + Timer.Width)) && (Pixel_Y >= Timer.y) && (Pixel_Y <= (Timer.y + Timer.Height))) {
    Serial.println ("Timer");
    menu_position = 4;
    tft.fillScreen(HX8357_DARKCYAN);
    Refresh_Disp = 0;


  }
  else if ((Pixel_X >= LoadWood.x) && (Pixel_X <= (LoadWood.x + LoadWood.Width)) && (Pixel_Y >= LoadWood.y) && (Pixel_Y <= (LoadWood.y + LoadWood.Height))) {
    Serial.println ("Load Timer");
    tft.fillRoundRect(LoadWood.x, LoadWood.y, LoadWood.Width, LoadWood.Height , LoadWood.Round, HX8357_CYAN);
    LoadWood_Disp = false;

    //   load_wood_timer = EEPROM.read(EE_Load_Wood_addr);
    End_load_wood_millis = 60000 * EEPROM.read(EE_Load_Wood_addr) + current_millis;
    Damp_Position = DampPositionMax;
    Damper_Servo.write(Damp_Position);  //update servo
    Operating_Mode = 1;   // set to new wood,




  }
  else if ((Pixel_X >= Stack.x) && (Pixel_X <= (Stack.x + Stack.Width)) && (Pixel_Y >= Stack.y) && (Pixel_Y <= (Stack.y + Stack.Height))) {
    Serial.println ("Stack");
    menu_position = 2;
    tft.fillScreen(HX8357_DARKCYAN);
  }
  else if ((Pixel_X >= Fan.x) && (Pixel_X <= (Fan.x + Fan.Width)) && (Pixel_Y >= Fan.y) && (Pixel_Y <= (Fan.y + Fan.Height))) {
    Serial.println ("Fan");  // when fan starts to turn on
    menu_position = 3;
    tft.fillScreen(HX8357_DARKCYAN);
    Refresh_Disp = 0;
  }
  else if ((Pixel_X >= Debug.x) && (Pixel_X <= (Debug.x + Fan.Width)) && (Pixel_Y >= Debug.y) && (Pixel_Y <= (Debug.y + Debug.Height))) {
    Serial.println ("Debug");  // text tft screen for debug.
    menu_position =  9;
    tft.fillScreen(HX8357_BLUE);
    //   Refresh_Disp = 0;
  }
  else if ((Pixel_X >= Damper.x) && (Pixel_X <= (Damper.x + Damper.Width)) && (Pixel_Y >= Damper.y) && (Pixel_Y <= (Damper.y + Damper.Height))) {
    Serial.println ("Damper");
    menu_position = 8;
    tft.fillScreen(HX8357_DARKCYAN);
  }


  else {
    //Serial.println ("missed button");
  }

  // while (Switch_Pressed == true) {
  //   TSPoint p = ts.getPoint();
  //   if (p.z < MINPRESSURE  ) Switch_Pressed = false;
  //   delay (200);
  //  }
}
