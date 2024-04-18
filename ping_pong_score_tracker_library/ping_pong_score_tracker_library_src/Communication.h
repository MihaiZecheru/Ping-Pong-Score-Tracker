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
  increment_red_score = 98, // b

  toggle_score_mode = 99, // c
  
  red_serves_first = 100, // d
  blue_serves_first = 101, // e
  
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
    case 'b': return DataMessage::increment_red_score;
    case 'c': return DataMessage::toggle_score_mode;
	case 'd': return DataMessage::red_serves_first;
	case 'e': return DataMessage::blue_serves_first;
    default:  return DataMessage::invalid_or_none;
  }
}

/**
 * Convert a member of the DataMessage enum to it's character form (a - c)
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
  serial_com.write(DataMessageToChar(msg)); // serial_com instance from main controller file
}

/**
 * Gets an incoming data transmission if there is one.
 * 
 * The nanos are by default in the listening state. they switch only to send information whenever a button is pressed
 */
DataMessage ReceiveData()
{
  if (!serial_com.available()) return DataMessage::invalid_or_none; // serial_com instance from main controller file
  char transmission = serial_com.read(); // serial_com instance from main controller file
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
 * Perform the button action and send the message to the other device
 */
void on_blue_btn_press()
{
	send_increment_blue_score_message();
	IncrementBlueScore();
}

/**
 * Add one to the red player's score. For when the red button is pressed once
 */
void send_increment_red_score_message()
{
	TransmitData(DataMessage::increment_red_score);
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

#endif