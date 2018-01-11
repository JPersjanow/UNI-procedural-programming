enum IDS { PLAYER, ENEMY, FRIEND, OBJECT, MOUSE };

//player

struct Student
{
	int ID;
	int x;
	int y;
	int score;
	int speed;
	//collision
	int width;
	int height;
	int boundx;
	int boundy;

};

struct Bottle
{
	int ID;
	int x;
	int y;
	int live;

	int width;
	int height;
	int boundx;
	int boundy;
};
