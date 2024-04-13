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

public:
  Button(const uint8_t pin) : pin(pin)
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
   * Check if the button is pressed, and then call the given on_press function
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
	
	// Wait for the player to let go of the button, otherwise the on_press function would get called immediately after
	while (GetState() == ButtonState::ON) {};

    // The button has been pressed but was not held for two seconds
    on_press();
  }
};

#endif
