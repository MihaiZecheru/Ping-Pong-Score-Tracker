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

uint8_t blue_score = 0;
uint8_t red_score = 0;

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
  radio.openWritingPipe(RADIO_ADDRESS);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop()
{
  DisplayScores(&score_display, blue_score, red_score);
  blue_btn.CheckForPressOrHold(send_increment_blue_score_message, send_decrement_blue_score_message, &radio);
//  white_btn.CheckForPressOrHold(send_start_new_game_message, send_toggle_score_mode_message, &radio);
}
