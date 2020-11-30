#ifndef MODESTORAGE_H_
#define MODESTORAGE_H_

#include "ModeDefines.h"

#define MAX_MODE_COUNT 13

class ModeStorage
{
public:
	ModeStorage();
	void addMode(ModeDescription &descr);
	ModeDescription &getMode(uint8_t modeNumber);
	uint8_t getLastModeNumber() {return _lastMode;}
private:
	ModeDescription _storage[MAX_MODE_COUNT];
	uint8_t _tail = 0;
	uint8_t _lastMode;
};



#endif /* MODESTORAGE_H_ */