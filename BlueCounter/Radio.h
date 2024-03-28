#ifndef SHARED_H
#define SHARED_H

/**
 * The address the blue red will be listening on
 */
const byte BLUE_RADIO_HOME_ADDRESS[6] = "00001";

/**
 * The address the red radio will be listening on
 */
const byte RED_RADIO_HOME_ADDRESS[6] = "00002";

/**
 * The various messages to be transmitted and received by radio.
 * Each radio message corresponds to a character, which is what is actually transmitted.
 */
enum RadioMessage
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
 * Convert a character (a - f) into a member of the RadioMessage enum
 */
RadioMessage GetRadioMessageFromChar(char c)
{
  switch(c)
    {
        case 'a': return RadioMessage::increment_blue_score;
        case 'b': return RadioMessage::increment_red_score;
        case 'c': return RadioMessage::decrement_blue_score;
        case 'd': return RadioMessage::decrement_red_score;
        case 'e': return RadioMessage::toggle_score_mode;
        case 'f': return RadioMessage::start_new_game;
        default:  return RadioMessage::invalid_or_none;
    }
}

/**
 * Convert a member of the RadioMessage enum to it's character form (a - f)
 */
RadioMessage RadioMessageToChar(RadioMessage rm)
{
  return (char)rm;
}

/**
 * Send a RadioMessage over the radio, which is just one char
 *
 * @param msg_char The message to send
 */
void radio_send(RF24* _radio, RadioMessage msg)
{
  _radio->stopListening();
  _radio->write(&RadioMessageToChar(msg), sizeof(char));
}

/**
 * End the current game and start a new one. Resets the score. For when the white button is pressed once
 */
void send_start_new_game_message(RF24* _radio)
{
  radio_send(_radio, RadioMessage::start_new_game);
}

/**
 * Toggle whether the game is to 11 or to 21. For when the white button is held for three seconds
 */
void send_toggle_score_mode_message(RF24* _radio)
{
  radio_send(_radio, RadioMessage::toggle_score_mode);
}

#endif
