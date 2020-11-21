
//===== load array from Resources.h ==========
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA)



//===== Strings used to build Post string requests =============
String access_token = "";
String refresh_token = "";
String ecobeePin = "";
String code = "";

char test_c ;
char comma = ',';
char double_quote = '"';
String test_response ;
int response_stepping;

int status = WL_IDLE_STATUS;

//===== JSON max content size
const size_t MAX_CONTENT_SIZE = 1600;

//===== client response info ===========
int statuscode = 0;
String response = "";

char server[] = "api.ecobee.com";

WiFiSSLClient wifiClient;
HttpClient client = HttpClient(wifiClient, server, 443);

//============================================
String Stream_str_Read_JSON (char* Search_for) {
  String JSON_Response;
  char test_c;

  // Serial.println ("======  after cat Stream_int_Read_JSON ==========");
  int find_length = strlen(Search_for);
  //  Serial.println ("======  after sizeof Stream_int_Read_JSON ==========");

  if (Debug_temperature) {
    Serial.print("=========Search_for : ");
    Serial.println(Search_for);
  }
  if (!client.find( Search_for, find_length)) {
    Serial.print(F("Failed client.find : "));
    Serial.print (Search_for);
    //   return;
  }
  else {
    if (Debug_temperature) {
      Serial.print("found Stream_int_Read_JSON: "); Serial.println (Search_for);
    }
    JSON_Response = "";
    test_c = client.read();
    do {
      JSON_Response += (test_c);
      test_c = client.read();

    } while (test_c != '"'); //finish reading char_response

  }
  return JSON_Response;
}
//==================================

//============================================
int Stream_int_Read_JSON (char* Search_for) {
  String JSON_Response;
  char test_c;

  // Serial.println ("======  after cat Stream_int_Read_JSON ==========");
  int find_length = strlen(Search_for);
  //  Serial.println ("======  after sizeof Stream_int_Read_JSON ==========");
  if (Debug_temperature) {
    Serial.print("=========Search_for : ");
    Serial.println(Search_for);
  }
  if (!client.find( Search_for, find_length)) {
    Serial.print(F("Failed client.find : "));
    Serial.println (Search_for);
  }
  else {
    if (Debug_temperature) {
      Serial.print("found Stream_int_Read_JSON: "); Serial.println (Search_for);
    }
    JSON_Response = "";
    test_c = client.read();
    do {
      JSON_Response += (test_c);
      test_c = client.read();

    } while (test_c != ','); //finish reading char_response

  }
  return JSON_Response.toInt();
}

//==================================

void writeEEstringFlash(char add, String data) {
  int _size = data.length();
  int i;
  for (i = 0; i < _size; i++)
  {
    EEPROM.write(add + i, data[i]);
  }
  EEPROM.write(add + _size, '\0'); //Add termination null character for String Data
  EEPROM.commit();
}


String readEEstringFlash(char add) {
  int i;
  char data[7000]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < address_null_termination + 1) //Read until null character
  {
    k = EEPROM.read(add + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  return String(data);
}


void Extract_Json_Strings (String data, String & Var_1, String Var_1_json, String & Var_2, String Var_2_json) { // use of ampersand passes pointers
  StaticJsonDocument <1000> jsonDoc; // changed from 500 to 1500 6/28 Nano 33 test
  auto error = deserializeJson(jsonDoc, data);
  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
  }
  else {
    String tmp1 = jsonDoc[Var_1_json];
    Var_1 = tmp1 ;                            // this is the only way I could get this access_token updated
    String tmp2 = jsonDoc[Var_2_json];
    Var_2 = tmp2 ;                            // this is the only way I could get this access_token updated
    if (Debug_temperature) {
      Serial.print (Var_1_json);
      Serial.print (": ");
      Serial.println (Var_1);
      Serial.print (Var_2_json);
      Serial.print (": ");
      Serial.println (Var_2);
    }
  }
}




