// Note: the blue controller is the primary controller, meaning it handles game logging

#include <TM1637.h>
#include <NeoSWSerial.h>

#define CLK 10
#define DIO 9
#define RX 4
#define TX 6
#define BLUE_BTN_PIN 8
#define WHITE_BTN_PIN 2
#define SERVE_INDICATOR_PIN 13

TM1637 score_display(CLK, DIO);
NeoSWSerial serial_com(RX, TX);

//#include "Communication.h"
//#include "Button.h"
//#include "ScoreDisplay.h"
#include <ping_pong_score_tracker_library.h>

Button blue_btn(BLUE_BTN_PIN);
Button white_btn(WHITE_BTN_PIN);
LED serve_indicator(LED_PIN);
GameLog* game_log;

Player GetFirstServer()
{
  uint64_t blinker_start = millis();
  while (true)
  {
    // Blink the LED every half-second
    if (millis() - blinker_start >= 500)
    {
      serve_indicator.Toggle();
      blinker_start = millis();
    }

    if (blue_btn.GetState() == ButtonState::ON) return Player::Blue;

    // The increment_red_score DataMessage will be sent if the red player presses the red button once
    if (ReceiveData() == DataMessage::starting_server_is_red) return Player::Red;
  }
}

void setup()
{
  // Intialize serial
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  serial_com.begin(9600);

  // Initialize the score_display and set brightness to constant from ping_pong_score_tracker_library/ScoreDisplay.h
  InitScoreDisplay(&score_display);
  
  // Setup the first server
  Player first_server = GetFirstServer();
  TransmitData(DataMessage::server_chosen);
  game_log = new GameLog(first_server);
  if (first_server == Player::Blue) serve_indicator.Show();
  else
  {
    serve_indicator.Hide();
    TransmitData(DataMessage::starting_server_is_red);
  }

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
    case DataMessage::start_new_game:      StartNewGame(); break;
  };

  // Show the updated scores
  DisplayScores(&score_display, blue_score, red_score);

  // Listen for button presses and send events if they one of the buttons is pressed
  blue_btn.CheckForPressOrHold(on_blue_btn_press, on_blue_btn_hold);
  white_btn.CheckForPressOrHold(on_white_btn_press, on_white_btn_hold);

  // If a player has won, blink the final score then start a new game
  HandlePossibleWin(&score_display);
}
