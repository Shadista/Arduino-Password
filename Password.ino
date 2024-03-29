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
    if (key != NO_KEY) {if (key == 'A') {
        backspace(inputPassword);
        keyPress();
      } else
    {
      keyPress();
      inputPassword += key;
      lcd.setCursor(inputPassword.length() - 1, 1);
      lcd.print('*');
    } 
    if (key == '*') {
      changePassMode = true;
      lcd.clear();
      lcd.print("Old Password:");
      changePassword();
      return;
    }
  }
  }

  if (inputPassword == password) {
    lcd.clear();
    lcd.print("Twinkle Twinkle!");
    lcd.setCursor(2, 1);
    lcd.print("Little Star!");
    correct = true;
    attempts = 0;
    ledCheck();
      if (correct) {
        digitalWrite(led[3], HIGH);
      }
    twinkle(); //Credits to Abhishek Ghosh, Neerajkry, and Rohan Goyal for the song source code
    //https://github.com/AbhishekGhosh/Arduino-Buzzer-Tone-Codes/tree/master
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
    if (key != NO_KEY) {if (key == 'A') {
        backspace(oldPassword);
        keyPress();
      } else
    {
      keyPress();
      oldPassword += key;
      lcd.setCursor(oldPassword.length() - 1, 1);
      lcd.print('*');
    }
  }
}
    if (oldPassword == password) {
    lcd.clear();
  String newPassword;
  char key;
  lcd.print("New Password:");
  while ((key = keypad.getKey()) != '#'){
    if (key != NO_KEY) {if (key == 'A') {
        backspace(newPassword);
        keyPress();
      } else
    {
      keyPress();
      newPassword += key;
      lcd.setCursor(newPassword.length() - 1, 1);
      lcd.print('*');
    }
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
    if (key != NO_KEY) {if (key == 'A') {
        backspace(newPassword);
        keyPress();
      } else
    {
      keyPress();
      newPassword += key;
      lcd.setCursor(newPassword.length() - 1, 1);
      lcd.print('*');
    }
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
void backspace(String &str) {
  if (str.length() > 0) {
    str.remove(str.length() - 1);
    lcd.setCursor(str.length(), 1);
    lcd.print(" ");
  }
}

//Credits to Abhishek Ghosh, Neerajkry, and Rohan Goyal for the song source code

int length = 15; // the number of notes
//twinkle twinkle little star
char notes[] = "ccggaag ffeeddc ggffeed ggffeed ccggaag ffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzer, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void twinkle() {
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    // pause between notes
    delay(tempo / 2); 
  }
}