void create_new_refresh_keys (String & refresh_token_loc, String & access_token_loc) {


  client.beginRequest();

  //======= get ecobeePin ===================
  // converted from JavaScript: https://www.ecobee.com/home/developer/api/examples/ex1.shtml
  // ======== update for jwt 12/01/2020 requirment
  // Authorize Request
  client.get("https://api.ecobee.com/authorize?response_type=ecobeePin&client_id=dRc7CqSwIxKI7pNp3ZMDqJGPY7aT9akm&scope=smartWrite");
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  if (Debug_temperature) {
    Serial.print("ecobeePin Status code: "); Serial.println(statusCode);
    Serial.print("Authorize Response: "); Serial.println(response);
    Serial.print ("FreeMemory"); Serial.println( freeMemory());
  }

  Extract_Json_Strings(response, ecobeePin, "ecobeePin", code, "code");
  //writeEEstring ("refresh.txt", refresh_token);       // Save for next running of Setup/Refresh
  writeEEstringFlash (address_refresh_token, refresh_token);
  if (Debug_temperature) {
    Serial.print ("Aurthorization code: "); Serial.println (code );
    Serial.print ("ecobeePin: "); Serial.println (ecobeePin );
    Serial.println ("go to ecobee.com MY APPS");
    Serial.println ("click add appliaction & paste ecobeePin into autorization code");
    Serial.println ("click add app in lower right corner");
    Serial.print ("FreeMemoryafter Json extraction: "); Serial.println( freeMemory());
  }




  menu_position =  9;
  tft.fillScreen(HX8357_BLUE);
  tft.setCursor(0, 0);
  tft.setFont();


  tft.setTextSize(5);
  tft.print ("ecobeePin: ");
  tft.println (ecobeePin );
  tft.setTextSize(2);
  tft.println (" ");
  tft.println ("Aurth code: ");
  tft.println (code );
  tft.println (" ");

  tft.setTextSize(1);
  tft.println ("go to ecobee.com MY APPS");
  tft.println ("click add appliaction & paste ecobeePin into autorization code");
  tft.drawLine(0, 140, 480, 140, HX8357_WHITE);
  tft.drawLine(0, 142, 480, 142, HX8357_WHITE);



  //======= get ecobee Aurthorization code, final step to get Access and Refresh codes ===================
  // converted from JavaScript: https://www.ecobee.com/home/developer/api/examples/ex1.shtml
  refresh_token = "null";
  delay (20000);
  int i = 0;
  while (refresh_token == "null" || i == 6) {
    //  client.post("https://api.ecobee.com/token?grant_type=ecobeePin&code=" + code + "&client_id=" + SECRET_Client_ID);
    client.post("https://api.ecobee.com/token?grant_type=ecobeePin&code=" + code + "&client_id=" + SECRET_Client_ID + "&ecobee_type=jwt");

    client.endRequest();
    // read the status code and body of the response
    statusCode = client.responseStatusCode();
    response = client.responseBody();
    if (Debug_temperature) {

      Serial.println("message sent :  ");
      Serial.println("https://api.ecobee.com/token?grant_type=ecobeePin&code=" + code + "&client_id=" + SECRET_Client_ID + "&ecobee_type=jwt");

      Serial.print("Aurthorization Status code: "); Serial.println(statusCode);
      Serial.print("Refresh Response: "); Serial.println(response);
    }

    tft.setCursor(0, 150);
    tft.setTextSize(2);
    tft.println(response);

    Extract_Json_Strings(response, access_token_loc, "access_token", refresh_token_loc, "refresh_token");
    //    writeEEstring ("refresh.txt", refresh_token_loc);       // Save for next running of Setup/Refresh
    writeEEstringFlash (address_refresh_token, refresh_token);
    if (Debug_temperature) {
      Serial.print ("refresh_token: ");
      Serial.println (refresh_token_loc );
      Serial.print ("access_token: ");
      Serial.println (access_token_loc );
    }
    delay (10000);                          //10 Second Delay
    tft.fillRoundRect(0, 150, 480, 329, 10, HX8357_BLACK);

  }
  // Exit Debug mode TFT display
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
  share_to_Debug = false;

  tft.fillScreen(HX8357_DARKCYAN);
  UpDate_BMP();

}

