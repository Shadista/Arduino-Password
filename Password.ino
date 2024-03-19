#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {6, 7, 8, 9};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


String password = "";
String newPassword = "";
int attempts = 0;
bool changePassMode = false;
bool initialPassMode = true;

int led[] = {A2, A1, A0, 11};
const int buzzer = 10;
bool correct = false;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i <= 4; i++) {
    pinMode(led[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  while (initialPassMode) {
    lcd.print("Set Password:");
    setPassword();
  }
}

void loop() {
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    if (key == '#') {
      lcd.clear();
      lcd.print("Enter Password:");
      checkPassword();
      buzzerCheck();
    }
  }
}



void checkPassword() {
  String inputPassword;
  char key;

  while ((key = keypad.getKey()) != '#'){
    if (key != NO_KEY) {
      keyPress();
      inputPassword += key;
      lcd.setCursor(inputPassword.length() - 1, 1);
      lcd.print(key);
    } 
    if (key == '*') {
      changePassMode = true;
      lcd.clear();
      lcd.print("Old Password:");
      changePassword();
      return;
    }
  }

  if (inputPassword == password) {
    lcd.clear();
    lcd.print("Allowed");
    correct = true;
    attempts = 0;
    ledCheck();
      if (correct) {
        digitalWrite(led[3], HIGH);
      }
    tone(buzzer, 1000, 300);
      delay(300);
  } else {
    lcd.clear();
    lcd.print("Denied");
    correct = false;
    attempts++;
    ledCheck();
      if (correct) {
        digitalWrite(led[3], HIGH);
      }
    tone(buzzer, 500, 300);
    delay(300);
  }
}

void changePassword() {
  String oldPassword;
  char key;

  while ((key = keypad.getKey()) != '#'){
    if (key != NO_KEY) {
      keyPress();
      oldPassword += key;
      lcd.setCursor(oldPassword.length() - 1, 1);
      lcd.print(key);
    }
  }
    if (oldPassword == password) {
    lcd.clear();
  String newPassword;
  char key;
  lcd.print("New Password:");
  while ((key = keypad.getKey()) != '#'){
    if (key != NO_KEY) {
      keyPress();
      newPassword += key;
      lcd.setCursor(newPassword.length() - 1, 1);
      lcd.print(key);
    }
  }
  password = newPassword;
  lcd.clear();
  lcd.print("Password Changed!");
  attempts = 0;
  ledCheck();
} else {
  lcd.clear();
    lcd.print("Denied");
    correct = false;
    attempts++;
    ledCheck();
      if (correct) {
        digitalWrite(led[3], HIGH);
      }
    tone(buzzer, 500, 300);
    delay(300);
  }
}


void setPassword() {
  char key;
  while ((key = keypad.getKey()) != '#'){
    if (key != NO_KEY) {
      keyPress();
      newPassword += key;
      lcd.setCursor(newPassword.length() - 1, 1);
      lcd.print(key);
    }
  }
  password = newPassword;
  newPassword = "";
  initialPassMode = false;
  lcd.clear();
  lcd.print("Password Done!");
}

void ledCheck() {
  if (attempts == 0) {
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], LOW);
    digitalWrite(led[3], LOW);
  } else if (attempts == 1) {
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], LOW);
    digitalWrite(led[3], LOW);
  } else if (attempts == 2) {
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], HIGH);
    digitalWrite(led[2], LOW);
    digitalWrite(led[3], LOW);
  } else if (attempts == 3) {
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], HIGH);
    digitalWrite(led[2], HIGH);
    digitalWrite(led[3], LOW);
  }
}

void buzzerCheck() {
  if (attempts >= 3) {
    for(int x = 0; x <= 20; x++){
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);}
  } else {
    digitalWrite(buzzer, LOW);
  }
}

void keyPress() {
  tone(buzzer, 1000, 50);
}
