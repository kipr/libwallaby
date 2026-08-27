

#pragma once

#define MAX_NAME_LEN 16

struct object_detection
{
	int category;
	int name_len;
	char name[MAX_NAME_LEN];
	float confidence;
	struct box
	{
		float x;
		float width;
		float y;
		float height;
	} box;
};


