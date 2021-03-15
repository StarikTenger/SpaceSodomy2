#include "pch.h"
#include "Id_Manager.h"
#include <iostream>

void Id_Manager::set_id(iId* entity) {
    entity->set_id(get_new_id());
}

int Id_Manager::get_new_id() {
    return id_current++;
}

void Id_Manager::delete_id() {
    // Use it in case of id table
}
