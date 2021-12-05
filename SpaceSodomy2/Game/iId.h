#pragma once
class iId {
private:
	int id = 0;
public:
	iId() = default;
	iId(int id_);
	int get_id();
	void set_id(int val);
};

