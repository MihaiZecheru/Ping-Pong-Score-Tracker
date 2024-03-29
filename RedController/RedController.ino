#include <TM1637.h>

TM1637 score_display(4, 5); // CLK - 4, DIO = 5

//#include "Communication.h"
//#include "Button.h"
//#include "ScoreDisplay.h"
#include <ping_pong_score_tracker_library.h>

#define RED_BTN_PIN 3
#define WHITE_BTN_PIN 2

Button red_btn(RED_BTN_PIN);
Button white_btn(WHITE_BTN_PIN);

void setup()
{
  // Initialize the score_display and set brightness to constant from ping_pong_score_tracker_library/ScoreDisplay.h
  InitScoreDisplay(&score_display);
  Serial.begin(9600);

  // Show the players what they're playing to when they turn on their device
  ShowScoreMode(&score_display);
}

void loop()
{
  // The only transmissions that will be received is what the blue player can send
  switch (ReceiveData())
  {
    case DataMessage::increment_blue_score: IncrementBlueScore(); break;
    case DataMessage::decrement_blue_score: DecrementBlueScore(); break;
    case DataMessage::toggle_score_mode:   ToggleScoreMode(&score_display); break;
    case DataMessage::start_new_game:      StartNewGame(&score_display); break;
  };

  // Show the updated scores
  DisplayScores(&score_display, blue_score, red_score);

  // Listen for button presses and send events if they one of the buttons is pressed
  red_btn.CheckForPressOrHold(on_red_btn_press, on_red_btn_hold);
  white_btn.CheckForPressOrHold(on_white_btn_press, on_white_btn_hold);

  // If a player has won, blink the final score then start a new game
  HandlePossibleWin(&score_display);
}
