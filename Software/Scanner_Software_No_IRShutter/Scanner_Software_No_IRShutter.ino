#include <EEPROM.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
//#include <multiCameraIrControl.h>
#include <AccelStepper.h>

int cursor = 1;
byte error, add;
int I2C;
int val;
int val1;
int val2;
int val3;
int val4;
int FpR; // Fotos per Rotation
int Startangle; //Vertical startangle
int Positions; //number of vertical positions
int TpF; //Time per Foto
int minVert; //minimal vertical angle
int menu;
int buttonUP = 11; //pin for button up
int buttonDOWN = 10; //pin for button down
int buttonOK = 9; //pin for button ok
int step1startpin = 7; //Stepper1 start pin
int step2startpin = 4; //Stepper2 start pin
int photopin1 = 3; //CameraPin
int step1dirpin = 4; //Stepper1 direction pin
int step2dirpin = 6; //Stepper2 direction pin
int step1steppin = 5; //Stepper1 step pin
int step2steppin = 7; //Stepper2 step pin
int SpR1 = 3200; //Steps per Revolution (Stepper1)
int SpR2 = 16000; //Steps per Revolution (Stepper2)
int totalphotos =0;
int releasetime = 100; // 1500ms for cam and 300 for iphone



AccelStepper Stepper1(1, step1steppin, step1dirpin); // TABLE!!
AccelStepper Stepper2(1, step2steppin, step2dirpin); //(ROTOR!!!)

void setup() {

    Serial.begin(9600);
 
  Wire.begin();  ////////////////////////////////////////////////////////////Scanning LCD I2C Address - LCD Setup
  for(add = 1; add < 127; add++ )
  {
    Wire.beginTransmission(add);
    error = Wire.endTransmission();
 
    if (error == 0)
    { I2C=add;
      add=128;
    }
  }
  #define address I2C
  LiquidCrystal_I2C  lcd(address, 2, 1, 0, 4, 5, 6, 7);
  lcd.begin (20, 4); 
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  Stepper1.setMaxSpeed(1000); //set max speed the motor will turn (steps/second)   //KREIS
  Stepper1.setAcceleration(200); //set acceleration (steps/second^2)
  Stepper2.setMaxSpeed(5000); //set max speed the motor will turn (steps/second)   // TURNTABLE
  Stepper2.setAcceleration(1000); //set acceleration (steps/second^2)
  Stepper1.setCurrentPosition(0);
  Stepper2.setCurrentPosition(0);

  pinMode(buttonUP, INPUT_PULLUP);
  pinMode(buttonDOWN, INPUT_PULLUP);
  pinMode(buttonOK, INPUT_PULLUP);

  pinMode(photopin1,OUTPUT);

  // FIRST SETUP EVER //
  if (EEPROM.read(1) != 103 || EEPROM.read(2) != 101 || EEPROM.read(3) != 101) {
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("initializing");
    delay(100);
      
    EEPROM.write(4, 25);
    EEPROM.write(5, 4);
    EEPROM.write(6, 4);
    EEPROM.write(7, 10);
    EEPROM.write(100, 1);
    EEPROM.write(101, 16);
    EEPROM.write(102, 0);

    EEPROM.write(8, 16); EEPROM.write(9, 0); EEPROM.write(10, 1);
    EEPROM.write(11, 32); EEPROM.write(12, 0); EEPROM.write(13, 1);
    EEPROM.write(14, 64); EEPROM.write(15, 0); EEPROM.write(16, 1);

    EEPROM.write(17, 16); EEPROM.write(18, 25); EEPROM.write(19, 3);
    EEPROM.write(20, 16); EEPROM.write(21, 50); EEPROM.write(22, 3);
    EEPROM.write(23, 16); EEPROM.write(24, 75); EEPROM.write(25, 3);

    EEPROM.write(26, 16); EEPROM.write(27, 25); EEPROM.write(28, 5);
    EEPROM.write(29, 16); EEPROM.write(30, 50); EEPROM.write(31, 5);
    EEPROM.write(32, 16); EEPROM.write(33, 75); EEPROM.write(34, 5);

    EEPROM.write(35, 32); EEPROM.write(36, 25); EEPROM.write(37, 3);
    EEPROM.write(38, 32); EEPROM.write(39, 50); EEPROM.write(40, 5);
    EEPROM.write(41, 32); EEPROM.write(42, 75); EEPROM.write(43, 5);
    
    EEPROM.write(44, 1);

    EEPROM.write(1, 103);
    EEPROM.write(2, 101);
    EEPROM.write(3, 101);


  }
  menu = 1;
  }