void Update_Weather_Strings() {

  // remove http request, work with fixed JSON input
  //  String response_weather;
  //  boolean error;
  String postData;
  String statusCode;
  //======= Send Request for Ecobee rooom temperature & Desired temperature ===================
  postData = "{\"selection\":{\"selectionType\":\"registered\",\"selectionMatch\":\"\",\"includeRuntime\":false,\"includeWeather\":true}}";
  //  client.useHTTP10(true);
  client.beginRequest();
  client.get("https://api.ecobee.com/1/thermostat?format=json&body=" + postData);   //moved postData here & changed to a GET
  //  client.useHTTP10(true);
  client.sendHeader("Authorization", ("Bearer " + access_token));
  client.sendHeader("Content-Type", "text/json");
  client.endRequest();

  // ========  read the status code and body of the response  ===========
  statusCode = client.responseStatusCode();


  //======== Get Time Stamp ===========

  Weather_TimeStamp = Stream_str_Read_JSON("timestamp\": \"");
  if (Debug_temperature) {
    Serial.print("extract timestamp : ");
    Serial.println(Weather_TimeStamp);
  }

  //-------------------Start [0]----------------------------------------

  // =========== Get Weather Symbol 0
  Weather_Symbol[0] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_0 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_0 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[0] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[0] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[0] = Stream_int_Read_JSON("tempLow\":");
  Weather_Symbol[1] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_1 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_1 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[1] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[1] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[1] = Stream_int_Read_JSON("tempLow\":");
  Weather_Symbol[2] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_2 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_2 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[2] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[2] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[2] = Stream_int_Read_JSON("tempLow\":");
  Weather_Symbol[3] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_3 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_3 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[3] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[3] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[3] = Stream_int_Read_JSON("tempLow\":");
  Weather_Symbol[4] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_4 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_4 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[4] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[4] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[4] = Stream_int_Read_JSON("tempLow\":");
  Weather_Symbol[5] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_5 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_5 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[5] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[5] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[5] = Stream_int_Read_JSON("tempLow\":");
  Weather_Symbol[6] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_6 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_6 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[6] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[6] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[6] = Stream_int_Read_JSON("tempLow\":");
  Weather_Symbol[7] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_7 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_7 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[7] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[7] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[7] = Stream_int_Read_JSON("tempLow\":");
  Weather_Symbol[8] = Stream_int_Read_JSON("weatherSymbol\":");
  Weather_DateTime_8 = Stream_str_Read_JSON("dateTime\": \"");
  Weather_Condition_8 = Stream_str_Read_JSON("condition\": \"");
  Weather_Temperature[8] = Stream_int_Read_JSON("temperature\":");
  Weather_TempHigh[8] = Stream_int_Read_JSON("tempHigh\":");
  Weather_TempLow[8] = Stream_int_Read_JSON("tempLow\":");


  while (client.available()) {
    test_c = client.read();
    if (Debug_temperature) {
      Serial.print(test_c);
      Serial.print ("FreeMemoryafter after weather read clear: ");
      Serial.println( freeMemory());
    }
  }

  /*
    Serial.println ("==== 0 =====");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_0);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[0]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_0);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[0]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[0]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[0]);

    Serial.println ("==== 1 =====");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_1);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[1]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_1);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[1]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[1]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[1]);

    Serial.println ("==== 2 ===== ");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_2);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[2]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_2);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[2]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[2]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[2]);

    Serial.println (" == == 3 == == = ");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_3);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[3]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_3);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[3]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[3]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[3]);

    Serial.println (" == == 4 == == = ");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_4);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[4]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_4);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[4]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[4]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[4]);

    Serial.println ("==== 5 =====");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_5);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[5]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_5);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[5]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[5]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[5]);

    Serial.println ("==== 6 =====");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_6);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[6]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_6);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[6]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[6]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[6]);

    Serial.println ("==== 7 =====");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_7);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[7]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_7);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[7]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[7]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[7]);

    Serial.println ("==== 8 =====");
    Serial.print ("Date/Time : "); Serial.println (Weather_DateTime_8);
    Serial.print ("Symbol : "); Serial.println (Weather_Symbol[8]);
    Serial.print ("Condition : "); Serial.println (Weather_Condition_8);
    Serial.print ("Temperature : "); Serial.println (Weather_Temperature[8]);
    Serial.print ("Temp High : "); Serial.println (Weather_TempHigh[8]);
    Serial.print ("Temp Low : "); Serial.println (Weather_TempLow[8]);

  */



}

