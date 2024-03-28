#include <TM1637.h>

TM1637 score_display(4, 5); // CLK - 4, DIO = 5

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE = 7, CSN = 8

#include "Radio.h"
#include "Button.h"
#include "ScoreDisplay.h"

Button blue_btn(3);
//Button white_btn(4);

/**
 * Add one to the blue player's score. For when the blue button is pressed once
 */
void send_increment_blue_score_message(RF24* _radio)
{
  blue_score++;
  radio_send(_radio, RadioMessage::increment_blue_score);
}

/**
 * Remove one from the blue player's score. For when the blue button is held for three seconds
 */
void send_decrement_blue_score_message(RF24* _radio)
{
  radio_send(_radio, RadioMessage::decrement_blue_score);
}

void setup()
{
  // Initialize the score_display and set brightness to constant from ScoreDisplay.h
  InitScoreDisplay(&score_display);
  
  radio.begin(); // TODO: make radio able to both transmit AND receive
  radio.setPayloadSize(sizeof(char));
  radio.openWritingPipe(RED_RADIO_HOME_ADDRESS);
  radio.openReadingPipe(1, BLUE_RADIO_HOME_ADDRESS);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); // the radio is by default in listening mode, switching only to send info for a quick second
}

void loop()
{
  // The only transmissions that will be received is what the red player can send
  switch (GetRadioTransmission(&radio))
  {
    case RadioMessage::invalid_or_none:     return;
    case RadioMessage::increment_red_score: red_score++;
    case RadioMessage::decrement_red_score: red_score--;
    case RadioMessage::toggle_score_mode:   ToggleScoreMode();
    case RadioMessage::start_new_game:      StartNewGame();
  }

  // Show the updated scores
  DisplayScores(&score_display, blue_score, red_score);

  // Listen for button presses and send events if they one of the buttons is pressed
  blue_btn.CheckForPressOrHold(send_increment_blue_score_message, send_decrement_blue_score_message, &radio);
//  white_btn.CheckForPressOrHold(send_start_new_game_message, send_toggle_score_mode_message, &radio);

  // If a player has won, show that and end the game
  if (HasWon(blue_score)) ShowWin(&score_display);
  if (HasWon(red_score))  ShowLoss(&score_display);
}
