#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "ScoreDisplay.h"

/**
 * The various messages to be transmitted and received by the nanos.
 * Each data message corresponds to a character, which is what is actually transmitted.
 */
enum DataMessage
{
  increment_blue_score = 97, // a
  decrement_blue_score = 98, // b
  
  increment_red_score = 99, // c
  decrement_red_score = 100, // d

  toggle_score_mode = 101, // e
  start_new_game = 102, // f
  
  invalid_or_none = 0 // NUL
};

/**
 * Convert a character (a - f) into a member of the DataMessage enum
 */
DataMessage CharToDataMessage(char c)
{
  switch(c)
  {
    case 'a': return DataMessage::increment_blue_score;
    case 'b': return DataMessage::decrement_blue_score;
    case 'c': return DataMessage::increment_red_score;
    case 'd': return DataMessage::decrement_red_score;
    case 'e': return DataMessage::toggle_score_mode;
    case 'f': return DataMessage::start_new_game;
    default:  return DataMessage::invalid_or_none;
  }
}

/**
 * Convert a member of the DataMessage enum to it's character form (a - f)
 */
char DataMessageToChar(DataMessage msg)
{
  return (char)msg;
}

/**
 * Send a DataMessage to the other nano, which is just one char
 *
 * @param msg The message to send
 */
void TransmitData(DataMessage msg)
{
  Serial.write(DataMessageToChar(msg));
}

/**
 * Gets an incoming data transmission if there is one.
 * 
 * The nanos are by default in the listening state. they switch only to send information whenever a button is pressed
 */
DataMessage ReceiveData()
{
  if (!Serial.available()) return DataMessage::invalid_or_none;
  char transmission = Serial.read();
  return CharToDataMessage(transmission);
}

/**
 * Add one to the blue player's score. For when the blue button is pressed once
 */
void send_increment_blue_score_message()
{
  TransmitData(DataMessage::increment_blue_score);
}

/**
 * Remove one from the blue player's score. For when the blue button is held for two seconds
 */
void send_decrement_blue_score_message()
{
  TransmitData(DataMessage::decrement_blue_score);
}

/**
 * Perform the button action and send the message to the other device
 */
void on_blue_btn_press()
{
	send_increment_blue_score_message();
	IncrementBlueScore();
}

/**
 * Perform the button action and send the message to the other device
 */
void on_blue_btn_hold()
{
	send_decrement_blue_score_message();
	DecrementBlueScore();
	// When the user holds the button, after two seconds a point is deducted from blue but the score doesn't refresh until he lets go
	// because of this, the user won't know when to let go of the button until the score display refreshes, hence why the display is refreshed here
	DisplayScores(&score_display, blue_score, red_score); // TM1637 instance from main file
}

/**
 * Add one to the red player's score. For when the red button is pressed once
 */
void send_increment_red_score_message()
{
	TransmitData(DataMessage::increment_red_score);
}

/**
 * Remove one from the red player's score. For when the red button is held for two seconds
 */
void send_decrement_red_score_message()
{
	TransmitData(DataMessage::decrement_red_score);
}

/**
 * Perform the button action and send the message to the other device
 */
void on_red_btn_press()
{
	send_increment_red_score_message();
	IncrementRedScore();
}

/**
 * Perform the button action and send the message to the other device
 */
void on_red_btn_hold()
{
	send_decrement_red_score_message();
	DecrementRedScore();
	// When the user holds the button, after two seconds a point is deducted from red but the score doesn't refresh until he lets go
	// because of this, the user won't know when to let go of the button until the score display refreshes, hence why the display is refreshed here
	DisplayScores(&score_display, blue_score, red_score); // TM1637 instance from main file
}

/**
 * End the current game and start a new one. Resets the score. For when the white button is held for two seconds
 */
void send_start_new_game_message()
{
  TransmitData(DataMessage::start_new_game);
}

/**
 * Toggle whether the game is to 11 or to 21. For when the white button is pressed once
 */
void send_toggle_score_mode_message()
{
  TransmitData(DataMessage::toggle_score_mode);
}

/**
 * Perform the button action and send the message to the other device
 */
 void on_white_btn_press()
{
	send_toggle_score_mode_message();
	ToggleScoreMode(&score_display); // TM1637 instance from main file
}

/**
 * Perform the button action and send the message to the other device
 */
void on_white_btn_hold()
{
	send_start_new_game_message();
	StartNewGame(); // TM1637 instance from main file
	// When the user holds the button, after two seconds the scores will reset but the score doesn't refresh until he lets go
	// because of this, the user won't know when to let go of the button until the score display refreshes, hence why the display is refreshed here
	DisplayScores(&score_display, 0, 0); // TM1637 instance from main file
}

#endif