void Connect_to_Network() {
  //==========

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {                        // Connect to WPA/WPA2 network:
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);                                         // wait 10 seconds for connection:
  }

  Serial.println("You're connected to the network");
  //printCurrentNet();    Leave for Debug
  //printWifiData();

}


void UpDate_Temperatures() {
  if (Debug_temperature) {
    Serial.println("Starting connection to server...");     // if you get a connection, report back via serial:
  }
  client.beginRequest();

  //======= get latest refresh code from EEProm to use for refresh tokens ===================
  //  refresh_token = readEEstring("refresh.txt");
  refresh_token = readEEstringFlash(address_refresh_token);
  client.post("https://api.ecobee.com/token?grant_type=refresh_token&refresh_token=" + refresh_token + "&client_id=" + SECRET_Client_ID);
  client.endRequest();
  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  if (statusCode == 200) {
    access_token = Stream_str_Read_JSON("access_token\": \"");
    refresh_token = Stream_str_Read_JSON("refresh_token\": \"");

    while (client.available()) {
      test_c = client.read();
      if (Debug_temperature) {
        Serial.print(test_c);
      }
    }
    writeEEstringFlash (address_refresh_token, refresh_token);
  }
  else {
    Serial.println ("Error in obtaining access code, need to run create_new_refresh_keys ()");
    Serial.print ("Error StatusCode != 200 : "); Serial.println (statusCode);
    Serial.println ( "failed to get access_token & refresh_token : ");
    create_new_refresh_keys (refresh_token, access_token);
  }

  if (Debug_temperature) {
    Serial.print ("refresh_token: ");
    Serial.println (refresh_token );
    Serial.print ("access_token/Bearer: ");
    Serial.println (access_token );
    Serial.println ();
  }

  //======= Send Request for Ecobee rooom temperature & Desired temperature ===================
  String postData = "{\"selection\":{\"selectionType\":\"registered\",\"selectionMatch\":\"\",\"includeRuntime\":true,\"includeWeather\":false}}";
  client.beginRequest();
  client.get("https://api.ecobee.com/1/thermostat?format=json&body=" + postData);   //moved postData here & changed to a GET
  client.sendHeader("Authorization", ("Bearer " + access_token));
  client.sendHeader("Content-Type", "text/json");
  client.endRequest();

  statusCode = client.responseStatusCode();
  if (statusCode == 200) {
    LivingRoomTemp = Stream_int_Read_JSON("actualTemperature\":");
    DesiredRoomTemp = Stream_int_Read_JSON("desiredHeat\":");
    while (client.available()) {
      test_c = client.read();
      if (Debug_temperature) {
        Serial.print(test_c);
      }
    }
    float x = LivingRoomTemp;
    x = x / 10;
    if (Debug_temperature) {
      Serial.print("LivingRoomTemp: ");
      Serial.println (x, 1);
    }
    x = DesiredRoomTemp;
    x = x / 10;
    if (Debug_temperature) {
      Serial.print("Disired Temp: ");
      Serial.println (x, 1);
      Serial.print ("FreeMemoryafter clear response: ");
      Serial.println( freeMemory());
    }
    Update_Weather_Strings();

    Serial.print ("FreeMemoryafter after Update_Weather_String: ");
    Serial.println( freeMemory());
  }
  else {
    Serial.print ("Error StatusCode != 200 : "); Serial.println (statusCode);
    Serial.println ( "failed to get Temperature : ");
  }

}
