#ifndef SERVE_INDICATOR_H
#define SERVE_INDICATOR_H

#include "Player.h"

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

Player GetCurrentServer(Player starting_server, Player other_server)
{
  // From ScoreIndicator.h
  int total_score = blue_score + red_score;

  // The amount of times the server has changed is the amount of sets of 5 points that make up the total score
  int server_changes_count = floor(total_score / 5);

  // Get the current server
  // If the server has switched twice, it's back to the original server
  return server_changes_count % 2 == 0 ? starting_server : other_server;
}

#endif
