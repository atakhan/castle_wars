import express from "express";
import { createServer } from "http";
import { Server } from "colyseus";
import { GameRoom } from "./rooms/GameRoom";
import cors from "cors";

const app = express();
const port = Number(process.env.PORT || 2567);

app.use(cors());
app.use(express.json());

const gameServer = new Server({
  server: createServer(app),
});

// Register room
gameServer.define("game", GameRoom);

gameServer.listen(port);
console.log(`🎮 Castle Wars Server listening on ws://localhost:${port}`);

