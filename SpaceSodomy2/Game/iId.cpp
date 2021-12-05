#include "pch.h"
#include "iId.h"

iId::iId(int id_) : id(id_) {}

int iId::get_id() {
    return id;
}

void iId::set_id(int val) {
    id = val;
}
