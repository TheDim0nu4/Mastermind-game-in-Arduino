#include "mastermind.h"
#include <Arduino.h>
#include <Wire.h>
#include <lcd_wrapper.h>

#define RED1 7
#define BLUE1 6
#define RED2 9
#define BLUE2 8
#define RED3 13
#define BLUE3 12
#define RED4 11
#define BLUE4 10

#define BUTTON1 A0
#define BUTTON2 2
#define BUTTON3 3
#define BUTTON4 4
#define ENTER 5

LiquidCrystal_I2C lcd(0x27, 16, 2);

char *generate_code(bool repeat, int length) {
    if (length < 1 || (!repeat && length > 10)) {
        return NULL;
    }

    char *code = new char[length + 1];
    code[length] = '\0';

    char numbers[] = "0123456789";

    for (int i = 0; i < length; i++) {
        int idx = random(10);
        if (!repeat) {
            while (true) {
                idx = random(10);
                int a;
                for (a = 0; a < length; a++) {
                    if (code[a] == numbers[idx]) {
                        break;
                    }
                }
                if (a == length) {
                    code[i] = numbers[idx];
                    break;
                }
            }
        } else {
            code[i] = numbers[idx];
        }
    }

    return code;
}

void get_score(char *secret, char *guess, int *peg_a, int *peg_b) {
    *peg_a = 0;
    *peg_b = 0;

    for (int i = 0; i < strlen(secret); i++) {
        int red = 0;
        int blue = 0;
        for (int idx = 0; idx < strlen(guess); idx++) {
            if (i == idx && secret[i] == guess[idx]) {
                red++;
                continue;
            }
            if (secret[i] == guess[idx]) {
                blue++;
            }
        }
        if (red != 0) {
            *peg_a = *peg_a + 1;
        } else if (blue != 0) {
            *peg_b = *peg_b + 1;
        }
    }
}

void turn_off_leds() {
    digitalWrite(RED1, LOW);
    digitalWrite(RED2, LOW);
    digitalWrite(RED3, LOW);
    digitalWrite(RED4, LOW);
    digitalWrite(BLUE1, LOW);
    digitalWrite(BLUE2, LOW);
    digitalWrite(BLUE3, LOW);
    digitalWrite(BLUE4, LOW);
}

void render_leds(const int peg_a, const int peg_b) {
  if( peg_a == 1 ){
    digitalWrite(RED1, HIGH);
  }
  else if( peg_a == 2 ){
    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, HIGH);
  }
  else if( peg_a == 3 ){
    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, HIGH);
    digitalWrite(RED3, HIGH);
  }
  else if( peg_a == 4 ){
    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, HIGH);
    digitalWrite(RED3, HIGH);
    digitalWrite(RED4, HIGH);
  }

 
  if( peg_b == 1 ){
    digitalWrite(BLUE1, HIGH);
  }
  else if( peg_b == 2 ){
    digitalWrite(BLUE1, HIGH);
    digitalWrite(BLUE2, HIGH);
  }
  else if( peg_b == 3 ){
    digitalWrite(BLUE1, HIGH);
    digitalWrite(BLUE2, HIGH);
    digitalWrite(BLUE3, HIGH);
  }
  else if( peg_b == 4 ){
    digitalWrite(BLUE1, HIGH);
    digitalWrite(BLUE2, HIGH);
    digitalWrite(BLUE3, HIGH);
    digitalWrite(BLUE4, HIGH);
  }
}

void render_history(char *secret, char **history, const int entry) {
  lcd.setCursor(0, 0);
  lcd.print("             ");
  lcd.setCursor(0, 0);
  if( entry < 9 ){
    lcd.print("0");
  }
  lcd.print(entry+1);
  lcd.print(": ");
  lcd.setCursor(4, 0);
  lcd.print( history[entry] );


  lcd.setCursor(8, 0);
  lcd.print("       ");
  lcd.setCursor(9, 0);

  int red;
  int blue;
  get_score(secret, history[entry], &red, &blue);
  lcd.print(red);
  lcd.print("A");
  lcd.print(blue);
  lcd.setCursor(12, 0);
  lcd.print("   "); 
  lcd.setCursor(12, 0);
  lcd.print("B   ");        

}

