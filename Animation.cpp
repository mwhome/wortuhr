#include "Animation.h"

extern s_myanimation myanimation;
extern String myanimationslist[MAXANIMATION];
extern uint32_t anipalette[];
extern ESP8266WebServer webServer;

// Animationsliste
void getAnimationList()
{
  int aidx = 0;
  int sidx = 0;
  String Aname;
  uint8_t lidx = 0;
  for ( uint8_t z = 0;z < MAXANIMATION;z++) myanimationslist[z] = "";
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {    
    if (!dir.isDirectory())
    {
      aidx = dir.fileName().indexOf("ani_");
      if ( aidx >= 0 ) 
      {
        sidx = dir.fileName().lastIndexOf(".json");
        if ( sidx >= 0)
        {
          Aname = dir.fileName().substring(aidx+4,sidx);
          if ( Aname != "NEU" ) 
          {
            myanimationslist[lidx] = Aname;
//            Serial.println ( myanimationslist[lidx] );
            lidx++;
          }
        }
      }
    }
    yield();
  }

// sortieren
  for (int i = 0; i < lidx; i++) {
    for (int j = i + 1; j < lidx; j++) {
      if (myanimationslist[j] < myanimationslist[i]) {
        std::swap(myanimationslist[i], myanimationslist[j]);
      }
    }
  }
  saveAnimationsListe();
}

void saveAnimationsListe()
{ 
//  String filename = "
  String anioutput;
  int bytesWritten = 0;
  bool fk = false;
  
  File file = LittleFS.open(ANIMATIONSLISTE, "w");
  if (!file) {
    Serial.println(F("Save Animation: Failed to create file"));
  }
   anioutput = F("{\n \"Animationsliste\" : [ "); 
   for ( uint8_t z = 0;z < MAXANIMATION;z++) 
   {
     
     if ( myanimationslist[z].length() > 2 ) {
      if ( fk ) anioutput += F(",");
      anioutput+= "\"" + myanimationslist[z] + "\"";
     }
     fk = true;
   }
   anioutput+= F(" ]\n}\n");
   bytesWritten = file.print(anioutput);
#ifdef DEBUG_ANIMATION
   if (bytesWritten > 0) 
   {
    Serial.println(F("File " ANIMATIONSLISTE " was written"));
    Serial.println(bytesWritten);
   } else {
    Serial.println(F("File " ANIMATIONSLISTE " write failed"));
   }
#endif
  // Close the file
  file.close();
}

// Lade die Animation
bool loadAnimation(String aniname )
{
#ifdef DEBUG_ANIMATION
  Serial.print(F("Loading Animation "));
  Serial.println (aniname);
#endif
  String filename = "/ani_" + aniname + ".json";
  String filezeile;
  String typ;
  String wert;
  uint32_t farbwert;
  uint8_t frame = 0;
  uint8_t zeile = 0;
  String jsonBuffer;

  File file = LittleFS.open(filename,"r");
  if(!file)
  {
   Serial.print(F("There was an error opening the file "));
   Serial.print(aniname);
   Serial.println(F(" for reading"));
   return false;
  }
  // animation leer initialisieren
  for ( uint8_t f = 0;f < MAXFRAMES;f++)
  {
     myanimation.frame[f].delay = 0;
     for ( uint8_t zeile = 0;zeile <= 9;zeile++)
     {
        for ( uint8_t x = 0;x <= 10;x++)
        {
           myanimation.frame[f].color[x][zeile] = num_to_color(0);
        }
     }          
  }
  while (file.available()) 
  {
    filezeile = file.readStringUntil('\n');
    typ = filezeile.substring(0,filezeile.indexOf(":"));
    wert = filezeile.substring(filezeile.indexOf(":")+1);
    
    typ.replace("\"","");
    typ.trim();
    
    wert.replace(",","");
    wert.replace("\"","");
    wert.trim();
    
//    Serial.printf("typ: %s = %s\n" , typ.c_str(), wert.c_str());
    
    if ( typ == "Name" ) aniname.toCharArray(myanimation.name,aniname.length()+1);
    if ( typ == "Loops" ) myanimation.loops = wert.toInt();
    if ( typ == "Laufmode" ) myanimation.laufmode = wert.toInt();
    if ( typ == "Palette") 
    {
      jsonBuffer = filezeile;
      jsonBuffer.replace("],","]");
      jsonBuffer = '{' + jsonBuffer + "}";
//      Serial.print( "jsonBuffer0: " );
//      Serial.println (jsonBuffer);
      if ( !loadjsonarry(-1,0,jsonBuffer) ) return false;  //Load Palette
      jsonBuffer="";
    }
    if ( typ.substring(0,5) == "Frame" ) frame = typ.substring(typ.indexOf("_")+1).toInt();
    if ( typ == "Delay") myanimation.frame[frame].delay = wert.toInt();
    if ( typ.substring(0,5) == "Zeile" ) 
    {
      zeile = typ.substring(typ.indexOf("_")+1).toInt();
      jsonBuffer = filezeile;
      jsonBuffer.replace("],","]");
      jsonBuffer = '{' + jsonBuffer + "}";
      
      if ( !loadjsonarry(frame,zeile,jsonBuffer) ) return false; //Load Zeilen pro Frame
      jsonBuffer="";
    }
//    Serial.printf("AnimationFrames ESP.getMaxFreeBlockSize: %i Codezeile: %u\n", ESP.getMaxFreeBlockSize(),  __LINE__);
    yield();
  }
  file.close();
#ifdef DEBUG_ANIMATION
  Serial.print( "myanimation.name: " );
  Serial.println (myanimation.name);
  Serial.print( "myanimation.loops: " );
  Serial.println (myanimation.loops); 
  Serial.print( "myanimation.laufmode: " );
  Serial.println (myanimation.laufmode);    
  Serial.print( "anipalette: " );
  for ( uint8_t palidx = 0;palidx <= 9;palidx++)
  {
    Serial.print(num_to_string(anipalette[palidx]));
    Serial.print(" ");
  }
  Serial.println("");
  for ( uint8_t f = 0;f < MAXFRAMES;f++)
  {
    Serial.print ("myanimation.frame: ");
    Serial.println (f);
    Serial.print ("myanimation.frame.delay: ");
    Serial.println (myanimation.frame[f].delay);
    for ( uint8_t zeile = 0;zeile <= 9;zeile++)
    {
      Serial.print ("myanimation.frame.zeile: ");
      Serial.print (zeile);
      Serial.print (": ");
      for ( uint8_t x = 0;x <= 10;x++)
      {
        Serial.print (color_to_string(myanimation.frame[f].color[x][zeile]));
        Serial.print (" ");
      }
      Serial.println(" ");
    }          
  }
  Serial.printf("AnimationFrames ESP.getMaxFreeBlockSize: %i Codezeile: %u\n", ESP.getMaxFreeBlockSize(),  __LINE__);
#endif
  return true;
}

