/*EEPROM availability check
  Function: Prevent safety accidents caused by loading incorrect operating parameters due to damaged EEPROM
*/
void CheckEEPROM() {
  byte w, r;
  int pass = 0, fail = 0;
  for (int i = 0; i < EEPROM.length(); i++) {
    //Memory test program
    w = random(0, 255);
    EEPROM[i] = w;
    r = EEPROM[i];
    if (w == r) pass++; else fail++;
    if (arduboy.nextFrame()) {
      //Display test data
      arduboy.clear();
      arduboy.setTextSize(2);
      SetTextColor(0);
      arduboy.setCursor(0, 0);
      arduboy.print(F(" MEM CHECK "));

      arduboy.setTextSize(1);
      arduboy.setCursor(8, 40);
      arduboy.print(i); arduboy.print(F("/")); arduboy.print(EEPROM.length());
      SetTextColor(1);
      arduboy.setCursor(0 + 8, 24); arduboy.print(F("PASS-")); arduboy.print(pass);
      arduboy.setCursor(64 + 8, 24); arduboy.print(F("FAIL-")); arduboy.print(fail);

      arduboy.setCursor(0 + 8, 32); arduboy.print(F("W -> ")); arduboy.print(w);
      arduboy.setCursor(64 + 8, 32); arduboy.print(F("R -> ")); arduboy.print(r);

      ProgressBar(i, 0, EEPROM.length(), 0, 60, 128, 4, 1);

      arduboy.display();
    }
  }
  //EEPROM memory availability check failed
  while (fail != 0) {
    arduboy.clear();
    arduboy.setTextSize(2);
    SetTextColor(1);
    arduboy.setCursor(22, 16);
    arduboy.print(F("DAMAGED"));
    arduboy.setTextSize(1);
    arduboy.setCursor(31, 44);
    SetTextColor(0);
    if (millis() / 1000 % 2 == 0)arduboy.print(F("BOOT FAILED"));
    arduboy.display();
  }

}

/*View EEPROM memory
   You can scroll up and down to view the data in the EEPROM through the onboard rotary encoder
*/
void ViewEEPRom() {
  setRotary(0, 1023, 4, 0);
  lastbutton = (!digitalRead(BUTTON_PIN));
  arduboy.setTextSize(1);
  do {
    arduboy.clear();
    for (byte y = 0; y < 8; y++) {
      for (byte x = 0; x < 2; x++) {
        if (2 * y + x + getRotary() > 1024) break;
        arduboy.setCursor(x * 64, y * 8);
        arduboy.print(2 * y + x + getRotary());
        arduboy.print(F("->"));
        arduboy.print(EEPROM[2 * y + x + getRotary()]);
        arduboy.setCursor(x * 64 + 54, y * 8);
        arduboy.print((char)EEPROM[2 * y + x + getRotary()]);
      }
    }
    CheckLastButton();
    arduboy.display();
  } while (digitalRead(BUTTON_PIN) || lastbutton);
}
/* Read soldering iron tip parameters from EEPROM
    Input: serial number of the soldering iron tip
    Function: Directly load the parameters of the corresponding serial number soldering iron tip into memory
    Principle:
      1 Calculate the address
      2 Read data
*/
void GetEEPRomTip(byte n) {
  if (n <= TIPMAX) {
    uint16_t Counter = 21;
    //Jump to target address
    Counter += (TIPNAMELENGTH + 1) * n;
    Counter += 5 * 4 * n;
    //Read data from EEPROM and load it into memory
    for (int i = 0; i < (TIPNAMELENGTH + 1); i++) TipName[i] = EEPROM.read(Counter++);
    for (int i = 0; i < 4; i++) EEPROM.get(Counter += 4, PTemp[i]);
  }
}

