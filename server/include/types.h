#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct key {
	int id; /* Key id for uinput */
	int state; /* 0 or 1 ---- released or pressed */
} key;

typedef struct uinput_packet {
	key key_array[4];
	bool request_quit;
} uinput_packet;

struct socket_union {
	int primary_socket;
	int opened_socket;
};

#endif /* _TYPES_H_ */