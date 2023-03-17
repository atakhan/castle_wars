#include "Game.h"

Game::Game() {
  GenerateCastles();
  GeneratePath();
  // warrior = Warrior(blueCastle.pos, path, CW_BLUE);
  // std::vector<Milestone> reversedPath (path.rbegin(), path.rend());
  // warrior2 = Warrior(redCastle.pos, reversedPath, CW_RED);
  ui = GamePlayUi();
  road = Road(path);
}

void Game::GenerateCastles() {
  castles.push_back(Castle({100, 100}, CW_BLUE));
  castles.push_back(Castle({900, 900}, CW_RED));
}

void Game::GeneratePath() {
  path.push_back(Milestone({100,100}));
  path.push_back(Milestone({150,210}));
  path.push_back(Milestone({320,260}));
  path.push_back(Milestone({430,430}));
  path.push_back(Milestone({540,540}));
  path.push_back(Milestone({630,630}));
  path.push_back(Milestone({720,790}));
  path.push_back(Milestone({810,810}));
  path.push_back(Milestone({900,900}));
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Draw() {
  DrawStats();
  road.Draw();
  Castle::DrawAll(castles);
  // warrior.Draw();
  // warrior2.Draw();
  // ui.Draw(warrior);
  // Warrior::DrawPath(warrior.path);
}

void Game::Update() {
  
  // warrior.Update(ui);
  // warrior2.Update(ui);

}
