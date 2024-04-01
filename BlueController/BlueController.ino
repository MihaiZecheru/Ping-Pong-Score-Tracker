#include <TM1637.h>

#define CLK 10
#define DIO 9
#define RED_BTN_PIN 7
#define WHITE_BTN_PIN 5

TM1637 score_display(CLK, DIO);

//#include "Communication.h"
//#include "Button.h"
//#include "ScoreDisplay.h"
#include <ping_pong_score_tracker_library.h>

Button blue_btn(BLUE_BTN_PIN);
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
  // The only transmissions that will be received is what the red player can send
  switch (ReceiveData())
  {
    case DataMessage::increment_red_score: IncrementRedScore(); break;
    case DataMessage::decrement_red_score: DecrementRedScore(); break;
    case DataMessage::toggle_score_mode:   ToggleScoreMode(&score_display); break;
    case DataMessage::start_new_game:      StartNewGame(&score_display); break;
  };

  // Show the updated scores
  DisplayScores(&score_display, blue_score, red_score);

  // Listen for button presses and send events if they one of the buttons is pressed
  blue_btn.CheckForPressOrHold(on_blue_btn_press, on_blue_btn_hold);
  white_btn.CheckForPressOrHold(on_white_btn_press, on_white_btn_hold);

  // If a player has won, blink the final score then start a new game
  HandlePossibleWin(&score_display);
}
