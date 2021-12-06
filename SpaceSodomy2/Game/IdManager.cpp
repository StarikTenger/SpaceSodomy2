#include "pch.h"
#include "IdManager.h"
#include <iostream>

void IdManager::set_id(iId* entity) {
    entity->set_id(get_new_id());
}

int IdManager::get_new_id() {
    return id_current++;
}

void IdManager::delete_id() {
    // Use it in case of id table
}
