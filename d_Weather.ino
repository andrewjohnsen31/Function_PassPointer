


void   Display_Today_Weather() {


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
 // for (int i=5; i<=8; i++){
 //      Serial.print(i); Serial.print (" : "); Serial.print (dateTime_Str[i]); Serial.print ("  "); Serial.print (weatherSymbol[i]); Serial.print ("  "); Serial.println( temperature[i]);

 // }


  // =========== Forcast ============
  String test_Str = "";
  bmpDraw("Partly.bmp", 300, 80);
  tft.setTextSize(3);
  tft.setCursor(150, 100);
  test_Str = "2020-07-30 07:00:0A";
  test_Str = test_Str.substring(11,13);
  tft.println(test_Str);

  bmpDraw("Partly.bmp", 300, 140);
  tft.setCursor(150, 160);
  tft.println("test_6");
  test_Str = "test_6";
  
  bmpDraw("Partly.bmp", 300, 200);
  tft.setCursor(150, 220);
  tft.println("test_7");
  
  bmpDraw("Partly.bmp", 300, 260);
  tft.setCursor(150, 280);
  tft.println("test_8");


  /*

    StartOf = response.indexOf ("timestamp");
    Weather_TimeStamp = (response.substring((StartOf + 13), StartOf + (13 + 19)) ); // "dateTime": "2020-06-29 12:00:00"
  Serial.print ("time stamp :"); Serial.println(Weather_TimeStamp);

    tft.println("Up");
    tft.setCursor(7, 60);

    tft.drawRoundRect(Downstairs.x, Downstairs.y, Downstairs.Width, Downstairs.Height , Downstairs.Round, Touch_Line_Color);
    tft.fillRoundRect(85, 11, 125, 94, 10, HX8357_BLACK);

  */


}
