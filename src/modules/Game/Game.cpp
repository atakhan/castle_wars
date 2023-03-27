#include "Game.h"

Game::Game() {
  gameState = GAME_MENU_STATE;
  fight = Fight();
  control = Control();
  menu = Menu();
  castles.push_back(Castle({300, 300}, CW_BLUE, PLAYER,  0));
  castles.push_back(Castle({700, 800}, CW_RED,  AI,      7));
  castles.push_back(Castle({550, 200}, CW_RED,  AI,      0));
  castles.push_back(Castle({150, 100}, GRAY,    NEUTRAL, 1));
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Update() {
  
  if (gameState == GAME_PLAY_STATE) {

    control.Update(castles, roads);
    fight.Update(castles, warriors, roads);
    Castle::UpdateAll(castles, warriors);
    Warrior::UpdateAll(warriors);

  } else if (gameState == GAME_MENU_STATE) {

    menu.Update(gameState);

  }
}

void Game::Draw() {
  
  if (gameState == GAME_PLAY_STATE) {

    DrawStats();
    Road::DrawAll(roads);
    Castle::DrawAll(castles);
    Warrior::DrawAll(warriors);
    control.Draw();
  
  } else if (gameState == GAME_MENU_STATE) {
  
    menu.Draw();
  
  }
}
