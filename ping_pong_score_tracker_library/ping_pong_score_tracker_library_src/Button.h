// HEADER => Button Class

#ifndef BUTTON_H
#define BUTTON_H

enum ButtonState {
  ON = 1,
  OFF = 2
};

class Button
{
private:
  const uint8_t pin;
  const uint16_t last_press_time;

public:
  Button(const uint8_t pin) : pin(pin), last_press_time(millis())
  {
    pinMode(pin, INPUT);
  }

  /**
   * Is the button on or off? (pressed or idle)
   */
  ButtonState GetState()
  {
    return (digitalRead(pin) == HIGH) ? ButtonState::ON : ButtonState::OFF;
  }

  /**
   * Check if the button is pressed, and then call the given on_press function. Includes 500ms debounce time.
   * 
   * Note:
   * 	- Call this function repeatedly in a forever loop, such as a while (true) loop, as the function returns after the button has been pressed.
   * 	- This function only acts as an event listener for ONE event.
   * 
   * @param on_press Function to call when a single press occurs
   */
  void CheckForPress(void (*on_press)(void))
  {
    // Wait for a press
    if (GetState() != ButtonState::ON) return;

    // Wait for the button to stop being pressed by waiting for GetState() to equal OFF
    // This means that the on_press func will be called only once the user releases the button
	
	// Wait for the player to let go of the button
	while (GetState() == ButtonState::ON) {};

    // Button debouncing: make sure there's at least a 500ms time difference between the presses
	if (millis() - last_press_time > 500)
	{
		last_press_time = millis();
		on_press();
	}
  }
};

#endif
