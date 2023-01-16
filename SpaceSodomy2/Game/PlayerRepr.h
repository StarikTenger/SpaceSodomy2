#pragma once
#include "Player.h"

class Game;

class PlayerProcessingStage {
public:
	enum STAGE {
		UNBORN,
		WAITING_IN_LOBBY,
		PLAYING,
	};
};

//
// This class is a representation of the player inside Game
// PlayerRepr is not an Game object
// It is only used when iteracting with the outside world
//
struct PlayerRepr : public PlayerProcessingStage {
	STAGE stage = WAITING_IN_LOBBY;
	PlayerDefHint def_to_apply_next;
	Player* player = nullptr;
	PlayerRepr(PlayerDefHint def) : stage(WAITING_IN_LOBBY), def_to_apply_next(def) {}
};

// Stores a ptr to a player and the game he is in
// Might expire
class PlayerHandle : protected PlayerProcessingStage {
protected:
	PlayerRepr* repr = nullptr;
	Game* game = nullptr;
	friend class Game;
	PlayerHandle() = default;
public:
	bool is_wellformed() { return repr; }
	bool is_playing() { return repr && repr->stage == PLAYING; }
	Player* get_raw_player() {
		if (repr && repr->stage == PLAYING) {
			return repr->player;
		}
		return nullptr;
	}
	void apply_command(int command, int val);
	void delete_player();

	// will update as soon as possible (after next death)
	void update_def(const PlayerDefHint def) { repr->def_to_apply_next = def; apply_def(); }
	void apply_def();
};