/* Write soldering iron tip parameters to EEPROM
    Input: serial number of the soldering iron tip
    Function: Load the parameters of the corresponding serial number soldering iron tip from running memory to EEPROM
    Principle:
      1 Calculate the address
      2 Write to EEPROM
*/
void PutEEPRomTip(byte n) {
  if (n <= TIPMAX) {
    uint16_t Counter = 21;
    //Jump to target address
    Counter += (TIPNAMELENGTH + 1) * n;
    Counter += 5 * 4 * n;
    //Read data from EEPROM and load it into memory
    for (int i = 0; i < (TIPNAMELENGTH + 1); i++) EEPROM.update(Counter++, TipName[i]);
    for (int i = 0; i < 4; i++) EEPROM.put(Counter += 4, PTemp[i]);
  }
}

/* Delete a certain soldering iron tip parameter from EEPROM
    Input: soldering iron tip serial number
    Function: Overwrite and move the data bits of the soldering iron tip after the serial number to the position of the serial number to achieve the purpose of deleting data
*/
void DelEEPRomTip(byte n) {
  if (n <= TIPMAX) {
    uint16_t Counter = 21;
    //Jump to target address
    Counter += (TIPNAMELENGTH + 1) * n;
    Counter += 5 * 4 * n;
    //Start to overwrite and move the following data
    byte moveN = NumberOfTips - n - 1;
    while (moveN--) {
      for (int i = Counter; i < Counter + (TIPNAMELENGTH + 1) + 4 * 5; i++) EEPROM[i] = EEPROM[i + (TIPNAMELENGTH + 1) + 4 * 5];
      Counter += (TIPNAMELENGTH + 1) + 4 * 5;
    }
  }
}

// reads user settings from EEPROM; if EEPROM values are invalid, write defaults
void GetEEPROM() {
  uint16_t identifier = (EEPROM.read(0) << 8) | EEPROM.read(1);
  if (identifier == EEPROM_IDENT) {
    DefaultTemp = (EEPROM.read(2) << 8) | EEPROM.read(3);
    SleepTemp   = (EEPROM.read(4) << 8) | EEPROM.read(5);
    BoostTemp   =  EEPROM.read(6);
    Time2sleep  =  EEPROM.read(7);
    Time2off    =  EEPROM.read(8);
    TimeOfBoost =  EEPROM.read(9);
    MainScrType =  EEPROM.read(10);
    PIDenable   =  EEPROM.read(11);
    BeepEnable  =  EEPROM.read(12);
    CurrentTip  =  EEPROM.read(13);
    NumberOfTips = EEPROM.read(14);
    LANG = EEPROM.read(15);
    FlipState = EEPROM.read(16);
    UnderVoltage = EEPROM.read(17);
    Password = (EEPROM.read(18) << 8) | EEPROM.read(19);
    RotaryD = EEPROM.read(20);
  }
  else {
    CheckEEPROM();
    EEPROM.update(0, EEPROM_IDENT >> 8); EEPROM.update(1, EEPROM_IDENT & 0xFF);
    UpdateEEPROM();
  }
  GetEEPRomTip(CurrentTip);
}


// writes user settings to EEPROM using updade function to minimize write cycles
void UpdateEEPROM() {
  EEPROM.update( 2, DefaultTemp >> 8);
  EEPROM.update( 3, DefaultTemp & 0xFF);
  EEPROM.update( 4, SleepTemp >> 8);
  EEPROM.update( 5, SleepTemp & 0xFF);
  EEPROM.update( 6, BoostTemp);
  EEPROM.update( 7, Time2sleep);
  EEPROM.update( 8, Time2off);
  EEPROM.update( 9, TimeOfBoost);
  EEPROM.update(10, MainScrType);
  EEPROM.update(11, PIDenable);
  EEPROM.update(12, BeepEnable);
  EEPROM.update(13, CurrentTip);
  EEPROM.update(14, NumberOfTips);
  EEPROM.update(15, LANG);
  EEPROM.update(16, FlipState);
  EEPROM.update(17, UnderVoltage);
  EEPROM.update(18, Password >> 8);
  EEPROM.update(19, Password & 0xFF);
  EEPROM.update(20, RotaryD);
  PutEEPRomTip(CurrentTip);
}
