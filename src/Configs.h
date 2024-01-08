#ifndef CASTLE_WARS_SRC_CONFIGS_H
#define CASTLE_WARS_SRC_CONFIGS_H
namespace CW {

#define CW_RED             {205,  70,   50,   255}
#define CW_BLUE            {0,    117,  162,  255}
#define CW_MENU_LINE_COLOR {238,  230,  210,  255}
#define CW_MENU_BG_COLOR   {190,  179,  150,  255}
#define CW_MAIN_BG         {245,  245,  235,  255}
#define CW_ROAD            {198,  197,  185,  255}

enum Fraction {
  PLAYER,
  AI,
  NEUTRAL
};

enum GameState {
  GAME_MENU_STATE,
  GAME_PLAY_STATE,
  GAME_WIN_STATE,
  GAME_OVER_STATE
};

enum CastleState {
  CASTLE_IDLE,
  CASTLE_ATTACK,
  CASTLE_DEFENSE
};


}  // namespace CW

#endif  // CASTLE_WARS_SRC_CONFIGS_H