#pragma once
#include "iId.h"

class IdManager {
private:
	int id_current = 1;
public:
	void set_id(iId* entity);
	int get_new_id();
	void delete_id();
};