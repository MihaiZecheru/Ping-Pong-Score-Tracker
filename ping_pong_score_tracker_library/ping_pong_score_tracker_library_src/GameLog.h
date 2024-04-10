#ifndef GAME_H
#define GAME_H

enum Player
{
	Blue = 1,
	Red = 2
};

struct Point
{
	Player winner;
	Player server;
};

class GameLog
{
private:
	Player starting_server;
	Point point_order[100];
	uint8_t points_played = 0;

public:
	GameLog(Player starting_server) : starting_server(starting_server) {};

	void AddPoint(Player winner)
	{
		Point point;
		point.winner = winner;
		point.server = GetCurrentServer();
		point_order[points_played++] = point;
	}
	
	void RemovePoint(Player player)
	{
		Point empty_point;
		point_order[points_played--] = empty_point;
	}
	
	Player GetCurrentServer()
	{
		Player server = starting_server;
		while (points_played >= 5)
		{
			points_played -= 5;
			if (server == Player::Blue)
				server = Player::Red;
			else server = Player::Blue;
		}
		return server;
	}
	
	Player GetLastWinner()
	{
		return point_order[points_played].winner;
	}
};

#endif