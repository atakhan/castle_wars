import * as PIXI from 'pixi.js';
import { Client } from 'colyseus.js';
import { GameState, Player } from './schemas/GameState';
import { GameClient } from './GameClient';

// In Docker, server is accessible via service name
// In local dev, use localhost
const SERVER_URL = import.meta.env.VITE_SERVER_URL || 
  (window.location.hostname === 'localhost' 
    ? 'ws://localhost:2567' 
    : 'ws://server:2567');

// Initialize PixiJS
function initApp() {
  const canvas = document.getElementById('game-canvas') as HTMLCanvasElement;
  
  if (!canvas) {
    console.error('Canvas element not found');
    return;
  }
  
  // Используем размер окна браузера для адаптивности
  const width = window.innerWidth;
  const height = window.innerHeight;
  
  console.log('Window size:', width, 'x', height);
  
  // Устанавливаем размер canvas перед созданием Application
  canvas.width = width;
  canvas.height = height;
  
  // For PixiJS v7.3.2, используем существующий canvas
  const app = new PIXI.Application({
    width: width,
    height: height,
    backgroundColor: 0x2a2a2a,
    antialias: true,
    canvas: canvas,
    autoStart: true,
    sharedTicker: false,
    resolution: window.devicePixelRatio || 1,
    autoDensity: true,
  });
  
  // В PixiJS v7 canvas доступен через renderer.view
  // PixiJS может создать свой canvas, даже если мы передали существующий
  const renderer = app.renderer as any;
  const pixiCanvas = renderer?.view as HTMLCanvasElement;
  
  // Если PixiJS создал свой canvas, заменяем оригинальный в DOM
  if (pixiCanvas && pixiCanvas !== canvas) {
    if (canvas.parentElement) {
      pixiCanvas.id = 'game-canvas';
      canvas.parentElement.replaceChild(pixiCanvas, canvas);
    }
  }
  
  // Используем canvas от рендерера или оригинальный
  const finalCanvas = (pixiCanvas || canvas) as HTMLCanvasElement;
  
  // Устанавливаем стили для правильного отображения
  finalCanvas.style.width = '100vw';
  finalCanvas.style.height = '100vh';
  finalCanvas.style.display = 'block';
  finalCanvas.style.position = 'absolute';
  finalCanvas.style.top = '0';
  finalCanvas.style.left = '0';

  // Initialize game client after PixiJS is ready
  const gameClient = new GameClient(app, SERVER_URL);
  gameClient.start();

  // Update UI
  const statusEl = document.getElementById('status');
  const playersEl = document.getElementById('players');

  if (statusEl && playersEl) {
    gameClient.onStatusChange = (status) => {
      statusEl.textContent = status;
    };
    
    gameClient.onPlayersChange = (count) => {
      playersEl.textContent = count.toString();
    };
  }
}

// Wait for DOM to be ready
if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', initApp);
} else {
  initApp();
}

