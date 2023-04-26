#pragma once

struct Rect {
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float array[4];
	};

	Rect()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	};

	Rect(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	};

};