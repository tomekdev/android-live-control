#ifndef _UINPUT_H_
#define _UINPUT_H_
#include "types.h"

int initialize_uinput();
int parse_uinput_packet(uinput_packet &p);

#endif /* _UINPUT_H_ */