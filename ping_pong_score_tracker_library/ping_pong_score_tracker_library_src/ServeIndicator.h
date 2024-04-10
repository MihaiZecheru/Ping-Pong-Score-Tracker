#ifndef SERVE_INDICATOR_H
#define SERVE_INDICATOR_H

class ServeIndicator
{
private:
	uint8_t pin;
	bool is_on;
	
public:
	ServeIndicator(const uint8_t pin) : pin(pin), is_on(false)
	{
		pinMode(pin, OUTPUT);
	}
	
	void Show()
	{
		digitalWrite(pin, HIGH);
		is_on = true;
	}
	
	void Hide()
	{
		digitalWrite(pin, LOW);
		is_on = false;
	}
	
	void Toggle()
	{
		if (is_on) Hide();
		else Show();
	}
};



#endif