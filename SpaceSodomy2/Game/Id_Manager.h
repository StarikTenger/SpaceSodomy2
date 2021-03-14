#pragma once
#include "iId.h"

class Id_Manager {
private:
	int id_current = 1;
public:
	int set_id(iId* entity);
	int get_new_id();
	void delete_id();
};