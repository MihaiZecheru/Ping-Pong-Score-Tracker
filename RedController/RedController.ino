#include <TM1637.h>
#include <NeoSWSerial.h>

#define CLK 10
#define DIO 9
#define RX 2
#define TX 13
#define RED_BTN_PIN 7
#define WHITE_BTN_PIN 5
#define LED_PIN 6

TM1637 score_display(CLK, DIO);
NeoSWSerial serial_com(RX, TX);

//#include "Communication.h"
//#include "Button.h"
//#include "ScoreDisplay.h"
#include <ping_pong_score_tracker_library.h>

Button red_btn(RED_BTN_PIN);
Button white_btn(WHITE_BTN_PIN);

void setup()
{
  // Intialize serial
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  serial_com.begin(9600);
  
  // Initialize the score_display and set brightness to constant from ping_pong_score_tracker_library/ScoreDisplay.h
  InitScoreDisplay(&score_display);

  // Show the players what they're playing to when they turn on their device
  ShowScoreMode(&score_display);
}

void loop()
{
  // The only transmissions that will be received is what the blue player can send
  switch (ReceiveData())
  {
    case DataMessage::increment_blue_score: IncrementBlueScore(); break;
    case DataMessage::toggle_score_mode:    ToggleScoreMode(&score_display); break;
  };

  // Show the updated scores
  DisplayScores(&score_display, blue_score, red_score);

  // Listen for button presses and call the given func if one of the buttons is pressed
  red_btn.CheckForPress(on_red_btn_press);
  white_btn.CheckForPress(on_white_btn_press);

  // If a player has won, blink the final score then start a new game
  HandlePossibleWin(&score_display);
}