void play_game(char *secret) {

  lcd.setCursor(6, 0);
  lcd.print("Game");
  lcd.setCursor(3, 1);
  lcd.print("Mastermind");
  delay(2000);
  lcd.clear();

  char* history[10];
  
 for( int move = 1; move < 11; move++ ){
  char my_numbers[] = "0000";
  while( digitalRead(ENTER) != HIGH ){
    if( digitalRead(BUTTON1) == HIGH ){
      
      int mv_hist = move-1;
      int steps = 0;
      while( digitalRead(BUTTON1) == HIGH ){
        if( digitalRead(BUTTON2) == HIGH && mv_hist > 1){
          mv_hist--;
          render_history( secret, history, mv_hist-1 );

          int red, blue;
          get_score( secret, history[mv_hist-1], &red, &blue );
          render_leds( red, blue );
          delay(1000);
          turn_off_leds();

          steps++;
          while (digitalRead(BUTTON2) == HIGH) {
            delay(10);
          }
        }
      
        if( digitalRead(BUTTON3) == HIGH && mv_hist < move-1){
          mv_hist++;
          render_history( secret, history, mv_hist-1 );

          int red, blue;
          get_score( secret, history[mv_hist-1], &red, &blue );
          render_leds( red, blue );
          delay(1000);
          turn_off_leds();

          steps++;
          while (digitalRead(BUTTON3) == HIGH) {
            delay(10);
          }
        }

      }
      if( steps != 0 ){
        continue;
      }
      
      if( my_numbers[0] == '9' ){
        my_numbers[0] = '0';
      }
      else{
       my_numbers[0] = my_numbers[0] + 1;
      }
      while (digitalRead(BUTTON1) == HIGH) {
       delay(10);
      } 
    }


    if( digitalRead(BUTTON2) == HIGH ){
      if( my_numbers[1] == '9' ){
        my_numbers[1] = '0';
      }
      else{
       my_numbers[1] = my_numbers[1] + 1;
      }
      while (digitalRead(BUTTON2) == HIGH) {
       delay(10);
      } 
    }


    if( digitalRead(BUTTON3) == HIGH ){
      if( my_numbers[2] == '9' ){
        my_numbers[2] = '0';
      }
      else{
       my_numbers[2] = my_numbers[2] + 1;
      }
      while (digitalRead(BUTTON3) == HIGH) {
       delay(10);
      } 
    }


    if( digitalRead(BUTTON4) == HIGH ){
      if( my_numbers[3] == '9' ){
        my_numbers[3] = '0';
      }
      else{
       my_numbers[3] = my_numbers[3] + 1;
      }
      while (digitalRead(BUTTON4) == HIGH) {
       delay(10);
      } 
    }
    

    lcd.setCursor(0, 1);
    lcd.print("Your num:");
    lcd.setCursor( 10, 1);
    for( int idx = 0; idx < 4; idx++ ){
      lcd.print(my_numbers[idx]);
    }
  }

  strcpy(history[move - 1], my_numbers);
  char *step = my_numbers;
  int red, blue;
  get_score( secret, step, &red, &blue );
  render_leds( red, blue );
  delay(1000);
  turn_off_leds();
  render_history( secret, history, move-1);

  
  if( red == 4 ){
    lcd.setCursor( 0, 1);
    lcd.print("   You win!!! ");
    for( int a = 0; a < 5; a++ ){
      render_leds( red, blue );
      delay(500);
      turn_off_leds();
      delay(500);
    }
    return;
  }

 }
  
  lcd.setCursor(0, 1);
  lcd.print("Loos.Secret=");
  lcd.print(secret);
  delay(5000);
  lcd.clear();
     
}

void set() {
  pinMode(RED1, OUTPUT);
  pinMode(BLUE1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(BLUE2, OUTPUT);
  pinMode(RED3, OUTPUT);
  pinMode(BLUE3, OUTPUT);
  pinMode(RED4, OUTPUT);
  pinMode(BLUE4, OUTPUT);

  lcd.init();
  lcd.backlight();
}


