#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H

#include <TM1637.h>

/**
 * The brightness of the score_display. Integer between 0 - 7
 */
const uint8_t SCORE_DISPLAY_BRIGHTNESS = 5;

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

/**
 * Initialize the given _score_display module and set it's brightness to the SCORE_DISPLAY_BRIGHTNESS constant
 */
void InitScoreDisplay(TM1637* _score_display)
{
  _score_display->init();
  _score_display->set(SCORE_DISPLAY_BRIGHTNESS);
}

#endif