void loop() {

  #define address I2C
  LiquidCrystal_I2C  lcd(address, 2, 1, 0, 4, 5, 6, 7);
  lcd.begin (20, 4); 
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  while(1){
  while (menu != 0) {

    if (menu == 1) {             ///////////////////////////////////////// 1 Main Menu //
      cursor = 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("3D Scanner v1.0");
      lcd.setCursor (0, 1);
      lcd.print("-> Run Program");
      lcd.setCursor (3, 2);
      lcd.print("Settings");
      while (digitalRead(buttonOK) == LOW) {
        delay(10);
      }
      while (digitalRead(buttonOK) == HIGH) {
        if (digitalRead(buttonDOWN) == LOW && cursor < 2) {
          lcd.setCursor(0, cursor);
          lcd.print("  ");
          cursor = cursor + 1;
          lcd.setCursor(0, cursor);
          lcd.print("->");
          delay(300);
        }
        if (digitalRead(buttonUP) == LOW && cursor > 1) {
          lcd.setCursor(0, cursor);
          lcd.print("  ");
          cursor = cursor - 1;
          lcd.setCursor(0, cursor);
          lcd.print("->");
          delay(300);
        }
      }
      if (cursor == 1) {
        menu = 2;
      }
      if (cursor == 2) {
        menu = 3;
      }
      delay(300);
    }

    if (menu == 2) {                   ////////////////////////////////////////// 2 Run Program //
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("-> Run last Program");
      lcd.setCursor(3, 1);
      lcd.print("Load Program");
      lcd.setCursor(3, 2);
      lcd.print(" back");
      cursor = 0;
      while (digitalRead(buttonOK) == LOW) {
        delay(10);
      }
      while (digitalRead(buttonOK) == HIGH) {
        if (digitalRead(buttonDOWN) == LOW && cursor < 2) {
          lcd.setCursor(0, cursor);
          lcd.print("  ");
          cursor = cursor + 1;
          lcd.setCursor(0, cursor);
          lcd.print("->");
          delay(300);
        }
        if (digitalRead(buttonUP) == LOW && cursor > 0) {
          lcd.setCursor(0, cursor);
          lcd.print("  ");
          cursor = cursor - 1;
          lcd.setCursor(0, cursor);
          lcd.print("->");
          delay(300);
        }
      }
      if (cursor == 0) {
        menu = 21;
      }
      if (cursor == 1) {
        menu = 22;
      }
      if (cursor == 2) {
        menu = 1;
      }
      delay(300);
    }

    if (menu == 3) {             ///////////////////////////////////////////////// 3 Settings Main
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("-> Modify Program");
      lcd.setCursor(3, 1);
      lcd.print("Time per Photo");
      lcd.setCursor(3, 2);
      lcd.print("Min. Vert. Angle");
      lcd.setCursor(3, 3);
      lcd.print("Set Camera   BACK");
      cursor = 0;

      while (digitalRead(buttonOK) == LOW) {
        delay(10);
      }
      while (digitalRead(buttonOK) == HIGH) {
          lcd.setCursor(0, cursor);
          lcd.print("  ");
          delay(150);
          if (cursor<4){
            lcd.setCursor(0, cursor);
            lcd.print("->");
            delay(150);
            lcd.setCursor(0,cursor);
            lcd.print("  ");
          }
          if (cursor==4){
            lcd.setCursor(16,3);
            lcd.print("    ");
            delay(150);
            lcd.setCursor(16,3);
            lcd.print("BACK");
          }

        if (digitalRead(buttonDOWN) == LOW && cursor < 5) {
          cursor = cursor + 1;
        }
        if (digitalRead(buttonUP) == LOW && cursor > -1) {
          cursor = cursor - 1;
        }
        if(cursor==5){cursor=0;}
        if(cursor==-1){cursor=4;}
        
      }
      if (cursor == 0) {
        menu = 23;
      }
      if (cursor == 1) {
        menu = 31;
      }
      if (cursor == 2) {
        menu = 34;
      }
      if (cursor == 3) {
        menu =35;                      
      }
      if (cursor == 4) {
        menu = 1;
      }
    }

    if (menu == 21) {    /////////////////////////////////////////////////////// 2.1 Run Last Program
      val1 = EEPROM.read(100);
      val2 = EEPROM.read(101);
      val3 = EEPROM.read(102);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Last Program");
      lcd.setCursor(1, 1);
      lcd.print("(Photos/Angle/Pos.)");
      lcd.setCursor(1, 2);
      lcd.print("(");
      lcd.print(val1);
      lcd.print(" / ");
      lcd.print(val2);
      lcd.print(" / ");
      lcd.print(val3);
      lcd.print(")");
      lcd.setCursor(0, 3);
      lcd.print("START           BACK");
      cursor = 2;
      while (digitalRead(buttonOK) == LOW) {
        delay(10);
      }
      while (digitalRead(buttonOK) == HIGH) {
        while (cursor == 2 && digitalRead(buttonOK) == HIGH) {
          lcd.setCursor(16, 3);
          lcd.print("    ");
          delay(100);
          lcd.setCursor(16, 3);
          lcd.print("BACK");
          delay(200);
          if (digitalRead(buttonUP) == LOW) {
            cursor = 1;
          }
        }
        while (cursor == 1 && digitalRead(buttonOK) == HIGH) {
          lcd.setCursor(0, 3);
          lcd.print("     ");
          delay(100);
          lcd.setCursor(0, 3);
          lcd.print("START");
          delay(200);
          if (digitalRead(buttonDOWN) == LOW) {
            cursor = 2;
          }
        }
      }
      if (cursor == 2) {
        menu = 1;
      }
      if (cursor == 1) {
        menu = 0;
      }
    }

    if (menu == 22 || menu == 23) {          ///////////////////////////////////////////////// 2.2 Load Program OR 2.3. Mod Program
      cursor = 1;
      val = 1;
      while (val != 0) {
        if (val == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          if (menu == 22) {
            lcd.print("Load Program 1-3");
          }
          if (menu == 23) {
            lcd.print("Modify Program 1-3");
          }
          for(int i=1;i<4;i++){
              lcd.setCursor(3, i);
              lcd.print(EEPROM.read(3 * i + 5));
              lcd.print(" / ");
              lcd.print(EEPROM.read(3 * i + 6));
              lcd.print(" / ");
              lcd.print(EEPROM.read(3 * i + 7));
          }
          while (digitalRead(buttonOK) == LOW) {
            delay(10);
          }
          while (digitalRead(buttonOK) == HIGH && cursor != 4 && cursor != 0) {
            lcd.setCursor(0, cursor);
            lcd.print("->");
            if (digitalRead(buttonDOWN) == LOW && cursor < 4) {
              lcd.setCursor(0, cursor);
              lcd.print("  ");
              cursor = cursor + 1;
              lcd.setCursor(0, cursor);
              if (cursor != 4) {
                lcd.print("->");
              }
              if (cursor == 4) {
                val = 2;
              }
              delay(300);
            }
            if (digitalRead(buttonUP) == LOW && cursor > 0) {
              lcd.setCursor(0, cursor);
              lcd.print("  ");
              cursor = cursor - 1;
              lcd.setCursor(0, cursor);
              if (cursor != 0) {
                lcd.print("->");
              }
              if (cursor == 0) {
                val = 4;
              }
              delay(300);
            }
          }
          if (digitalRead(buttonOK) == LOW) {
            val = 0;
          }
          if (cursor == 0) {
            cursor = 12;
          }

        }

        if (val == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          if (menu == 22) {
            lcd.print("Load Program 4-6");
          }
          if (menu == 23) {
            lcd.print("Modify Program 4-6");
          }
          for(int i=4;i<7;i++){
              lcd.setCursor(3, i-3);
              lcd.print(EEPROM.read(3 * i + 5));
              lcd.print(" / ");
              lcd.print(EEPROM.read(3 * i + 6));
              lcd.print(" / ");
              lcd.print(EEPROM.read(3 * i + 7));
          }
          while (digitalRead(buttonOK) == HIGH && cursor != 3 && cursor != 7) {
            lcd.setCursor(0, cursor - 3);
            lcd.print("->");
            if (digitalRead(buttonDOWN) == LOW && cursor < 7) {
              lcd.setCursor(0, cursor - 3);
              lcd.print("  ");
              cursor = cursor + 1;
              lcd.setCursor(0, cursor - 3);
              if (cursor != 7) {
                lcd.print("->");
              }
              if (cursor == 7) {
                val = 3;
              }
              delay(300);
            }
            if (digitalRead(buttonUP) == LOW && cursor > 3) {
              lcd.setCursor(0, cursor - 3);
              lcd.print("  ");
              cursor = cursor - 1;
              lcd.setCursor(0, cursor - 3);
              if (cursor != 3) {
                lcd.print("->");
              }
              if (cursor == 3) {
                val = 1;
              }
              delay(300);
            }
          }
          if (digitalRead(buttonOK) == LOW) {
            val = 0;
          }
        }

        if (val == 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          if (menu == 22) {
            lcd.print("Load Program 7-9");
          }
          if (menu == 23) {
            lcd.print("Modify Program 7-9");
          }
          for(int i=7;i<10;i++){
              lcd.setCursor(3, i-6);
              lcd.print(EEPROM.read(3 * i + 5));
              lcd.print(" / ");
              lcd.print(EEPROM.read(3 * i + 6));
              lcd.print(" / ");
              lcd.print(EEPROM.read(3 * i + 7));
          }
          while (digitalRead(buttonOK) == HIGH && cursor != 6 && cursor != 10) {
            delay(100);
            lcd.setCursor(0, cursor - 6);
            lcd.print("->");
            if (digitalRead(buttonDOWN) == LOW && cursor < 10) {
              lcd.setCursor(0, cursor - 6);
              lcd.print("  ");
              cursor = cursor + 1;
              lcd.setCursor(0, cursor - 6);
              if (cursor != 10) {
                lcd.print("->");
              }
              if (cursor == 10) {
                val = 4;
              }
              delay(300);
            }
            if (digitalRead(buttonUP) == LOW && cursor > 6) {
              lcd.setCursor(0, cursor - 6);
              lcd.print("  ");
              cursor = cursor - 1;
              lcd.setCursor(0, cursor - 6);
              if (cursor != 6) {
                lcd.print("->");
              }
              if (cursor == 6) {
                val = 2;
              }
              delay(300);
            }
          }
          if (digitalRead(buttonOK) == LOW) {
            val = 0;
          }
        }

        if (val == 4) {
          lcd.clear();
          lcd.setCursor(0, 0);
          if (menu == 22) {
            lcd.print("Load Program 10-12");
          }
          if (menu == 23) {
            lcd.print("Modify Program 10-12");
          }
          for(int i=10;i<13;i++){
              lcd.setCursor(3, i-9);
              lcd.print(EEPROM.read(3 * i + 5));
              lcd.print(" / ");
              lcd.print(EEPROM.read(3 * i + 6));
              lcd.print(" / ");
              lcd.print(EEPROM.read(3 * i + 7));
          }
          while (digitalRead(buttonOK) == HIGH && cursor != 9 && cursor != 13) {
            delay(100);
            lcd.setCursor(0, cursor - 9);
            lcd.print("->");
            if (digitalRead(buttonDOWN) == LOW && cursor < 13) {
              lcd.setCursor(0, cursor - 9);
              lcd.print("  ");
              cursor = cursor + 1;
              lcd.setCursor(0, cursor - 9);
              if (cursor != 13) {
                lcd.print("->");
              }
              if (cursor == 13) {
                val = 1;
              }
              delay(300);
            }
            if (digitalRead(buttonUP) == LOW && cursor > 9) {
              lcd.setCursor(0, cursor - 9);
              lcd.print("  ");
              cursor = cursor - 1;
              lcd.setCursor(0, cursor - 9);
              if (cursor != 9) {
                lcd.print("->");
              }
              if (cursor == 9) {
                val = 3;
              }
              delay(300);
            }
          }
          if (cursor == 13) {
            cursor = 1;
          }
          if (digitalRead(buttonOK) == LOW) {
            val = 0;
          }
        }

      }


      if (menu == 22) {              //////////////////////////////////////////// Load Program
        val1 = EEPROM.read(3 * cursor + 5);
        val2 = EEPROM.read(3 * cursor + 6);
        val3 = EEPROM.read(3 * cursor + 7);
        val = 3 * cursor + 5;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Start Program?");
        lcd.setCursor(0, 1);
        lcd.print("-Photos: ");
        lcd.print(val1);
        lcd.setCursor(0, 2);
        lcd.print("-Angle:  ");
        lcd.print(val2);
        lcd.setCursor(0, 3);
        lcd.print("-Pos.:   ");
        lcd.print(val3);
        lcd.setCursor(17, 2); lcd.print("YES");
        lcd.setCursor(18, 3); lcd.print("NO");
        cursor = 2;
        while (digitalRead(buttonOK) == LOW) {
          delay(10);
        }
        while (digitalRead(buttonOK) == HIGH) {
          if (digitalRead(buttonUP) == LOW && cursor > 1) {
            cursor = cursor - 1;
          }
          if (digitalRead(buttonDOWN) == LOW && cursor < 2) {
            cursor = cursor + 1;
          }
          if (cursor == 1) {
            lcd.setCursor(17, 2);
            lcd.print("   ");
            delay(100);
            lcd.setCursor(17, 2);
            lcd.print("YES");
            delay(200);
          }
          if (cursor == 2) {
            lcd.setCursor(17, 3);
            lcd.print("   ");
            delay(100);
            lcd.setCursor(18, 3);
            lcd.print("NO");
            delay(200);
          }
        }
        if (cursor == 2) {
          menu = 1;
        }
        if (cursor == 1) {
          menu = 0;
          if (EEPROM.read(100) != EEPROM.read(val)) {
            EEPROM.write(100, EEPROM.read(val));
          }
          if (EEPROM.read(101) != EEPROM.read(val + 1)) {
            EEPROM.write(101, EEPROM.read(val + 1));
          }
          if (EEPROM.read(102) != EEPROM.read(val + 2)) {
            EEPROM.write(102, EEPROM.read(val + 2));
          }
        }
      }


      if (menu == 23) {                     //////////////////////////////////////// Modify
        val = 3 * cursor + 5;
        val1 = EEPROM.read(val);
        val2 = EEPROM.read(val + 1);
        val3 = EEPROM.read(val + 2);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Photos per Rotation:");
        lcd.setCursor(0, 1);
        lcd.print("current: ");
        lcd.print(val1);
        lcd.setCursor(0, 2);
        lcd.print("new:     ");
        while (digitalRead(buttonOK) == LOW) {
          delay(10);
        }
        while (digitalRead(buttonOK) == HIGH) {
          if (digitalRead(buttonDOWN) == LOW && val1 >= 8) {
            val1 = val1 / 2;
          }
          if (digitalRead(buttonUP) == LOW && val1 < 128) {
            val1 = val1 * 2;
          }
          lcd.setCursor(10, 2);
          lcd.print("   ");
          lcd.setCursor(9, 2);
          lcd.print(val1);
          delay(200);
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Start at Angle:");
        lcd.setCursor(0, 1);
        lcd.print("current: ");
        lcd.print(val2);
        lcd.setCursor(0, 2);
        lcd.print("new:     ");
        while (digitalRead(buttonOK) == LOW) {
          delay(10);
        }
        while (digitalRead(buttonOK) == HIGH) {
          if (digitalRead(buttonDOWN) == LOW && val2 > 0) {
            val2 = val2 - 1;
          }
          if (digitalRead(buttonUP) == LOW && val2 < 75) {
            val2 = val2 + 1;
          }
          lcd.setCursor(10, 2);
          lcd.print("   ");
          lcd.setCursor(9, 2);
          lcd.print(val2);
          delay(80);
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vert. Positions");
        lcd.setCursor(0, 1);
        lcd.print("current: ");
        lcd.print(val3);
        lcd.setCursor(0, 2);
        lcd.print("new:     ");
        while (digitalRead(buttonOK) == LOW) {
          delay(10);
        }
        while (digitalRead(buttonOK) == HIGH) {
          if (digitalRead(buttonDOWN) == LOW && val3 > 1) {
            val3 = val3 - 1;
          }
          if (digitalRead(buttonUP) == LOW && val3 < 32) {
            val3 = val3 + 1;
          }
          lcd.setCursor(10, 2);
          lcd.print("   ");
          lcd.setCursor(9, 2);
          lcd.print(val3);
          delay(200);
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Save new Program?");
        lcd.setCursor(0, 1);
        lcd.print("- Old: ");
        lcd.print(EEPROM.read(val));
        lcd.print("/");
        lcd.print(EEPROM.read(val + 1));
        lcd.print("/");
        lcd.print(EEPROM.read(val + 2));
        lcd.setCursor(0, 2);
        lcd.print("- New: ");
        lcd.print(val1);
        lcd.print("/");
        lcd.print(val2);
        lcd.print("/");
        lcd.print(val3);

        lcd.setCursor(0, 3); lcd.print("YES");
        lcd.setCursor(18, 3); lcd.print("NO");
        cursor = 2;
        while (digitalRead(buttonOK) == LOW) {
          delay(10);
        }
        while (digitalRead(buttonOK) == HIGH) {
          if (digitalRead(buttonUP) == LOW && cursor > 1) {
            cursor = cursor - 1;
          }
          if (digitalRead(buttonDOWN) == LOW && cursor < 2) {
            cursor = cursor + 1;
          }
          if (cursor == 1) {
            lcd.setCursor(0, 3);
            lcd.print("   ");
            delay(100);
            lcd.setCursor(0, 3);
            lcd.print("YES");
            delay(200);
          }
          if (cursor == 2) {
            lcd.setCursor(18, 3);
            lcd.print("  ");
            delay(100);
            lcd.setCursor(18, 3);
            lcd.print("NO");
            delay(200);
          }
        }
        if (cursor == 2) {
          menu = 1;
        }
        if (cursor == 1) {
          menu = 1;
          if (val1 != EEPROM.read(val)) {
            EEPROM.write(val, val1);
          }
          if (val2 != EEPROM.read(val + 1)) {
            EEPROM.write(val + 1, val2);
          }
          if (val2 != EEPROM.read(val + 2)) {
            EEPROM.write(val + 2, val3);
          }
        }
      }

    }



    if (menu == 31) {             ///////////////////////////////////////////////// 31 - Settings - Time per Foto
      val = EEPROM.read(4) * 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time per Photo");
      lcd.setCursor(1, 1);
      lcd.print("current:       ms");
      lcd.setCursor(10, 1);
      lcd.print(val);
      lcd.setCursor(1, 2);
      lcd.print("new:           ms");
      lcd.setCursor(10, 2);
      lcd.print(val);
      lcd.setCursor(0, 3);
      lcd.print("(Min: 0.1s Max: 20s)");
      while (digitalRead(buttonOK) == LOW) {
        delay(10);
      }
      while (digitalRead(buttonOK) == HIGH) {
        if (digitalRead(buttonUP) == LOW && val < 20000) {
          lcd.setCursor(10, 2);
          lcd.print("     ");
          lcd.setCursor(10, 2);
          val = val + 100;
          lcd.print(val);
          if (val < 3000) {
            delay(110);
          }
          delay(70);
        }
        if (digitalRead(buttonDOWN) == LOW && val > 100) {
          lcd.setCursor(10, 2);
          lcd.print("      ");
          lcd.setCursor(10, 2);
          val = val - 100;
          lcd.print(val);
          delay(70);
          if (val < 3000) {
            delay(110);
          }
        }
      }
      if (EEPROM.read(4) != val / 100) {
        EEPROM.write(4, val / 100);
      }
      menu = 1;
    }

    if (menu == 34) {             ///////////////////////////////////////////////// 34 - Settings - Min. Vert. Angle
      val = EEPROM.read(7);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Min. vertical angle");
      lcd.setCursor(1, 1);
      lcd.print("current:");
      lcd.setCursor(10, 1);
      lcd.print(val);
      lcd.setCursor(1, 2);
      lcd.print("new:");
      lcd.setCursor(10, 2);
      lcd.print(val);
      lcd.setCursor(0, 3);
      lcd.print("(Min: 2    Max: 30)");
      while (digitalRead(buttonOK) == LOW) {
        delay(10);
      }
      while (digitalRead(buttonOK) == HIGH) {
        if (digitalRead(buttonUP) == LOW && val < 30) {
          lcd.setCursor(10, 2);
          lcd.print("     ");
          lcd.setCursor(10, 2);
          val = val + 2;
          lcd.print(val);
          delay(100);
        }
        if (digitalRead(buttonDOWN) == LOW && val > 2) {
          lcd.setCursor(10, 2);
          lcd.print("      ");
          lcd.setCursor(10, 2);
          val = val - 2;
          lcd.print(val);
          delay(100);
        }
      }
      if (EEPROM.read(7) != val) {
        EEPROM.write(7, val);
      }
      menu = 1;
    }

    if (menu == 35) {             ///////////////////////////////////////////////// 35 - Choose Camera shutter mode
      val = EEPROM.read(44);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set camera mode");
      lcd.setCursor(0, 1);
      lcd.print("!SEE DOCUMENTATION!");
      lcd.setCursor(1, 2);
      lcd.print("current:");
      lcd.setCursor(1, 3);
      lcd.print("new:");
      lcd.setCursor(10, 2);
      if(val==1){lcd.print("direct");}
      if(val==2){lcd.print("Canon");}
      if(val==3){lcd.print("Minolta");}      
      if(val==4){lcd.print("Nikon");}
      if(val==5){lcd.print("Olympus");}
      if(val==6){lcd.print("Pentax");}
      if(val==7){lcd.print("Sony");}      
      while (digitalRead(buttonOK) == LOW) {
        delay(10);
      }
      while (digitalRead(buttonOK) == HIGH) {
        if (digitalRead(buttonUP) == LOW && val < 8) {val=val+1;        }
        if (digitalRead(buttonDOWN) == LOW && val > 0) {val=val-1;}
         while(digitalRead(buttonUP)==LOW || digitalRead(buttonDOWN)==LOW){delay(10);}
        if(val==0){val=7;}
        if(val==8){val=1;}
        lcd.setCursor(10,3);
        if(val==1){lcd.print("direct ");}
        if(val==2){lcd.print("Canon  ");}
        if(val==3){lcd.print("Minolta");}      
        if(val==4){lcd.print("Nikon  ");}
        if(val==5){lcd.print("Olympus");}
        if(val==6){lcd.print("Pentax ");}
        if(val==7){lcd.print("Sony   ");}  
        delay(10);

      }
      if (EEPROM.read(44) != val) {
        EEPROM.write(44, val);
      }
      menu = 1;
    }

    if (menu != 0) {
      menu = 1;
    }
  }
  Stepper2.setCurrentPosition(0);
  Stepper1.setCurrentPosition(0);

  if (menu == 0) {
    lcd.clear();                                                                 // STARTROUTINE (STARTPOSITIONEN //
    lcd.setCursor(0, 0);
    lcd.print("HOME - Rotor");
    lcd.setCursor(0, 1);
    lcd.print("by pressing UP/DOWN");
    lcd.setCursor(0, 3);
    lcd.print("continue with OK");
    while (digitalRead(buttonOK) == LOW) {
      delay(10);
    }

    Stepper2.setAcceleration(20000);
    while (digitalRead(buttonOK) == HIGH) {
      if (digitalRead(buttonDOWN) == LOW) {
        Stepper2.runToNewPosition(+24);
        Stepper2.setCurrentPosition(0);
      }
      if (digitalRead(buttonUP) == LOW) {
        Stepper2.runToNewPosition(-24);
        Stepper2.setCurrentPosition(0);
      }
    }
    Stepper2.setAcceleration(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HOME - Turntable");
    lcd.setCursor(0, 1);
    lcd.print("by pressing UP/DOWN");
    lcd.setCursor(0, 3);
    lcd.print("continue with OK");
    while (digitalRead(buttonOK) == LOW) {
      delay(10);
    }
    Stepper1.setAcceleration(20000);
    while (digitalRead(buttonOK) == HIGH) {
      if (digitalRead(buttonDOWN) == LOW) {
        Stepper1.runToNewPosition(+16);
        Stepper1.setCurrentPosition(0);
      }
      if (digitalRead(buttonUP) == LOW) {
        Stepper1.runToNewPosition(-16);
        Stepper1.setCurrentPosition(0);
      }
    }
    Stepper1.setAcceleration(400);

    FpR = EEPROM.read(100);
    Startangle = EEPROM.read(101);

    Positions = EEPROM.read(102);
    TpF=EEPROM.read(4)*100;
    minVert=EEPROM.read(7);
    val4=EEPROM.read(44);
    Startangle=(SpR2/360*Startangle);
    minVert=SpR2/360*minVert;
    val1=FpR*Positions+2*Startangle/minVert;
    if (Positions == 1) {
      val1 = FpR;
    }
    lcd.clear();
    while (digitalRead(buttonOK) == LOW) {
      delay(10);
    }
    while (digitalRead(buttonOK) == HIGH) {
      lcd.setCursor(7, 1); lcd.print("READY?"); delay(200);
      lcd.setCursor(7, 1); lcd.print("      "); delay(100);
    }
    lcd.clear();
    Stepper1.setCurrentPosition(0);
    Stepper2.setCurrentPosition(0);
    
    val2 = 0;
    Stepper2.runToNewPosition(-Startangle); // COUNT PHOTOS ///////////////////////////////////////////////////////////////
      for (int j = 0; j < Positions; j = j + 1) {
      if((j!=0) && (Positions!=1)){
        int i = 1;
        while(-Startangle+2*Startangle/(Positions-1)*(j-1)+i*minVert<=-Startangle+2*Startangle/(Positions-1)*j){
          totalphotos=totalphotos+1;
          i= i+1;
        }
        };

      for (int i = 0; i < FpR; i = i + 1) {
        totalphotos=totalphotos+1;

      }
    }
    val1=0;
      for (int j = 0; j < Positions && val1==0; j = j + 1) {
      if((j!=0) && (Positions!=1)){
        int i = 1;
        while(-Startangle+2*Startangle/(Positions-1)*(j-1)+i*minVert<=-Startangle+2*Startangle/(Positions-1)*j && val1==0){
          Stepper2.runToNewPosition(-Startangle+2*Startangle/(Positions-1)*(j-1)+i*minVert);
          AUSLOESEN();
          i= i+1;
        }
        if(val1==0){
          Stepper2.runToNewPosition(-Startangle+2*Startangle/(Positions-1)*j);
        }
        };

      for (int i = 0; i < FpR&& val1==0; i = i + 1) {
        Stepper1.runToNewPosition(SpR1/FpR*i);
        AUSLOESEN();

      }
      if(val1==0){
        Stepper1.runToNewPosition(SpR1);
      }
      Stepper1.setCurrentPosition(0);
    }

    Stepper1.setCurrentPosition(0);
    Stepper2.runToNewPosition(0);

    menu = 1;
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Scan done");
    lcd.setCursor(2,1);
    lcd.print("photos taken:");
    lcd.print(val2);
    totalphotos=0;
      while (digitalRead(buttonOK) == HIGH) {
        lcd.setCursor(6,3);
        lcd.print("Press OK");
        delay(200);
        lcd.setCursor(6,3);
        lcd.print("          ");
        delay(100);
      }
    }
  }
}
void AUSLOESEN () {
  #define address I2C
  LiquidCrystal_I2C  lcd(address, 2, 1, 0, 4, 5, 6, 7);
  lcd.begin (20, 4); 
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  
  lcd.clear();
  lcd.setCursor(2, 1);
  val2 = val2 + 1;
  lcd.print("Photo ");
  lcd.print(val2);
  Serial.println(val2);
  lcd.print(" of ");
  lcd.print(totalphotos);
  if(val4==1){
    digitalWrite(photopin1, HIGH);}
  //if(val4==2){Canon C(photopin1);
  //  C.shutterNow();}
  //if(val4==3){Minolta M(photopin1);
  //  M.shutterNow();}
  //if(val4==4){Nikon N(photopin1);
  //  N.shutterNow();}
  //if(val4==5){Olympus O(photopin1);
  // O.shutterNow();}
  //if(val4==6){Pentax P(photopin1);
  //  P.shutterNow();}
  //if(val4==7){Sony S(photopin1);
  //  S.shutterNow();}
  //delay(releasetime);
  digitalWrite(photopin1, LOW);
  delay(TpF);

  while (digitalRead(buttonOK) == LOW) {
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("STOP PROGRAM?");
    lcd.setCursor(2, 1);
    lcd.print("-> press UP");
    delay(200);
    if (digitalRead(buttonUP) == LOW) {
      lcd.clear();
      val1=1;
      lcd.setCursor(0,0);
      lcd.print("Program terminated");
      delay(2000);
      lcd.setCursor(0,0);
      lcd.print("resetting positions...");
      }
    }
  }
