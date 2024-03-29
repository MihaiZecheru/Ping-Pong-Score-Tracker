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
   * Is the button on or off?
   */
  ButtonState GetState()
  {
    return (digitalRead(pin) == HIGH) ? ButtonState::ON : ButtonState::OFF;
  }

  /**
   * Check if the button is pressed or if the button was held for two seconds, and then call the given on_press function or the given on_hold function respectively
   * Note:
   * 	- Call this function repeatedly in a forever loop, such as a while (true) loop, as the function returns after the button has been pressed or held.
   * 	- This function only acts as an event listener for ONE event.
   * 
   * @param on_press Function to call when a single press occurs
   * @param on_hold Function to call when the button is held for two seconds
   */
  void CheckForPressOrHold(void (*on_press)(void), void (*on_hold)(void))
  {
    // Wait for a press
    if (GetState() != ButtonState::ON) return;
    
    // Wait for button to stop being pressed by waiting for GetState() to equal OFF
    // If the button is held for two seconds, call the on_hold function, otherwise
    // call the on_press function.
    // This means that the on_press func will be called only once the user releases the button
    uint16_t start = millis();
    while (GetState() == ButtonState::ON)
    {
      if (millis() - start >= 2000)
      {
        // Button has been held for two seconds
        on_hold();

        // Wait for the player to let go of the button, otherwise the on_press function would get called immediately after
        while (GetState() == ButtonState::ON) {};
        return;
      }
    }

    // The button has been pressed but was not held for two seconds
    on_press();
  }
};

#endif
