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
LED serve_indicator(SERVE_INDICATOR_PIN);

//#include "Communication.h"
//#include "Button.h"
//#include "ScoreDisplay.h"
#include <ping_pong_score_tracker_library.h>

Button blue_btn(BLUE_BTN_PIN);
Button white_btn(WHITE_BTN_PIN);

/**
 * Have the blue player select who goes first.
 * He will press the blue button for himself,
 * and the white button for his opponent.
 * 
 * Flash the serve indicator light while the blue player chooses.
 */
Player ChooseFirstServer()
{
  uint16_t last_time = millis();
  while (true)
  {
    if (millis() - last_time > 250)
    {
      last_time = millis();
      serve_indicator.Toggle();
    }

    if (blue_btn.GetState() == ButtonState::ON) return Player::Blue;
    if (white_btn.GetState() == ButtonState::OFF) return Player::Red;
  }
}

Player starting_server;

void setup()
{
  // Intialize serial
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  serial_com.begin(9600);

  // Have the blue player select who goes first
  starting_server = ChooseFirstServer();

  // Let the red player know who was chosen
  if (starting_server == Player::Blue) TransmitData(DataMessage::blue_serves_first);
  else TransmitData(DataMessage::red_serves_first);

  // Initialize the score_display and set brightness to the constant from ping_pong_score_tracker_library/ScoreDisplay.h
  InitScoreDisplay(&score_display);

  // Show the players what they're playing to when they turn on their device
  ShowScoreMode(&score_display);
}

void loop()
{
  // The only transmissions that will be received is what the red player can send
  switch (ReceiveData())
  {
    case DataMessage::increment_red_score: IncrementRedScore(); break;
    case DataMessage::toggle_score_mode:   ToggleScoreMode(&score_display); break;
  };

  // Show the updated scores
  DisplayScores(&score_display, blue_score, red_score);

  // Listen for button presses and call the given func if one of the buttons is pressed
  blue_btn.CheckForPress(on_blue_btn_press);
  white_btn.CheckForPress(on_white_btn_press);

  // Check serve indicator
  if (GetCurrentServer() == Player::Blue)
       serve_indicator.Show();
  else serve_indicator.Hide();

  // If a player has won, blink the final score then start a new game
  HandlePossibleWin(&score_display);
}
