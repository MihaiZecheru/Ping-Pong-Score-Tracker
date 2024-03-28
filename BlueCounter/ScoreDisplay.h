#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H

#include <TM1637.h>

/**
 * Blue and Red players' scores
 */
uint8_t blue_score = 0;
uint8_t red_score = 0;

void StartNewGame()
{
  blue_score = 0;
  red_score = 0;
  // Display will be automatically refreshed in loop by DisplayScores func
}

/**
 * Whether the ping pong game is to be played to 11 or to 21
 */
enum ScoreMode
{
  ToEleven = 1,
  ToTwentyOne = 2
}

ScoreMode SCORE_MODE = ScoreMode::ToEleven; // Game to 11 is the default

/**
 * Toggle what score the game is played to, either to 11 or to 21
 */
void ToggleScoreMode()
{
  if (SCORE_MODE == ScoreMode::ToEleven)
    SCORE_MODE = ScoreMode::ToTwentyOne;
  else
    SCORE_MODE = ScoreMode::ToEleven;
}

/**
 * The brightness of the score_display. Integer between 0 - 7
 */
const uint8_t SCORE_DISPLAY_BRIGHTNESS = 5;

/**
 * Initialize the given _score_display module and set it's brightness to the SCORE_DISPLAY_BRIGHTNESS constant
 */
void InitScoreDisplay(TM1637* _score_display)
{
  _score_display->init();
  _score_display->set(SCORE_DISPLAY_BRIGHTNESS);
}

/**
 * Display both the blue and red players' scores on the given _score_display module
 * 
 * Note: this function must be called in a loop
 */
void DisplayScores(TM1637* _score_display, uint8_t score_blue, uint8_t score_red)
{
  /**
   * If the score is 6, floor(6 / 10) is 0 (the first digit, i.e. the tens digit),
   * and (6 % 10) is 6 (the second digit, i.e. the ones digit), giving "06"
   */
  _score_display->display(0, floor(score_blue / 10)); // Get the "ones" digit
  _score_display->display(1, score_blue % 10); // Get the "tens" digit
  
  _score_display->point(true); // Turn on the colon separator
  
  _score_display->display(2, floor(score_red / 10)); // Get the "ones" digit
  _score_display->display(3, score_red % 10); // Get the "tens" digit
}

#endif
