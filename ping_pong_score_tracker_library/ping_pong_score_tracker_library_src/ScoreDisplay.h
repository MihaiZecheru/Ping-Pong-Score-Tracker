#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H

#include <TM1637.h>

/**
 * The brightness of the score_display. Integer between 0 - 7
 */
#define SCORE_DISPLAY_BRIGHTNESS 5


/**
 * Byte for an empty spot in the score display. Turns all seven segments off
 */
#define EMPTY_DIGIT 0x7f

/**
   Blue and Red players' scores
*/
uint8_t blue_score = 0;
uint8_t red_score = 0;

void BlinkScores(TM1637* _score_display, uint8_t blinks = 6);

void IncrementBlueScore()
{
  blue_score++;
}

void DecrementBlueScore()
{
  if (blue_score != 0)
  {
    blue_score--;
    return;
  }
  else
  {
    BlinkScores(&score_display, 1);
  }
}

void IncrementRedScore()
{
  red_score++;
}

void DecrementRedScore()
{
  if (red_score != 0)
  {
    red_score--;
    return;
  }
  else
  {
    BlinkScores(&score_display, 1);
  }
}

/**
   Whether the ping pong game is to be played to 6, to 11, or to 21
*/
enum ScoreMode
{
  ToSix = 1
  ToEleven = 2,
  ToTwentyOne = 3
};

ScoreMode SCORE_MODE = ScoreMode::ToEleven; // Game to 11 is the default

void ShowScoreMode(TM1637* _score_display)
{
  _score_display->point(false); // turn semicolon off to completely clear display
  for (int i = 0; i < 6; i++)
  {
    
    uint16_t start = millis();
    while (millis() - start < 125) _score_display->clearDisplay();
    
    start = millis();
    while (millis() - start < 500)
    {
      // Show what the game is being played to in the middle of the score display
      if (SCORE_MODE == ScoreMode::ToSix)
	  {
		_score_display->display(1, 0);
		_score_display->display(1, 6);
	  }
	  else if (SCORE_MODE == ScoreMode::ToEleven)
      {
        // Show the number 11 in the middle of the score display
        _score_display->display(1, 1);
        _score_display->display(2, 1);
      }
      else if (SCORE_MODE == ScoreMode::ToTwentyOne)
      {
        // Show the number 21 in the middle of the score display
        _score_display->display(1, 2);
		_score_display->display(2, 1);
      }
    }
  }
  _score_display->point(true); // turn semicolon on again
}

/**
 * Start a new game. Reset the scores.
 */
void StartNewGame()
{
  blue_score = 0;
  red_score = 0;
}

/**
  * Toggle what score the game is played to, either to 6, to 11, or to 21.
  * 
  * The order of ScoreMode's is to 11, to 21, then to 6.
*/
void ToggleScoreMode(TM1637* _score_display)
{
  if (SCORE_MODE == ScoreMode::ToSix)
  {
	SCORE_MODE = ScoreMode::ToEleven; // Game will now be played to 11, a 5pt extention from the ToSix game
  }
  else if (SCORE_MODE == ScoreMode::ToEleven)
  {
    SCORE_MODE = ScoreMode::ToTwentyOne; // Game will now be played to 21, a 10pt extension from the ToEleven game
  }
  else if (SCORE_MODE == ScoreMode::ToTwentyOne && max(blue_score, red_score) < 6)
  {
    // Check if any of the players have surpassed 6pts before switching to an 11pt game
    SCORE_MODE = ScoreMode::ToSix; // Game will now be played to 6, a 15pt reduction from the ToTwentyOne game
  }

  ShowScoreMode(_score_display);
}

/**
   Initialize the given _score_display module and set it's brightness to the SCORE_DISPLAY_BRIGHTNESS constant
*/
void InitScoreDisplay(TM1637* _score_display)
{
  _score_display->init();
  _score_display->set(SCORE_DISPLAY_BRIGHTNESS);
}

/**
   Display both the blue and red players' scores on the given _score_display module

   Note: this function must be called in a loop
*/
void DisplayScores(TM1637* _score_display, uint8_t _blue_score, uint8_t _red_score)
{
  /**
     If the score is 6, floor(6 / 10) is 0 (the first digit, i.e. the tens digit),
     and (6 % 10) is 6 (the second digit, i.e. the ones digit), giving "06"
	 
	 However, if there is a leading zero, it won't be displayed.
	 So in this example, "6" will be displayed instead of "06"
  */
  
  // Show the "tens" digit, removing the leading zero if it exists
  if (floor(_blue_score / 10) != 0)
  {  
	_score_display->display(0, floor(_blue_score / 10));
  }
  else
  {
    _score_display->display(0, EMPTY_DIGIT);
  }
  
  // Show the "ones" digit
  _score_display->display(1, _blue_score % 10);

  // Turn on the colon separator
  _score_display->point(true);

  // Show the "tens" digit, removing the leading zero if it exists
  if (floor(_red_score / 10) != 0)
  {
	_score_display->display(2, floor(_red_score / 10));
  }
  else
  {
	_score_display->display(2, EMPTY_DIGIT);
  }

  // Show the "ones" digit
  _score_display->display(3, _red_score % 10);
}

/**
   Check if the given player_score is high enough to win the game
*/
bool HasWon(uint8_t player_score)
{
  // Check for special win conditions
  if (SCORE_MODE == ScoreMode::ToSix && max(blue_score, red_score) == 3 && min(blue_score, red_score) == 0) return true;
  else if (SCORE_MODE == ScoreMode::ToEleven && max(blue_score, red_score) == 6 && min(blue_score, red_score) == 0) return true;
  else if (SCORE_MODE == ScoreMode::ToTwentyOne && max(blue_score, red_score) == 9 && min(blue_score, red_score) == 0) return true;

  // Check for deuce
  // Regardless of the score or what the game is being played to, neither player can win unless they're beating the other by two points
  if (abs(blue_score - red_score) < 2) return false;

  // Use >= because deuce makes it so that the score can go over 6, over 11, or over 21
  if (SCORE_MODE == ScoreMode::ToSix && player_score >= 6) return true;
  else if (SCORE_MODE == ScoreMode::ToEleven && player_score >= 11) return true;
  else if (SCORE_MODE == ScoreMode::ToTwentyOne && player_score >= 21) return true;
  return false;
}

/**
   Blink the score <blinks> amount of times. Each blink is 625ms. Default blinks is 6, meaning 3.75 seconds

   @param blinks The amount of times to blink
*/
void BlinkScores(TM1637* _score_display, uint8_t blinks = 6)
{
  for (int i = 0; i < blinks; i++)
  {
    _score_display->point(false); // turn semicolon off to completely clear display
    uint16_t start = millis();
    while (millis() - start < 125) _score_display->clearDisplay();
    
    start = millis();
    _score_display->point(true); // turn semicolon on again
    while (millis() - start < 500) DisplayScores(_score_display, blue_score, red_score);
  }
}

/**
 * Check if a player has won the game. If the have, blink the score six times then start a new game
 */
void HandlePossibleWin(TM1637* _score_display)
{
  if (!HasWon(blue_score) && !HasWon(red_score)) return;

  // Player has won
  BlinkScores(_score_display, 6);
  StartNewGame();
}

#endif
