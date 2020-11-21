char  *days[] = {"Sunday", "Monday", "Tuesday", "Wendnesday", "Thrusday", "Friday", "Saturday"};

int Year;
int Month;
int Day_int;

String Day_of_Week (String Date_Time_Str) {

  String Year_Str (Date_Time_Str.substring(0, 4));
  Year = Year_Str.toInt();
//  Serial.print ("Year int : ");  Serial.println (Year);

  String Month_Str (Date_Time_Str.substring(5, 7));
  Month = Month_Str.toInt();
//  Serial.print ("Month int : ");  Serial.println (Month);

  String Day_Str (Date_Time_Str.substring(8, 10));
  Day_int = Day_Str.toInt();
//  Serial.print ("Day int : ");  Serial.println (Day_int);

//  Serial.println (days[dayofweek(Day_int, Month, Year) ]);
 

  return days[dayofweek(Day_int, Month, Year) ];
}


int dayofweek(int d, int m, int y)
{
  static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
  y -= m < 3;
  return ( y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}
