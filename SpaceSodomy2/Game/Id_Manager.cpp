#include "pch.h"
#include "Id_Manager.h"
#include <iostream>

int Id_Manager::set_id(iId* entity) {
    int id = get_new_id();
    entity->set_id(id);
    return id;
}

int Id_Manager::get_new_id() {
    std::cout << id_current << "\n";
    return id_current++;
}

void Id_Manager::delete_id() {
    // Use it in case of id table
}
