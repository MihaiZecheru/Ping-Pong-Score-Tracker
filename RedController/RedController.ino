#include <TM1637.h>
#include <NeoSWSerial.h>

#define CLK 10
#define DIO 9
#define RX 2
#define TX 13
#define RED_BTN_PIN 7
#define WHITE_BTN_PIN 5
#define SERVE_INDICATOR_PIN 6

TM1637 score_display(CLK, DIO);
NeoSWSerial serial_com(RX, TX);

//#include "Communication.h"
//#include "Button.h"
//#include "ScoreDisplay.h"
#include <ping_pong_score_tracker_library.h>

Button red_btn(RED_BTN_PIN);
Button white_btn(WHITE_BTN_PIN);
ServeIndicator serve_indicator(SERVE_INDICATOR_PIN);

void setup()
{
  // Intialize serial
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  serial_com.begin(9600);

  // Initialize the score_display and set brightness to constant from ping_pong_score_tracker_library/ScoreDisplay.h
  InitScoreDisplay(&score_display);

  // Wait for server to be chosen, blinking the serve_indicator every half-second while the arduino waits
  uint64_t start = millis();
  while (ReceiveData() != DataMessage::server_chosen)
  {
    if (millis() - start >= 500)
    {
      serve_indicator.Toggle();
      start = millis();
    }

    // The program will quit out of the while loop once the blue controller responds with server_chosen
    if (red_btn.GetState() == ButtonState::ON) TransmitData(DataMessage::starting_server_is_red);
  }

  // Check if red is the starting server
  if (ReceiveData() == DataMessage::starting_server_is_red) serve_indicator.Show();
  else serve_indicator.Hide();

  // Show the players what they're playing to when they turn on their device
  ShowScoreMode(&score_display);
}

void loop()
{
  // The only transmissions that will be received is what the blue player can send
  switch (ReceiveData())
  {
    case DataMessage::increment_blue_score:   IncrementBlueScore(); break;
    case DataMessage::decrement_blue_score:   DecrementBlueScore(); break;
    case DataMessage::toggle_score_mode:      ToggleScoreMode(&score_display); break;
    case DataMessage::start_new_game:         StartNewGame(); break;
    case DataMessage::hide_serve_indicator:   serve_indicator.Hide(); break;
	case DataMessage::show_serve_indicator:   serve_indicator.Show(); break;
  };

  // Show the updated scores
  DisplayScores(&score_display, blue_score, red_score);

  // Listen for button presses and send events if they one of the buttons is pressed
  red_btn.CheckForPressOrHold(on_red_btn_press, on_red_btn_hold);
  white_btn.CheckForPressOrHold(on_white_btn_press, on_white_btn_hold);

  // If a player has won, blink the final score then start a new game
  HandlePossibleWin(&score_display);
}