bool loadjsonarry( int8_t frame, uint8_t zeile, String &jsonBuffer)
{
  String farbwert;
  String zeile_json;
  JSONVar myObject = JSON.parse(jsonBuffer);

  if (JSON.typeof(myObject) == "undefined") { Serial.println(F("Load JSON_ARRAY: Parsing the Animationsfile failed")); return false;}
  delay(0);
  if ( frame == -1 && zeile == 0 ) 
  {
    for ( uint8_t palidx = 0;palidx <= 9;palidx++)
    {
       farbwert = (const char*)myObject["Palette"][palidx];
       anipalette[palidx]=string_to_num(farbwert);
    }
  }
  else
  {
    zeile_json = "Zeile_" + String(zeile);
    for ( uint8_t x = 0;x < myObject[zeile_json].length();x++)
    {
      farbwert = (const char*)myObject[zeile_json][x];
//      Serial.printf ("pixel x/y %i/%i = %s ",x,zeile,farbwert.c_str());
      myanimation.frame[frame].color[x][zeile] = string_to_color(farbwert);
    }
  }
  return true;
}

// Farbwert (z.B. #FF00DD ) von HEX nach int wandeln
uint32_t string_to_num(String in_color)
{
  char buffer[9];
  in_color.substring(1, 7).toCharArray(buffer,7);
  return strtol(buffer, 0, 16);
}

// Farbwert (z.B. #FF00DD ) von HEX nach String wandeln
String num_to_string(uint32_t in_color)
{
  String farbe;
  farbe = "000000" + String(in_color,HEX);
  farbe = "#" + farbe.substring( farbe.length()-6,farbe.length());
  farbe.toUpperCase();
  return farbe;
}

// Farbwert (z.B. red, green, blue ) nach String wandeln
String color_to_string(color_s in_color)
{
  String farbe;
  uint32_t colbuf = 0;
  colbuf = in_color.red<<16;
  colbuf = colbuf | in_color.green<<8;
  colbuf = colbuf | in_color.blue;
  return num_to_string(colbuf);
}

// Farbwert string -> s_color
color_s string_to_color(String in_color)
{
  return num_to_color(string_to_num(in_color));
}
// Farbwert num -> s_color (z.B. 0xFF00DD) nach red, green, blue
color_s num_to_color(uint32_t in_color)
{
  color_s s_color;
  s_color.red = in_color >> 16 & 0xFF;
  s_color.green = in_color >> 8 & 0xFF;
  s_color.blue = in_color & 0xFF;
  return s_color;
}
