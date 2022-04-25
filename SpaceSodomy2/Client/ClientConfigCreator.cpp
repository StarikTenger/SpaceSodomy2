#include "ClientConfigCreator.h"

void ClientConfigCreator::run() {
	// client_config.conf
	{write_config("client_config.conf", R"(
localhost 8001 0 Name
		)"); }
	// keys.conf
	{write_config("keys.conf", R"(
ENGINE_LIN_FORWARD W Up END
ENGINE_LIN_BACKWARD S Down END
ENGINE_LIN_LEFT Left END
ENGINE_LIN_RIGHT Right END
ENGINE_ANG_LEFT A END
ENGINE_ANG_RIGHT D END
SHOOT Space END
STABILIZE_ROTATION LControl END
STABILIZE_ROTATION_AUTO LAlt END
BOOST LShift END
ZOOM_IN X END
ZOOM_OUT Z END
RESPAWN R END
BONUS_ACTIVATION F END
LEFT_MODULE Q END
RIGHT_MODULE E END
MENU Tab END
FULLSCREEN F11 END
REPLAY_SPEED_UP W Up END
REPLAY_SPEED_DOWN S Down END
NEXT_PLAYER D Right END
PREVIOUS_PLAYER A Left END
REPLAY_PLAY Space END
		)"); }
	// sound_settings.conf
	{write_config("sound_settings.conf", R"(
100
32
		)"); }
	// fonts.conf
	{write_config("fonts.conf", R"(
TEMPLATE [ font arial neon square ]
{ * fonts/*.ttf }
		)"); }
	// setup.conf
	{write_config("setup.conf", R"(
GUN default
HULL default
LEFT_MODULE FORCE
RIGHT_MODULE DASH
		)"); }
	// sounds.conf
	{write_config("sounds.conf", R"(
TEMPLATE [ explosion fire hit  
    death laser impulse blink  ]
{ * sounds/*.wav }

shot sounds/launch.wav
damage sounds/damage.wav
bonus_pickup sounds/bonus_pickup.wav
engineOn sounds/engine.wav
engineOff sounds/engineOff.wav
engine sounds/jet-engine.wav
projectile_hit sounds/projectile_hit.wav

# Modules
force sounds/splash.wav
shotgun sounds/shotgun.wav
rocket sounds/rocket.wav
dash sounds/dash.wav
		)"); }
	// soundtrack.conf
	{write_config("soundtrack.conf", R"(
TEMPLATE [ 00 04 05 06 07 08 09 10 ]
{ ss* soundtrack/ss*.ogg }
		)"); }
	// textures.conf
	{write_config("textures.conf", R"(
# particles
circle16 textures/particles/circle16.png
circle32 textures/particles/circle32.png
particleSmoke textures/particles/particleSmoke.png 
particle_laser textures/particles/particleLaser.png 
particleFlame textures/particles/particleFlame.png 
force textures/particles/force.png
effect textures/particles/effect.png
box textures/particles/box.png
laser_aura textures/particles/laser.png
laser_kernel textures/particles/laserBase.png
laser_end textures/particles/laserEnd.png

interface textures/new-interface.png
bullet textures/bullet.png 
mass textures/mass.png 
rocket textures/rocket.png 
circle textures/circle.png 

blood textures/legacy/blood.png 
explosion textures/legacy/explosion.png 
death textures/legacy/death.png 
arrow textures/legacy/arrow.png
pointer textures/legacy/pointer.png
ground textures/legacy/ground.png

backwall textures/background/backwall.png
backwall1 textures/background/backwall1.png 
background textures/background/background.png 


# ship hulls
TEMPLATE [ light default heavy ]
{ ship_* textures/ship/*/ship.png }
{ ship_colors_* textures/ship/*/shipColors.png }
{ ship_aura_* textures/ship/*/shipAura.png }
{ *-hull textures/ship/*/ship.png }
{ *-hull-colors textures/ship/*/shipColors.png }

sector textures/ship/sector.png

# engines

engine_lin_forward textures/ship/engines/fireForward.png 
engine_lin_backward textures/ship/engines/fireBackward.png 
engine_lin_left textures/ship/engines/fireLeft.png 
engine_lin_right textures/ship/engines/fireRight.png 
engine_turn_right textures/ship/engines/fireTurnRight.png 
engine_turn_left textures/ship/engines/fireTurnLeft.png 

# Bonuses 
TEMPLATE [ Empty Energy EnergyLight Hp Berserk Immortal Charge Laser ]
{ bonus* textures/PowerUps/bonus*.png }

# Modules
TEMPLATE [ HP_UP ENERGY_UP SHOTGUN DASH ROCKET 
			FORCE IMMORTALITY BLINK INVISIBILITY MASS NONE ]
{ *-module textures/modules/*.png }



wall textures/walls/wall.png 
wallA textures/walls/wallTopLeft.png 
wallB textures/walls/wallTopRight.png 
wallC textures/walls/wallBottomRight.png 
wallD textures/walls/wallBottomLeft.png 
wallBorder textures/walls/wallBorder.png 
glowRed textures/walls/glowRed.png 
glowCornerRed textures/walls/glowCornerRed.png



NewGame textures/menu/NewGame.png
About textures/menu/About.png
Exit textures/menu/Exit.png
Settings textures/menu/Settings.png
Control textures/menu/Control.png
Main textures/menu/Main.png
TextField textures/menu/TextField.png
LeftArrow textures/menu/LeftArrow.png
Apply textures/menu/Apply.png
Sound textures/menu/Sound.png
Gun textures/menu/gun.png
Hull textures/menu/Hull.png
HUD textures/menu/HUD.png
Replay textures/menu/Replay.png
Modules textures/menu/Modules.png
CheckBoxInactive textures/menu/check_box.png
CheckBoxActive textures/menu/check_box_active.png

TEMPLATE [ default cascade snipe heavy ]
{ *-gun textures/guns/*.png }
		)"); }
}