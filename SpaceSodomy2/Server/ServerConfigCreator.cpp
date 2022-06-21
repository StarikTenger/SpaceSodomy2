#include "ServerConfigCreator.h"

void ServerConfigCreator::run() {
	{write_config("bot_list.conf", R"(
BOT
    BOT_TYPE EDGAR_BOT
    GUN_NAME heavy
    HULL_NAME heavy
    LEFT_MODULE_NAME NONE
    RIGHT_MODULE_NAME NONE
END
BOT
    BOT_TYPE EDGAR_BOT
    GUN_NAME cascade
    HULL_NAME light
    LEFT_MODULE_NAME NONE
    RIGHT_MODULE_NAME NONE
END
END

		)"); }
}
