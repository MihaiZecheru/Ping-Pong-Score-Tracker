// HEADER => Button Class

#ifndef BTN_H
#define BTN_H

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

  ButtonState GetState()
  {
    return (digitalRead(pin) == HIGH) ? ButtonState::ON : ButtonState::OFF;
  }

  /**
   * Check if the button is pressed or if the button was held for three seconds, and then call the given on_press function or the given on_hold function respectively
   * Note: call this function repeatedly in a forever loop, such as a while (true) loop
   * 
   * @param on_press Function to call when a press occurs
   * @param on_hold Function to call when the button is held for three seconds
   */
  void CheckForPressOrHold(int (*on_press)(RF24*), int (*on_hold)(RF24*), RF24* argument)
  {
    // Wait for a press
    if (GetState() != ButtonState::ON) return;
    
    // Wait for button to stop being pressed by waiting for GetState() to equal OFF
    // If the button is held for three seconds, call the on_hold function, otherwise
    // call the on_press function.
    // This means that the on_press func will be called only once the user releases the button
    uint32_t start = millis();
    while (GetState() == ButtonState::ON)
    {
      if (millis() - start >= 3000)
      {
        // Button has been held for three seconds
        on_hold(argument);

        // Wait for the player to let go of the button, otherwise the on_press function would get called immediately after
        while (GetState() == ButtonState::ON) {};
        return;
      }
    }

    // The button has been pressed but was not held for three seconds
    on_press(argument);
  }
};

#endif
