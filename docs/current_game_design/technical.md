# Технический геймдизайн — Castle Wars

Документ описывает **техническую реализацию** текущей версии: архитектура, сеть, ECS, клиентские механизмы синхронизации. Игровые механики с точки зрения дизайна — в отдельном документе «Игровой геймдизайн».

---

## 1. Архитектура

- **Сервер**: Node.js, Colyseus. Комната типа `game`, игровой цикл **20 TPS**. Вся игровая логика в ECS (Entity-Component-System); после каждого тика состояние ECS синхронизируется в Colyseus Schema и рассылается клиентам.
- **Клиент**: браузер, PixiJS (2D WebGL), Colyseus client. Отрисовка **60 FPS**, ввод и отправка команд, client-side prediction для своего игрока, интерполяция и reconciliation.

---

## 2. ECS на сервере

### 2.1. Компоненты

- **Position** — x, y (мир в пикселях).
- **Velocity** — vx, vy.
- **InputController** — playerId, waypoints (очередь точек маршрута `{ x, y }[]`; пустая — стоять на месте).
- **NetworkReplication** — playerId, shouldReplicate (для отбора сущностей в snapshot).
- **Direction** — angle (радианы).
- **Collider** — shape (`"circle"` | `"aabb"`), isStatic, radius (для circle), halfWidth, halfHeight (для aabb).
- **Interactable** — interactionRadius, kind (`"chest"` | `"resource"`), state (`"closed"` | `"open"` | `"empty"`).

### 2.2. Системы (порядок выполнения каждый тик)

1. **InputSystem** — для сущностей с InputController: текущая цель = waypoints[0]. Если waypoints не пуст: вычисляет расстояние до цели; **круг прибытия** (радиус 15 px): вне круга — полная скорость 200 px/s; внутри — скорость пропорциональна расстоянию; при расстоянии &lt; 1 px — snap в точку, shift waypoints, переход к следующей точке. Пустой waypoints — velocity = 0. Обновляет Direction из velocity.
2. **MovementSystem** — для сущностей с Position и Velocity: `position.x += velocity.x * deltaTime`, `position.y += velocity.y * deltaTime`.
3. **CollisionSystem** — динамические сущности (имеющие Velocity) против статических (Collider.isStatic). Разрешение: circle vs circle, circle vs AABB, AABB vs circle, AABB vs AABB; сдвиг позиции динамического объекта для устранения пересечения. Скорость не обнуляется.

### 2.3. Сущности

- **Игрок**: создаётся при `onJoin`, id вида `player_N`. Компоненты: Position(0,0), Velocity(0,0), InputController(playerId), NetworkReplication(playerId), Direction(0), Collider("circle", false, 30, 0, 0).
- **Статические препятствия**: создаются в `spawnStaticObstacles()`, id `static_N`. Position + Collider("aabb", true, 0, halfWidth, halfHeight).
- **Интерактивы**: создаются в `spawnInteractables()`, id `static_N`. Position, Collider("aabb", true, …), Interactable(60, kind, "closed").

---

## 3. Сеть

### 3.1. Клиент → сервер

- **input** — тело: `{ seq, ts, targetX, targetY }` (цель движения в мировых координатах). Отправляется по правому клику по карте (после выбора персонажа левым кликом). На сервере rate-limit: не чаще одного moveTo в **100** мс на игрока.
- **interact** — без тела. Сервер по sessionId находит игрока, ищет ближайший статический объект с Interactable в state "closed" в радиусе interactionRadius, переводит его в "open" и обновляет Schema.

### 3.2. Сервер → клиент (Colyseus state)

- **GameState**: `players` (MapSchema<Player>), `staticEntities` (MapSchema<StaticEntity>).
- **Player**: id, x, y, vx, vy, direction (радианы).
- **StaticEntity**: id, x, y, shape, radius, halfWidth, halfHeight, interactableKind, interactableState.
- Статика заполняется один раз при создании комнаты (`syncStaticState()`); игроки перезаписываются каждый тик из ECS (`syncState()`).

### 3.3. Обработка ввода на сервере

- Сообщения `input` обрабатываются в `onMessage("input")`: по playerId (sessionId) находится сущность с InputController. Вызывается pathfinding (см. раздел 6). Результат — массив waypoints; записывается в InputController.waypoints. Сервер отправляет путь клиенту (`client.send("path", path)`) для синхронизации предсказания. Движение по маршруту и обновление скорости происходят в следующих тиках (InputSystem → MovementSystem → CollisionSystem).

---

## 4. Клиент: prediction, reconciliation, interpolation

### 4.1. Client-side prediction (свой игрок)

- Ввод: только мышь. Левый клик по своему персонажу — выбор (characterSelected). Правый клик по карте (если персонаж выбран) — отправка цели (targetX, targetY), установка localTarget (fallback до получения пути) и добавление команды в pendingInputs. Сервер отвечает сообщением `path` с массивом waypoints; клиент сохраняет их в localWaypoints и lastReceivedPath, сбрасывает localTarget. Предсказание использует localWaypoints[0] как текущую цель (если путь получен) — это устраняет дёргание при reconciliation, т.к. клиент и сервер следуют одному маршруту.
- Локальное состояние: x, y, vx, vy, direction; localTarget (fallback); localWaypoints (очередь точек маршрута). В ticker (60 FPS): цель = localWaypoints[0] или localTarget; та же логика круга прибытия, что на сервере; при достижении waypoint — shift, переход к следующей точке. Затем `x += vx * dt`, `y += vy * dt`, разрешение коллизий. Отрисовка своего персонажа — из локального состояния.

### 4.2. Pending inputs

- Каждая отправленная команда «move to» сохраняется: seq, targetX, targetY, timestamp. Используется при reconciliation. Команды старше **200** мс удаляются.

### 4.3. Reconciliation

- При обновлении своего игрока в state сравнивается локальная позиция с серверной (порог **5** пикселей). При расхождении локальные x, y, vx, vy подменяются серверными, затем вызывается replayPendingInputs: если есть lastReceivedPath (путь от сервера), устанавливается localWaypoints = path; иначе localTarget = последняя цель из pendingInputs. Подтверждённые команды отбрасываются по таймауту (200 мс).

### 4.4. Interpolation (другие игроки)

- Для каждого другого игрока хранится очередь снимков: x, y, vx, vy, direction, timestamp. Максимум **10** снимков. Отрисовка: целевое время = `now - 100` мс; линейная интерполяция между двумя снимками по x, y, direction для сглаживания при лагах.

---

## 5. Визуализация (клиент)

- PixiJS Application, один общий canvas (подменяется в DOM на renderer.view при необходимости). Размер = окно браузера, resolution = devicePixelRatio, autoDensity.
- Игроки: Graphics, drawCircle(0, 0, 30), цвет 0x00ff00 (свой) / 0x0088ff (другие), стрелка направления (lineTo(25, 0)), rotation = direction.
- Статика: drawRect по halfWidth/halfHeight, позиция entity.x/y + center экрана. Цвета по типу (препятствие / интерактив closed/open). При изменении interactableState перерисовывается объект.
- Подсказка «E — взаимодействовать»: проверка расстояния от localPlayerState до каждого staticEntity с interactableKind и interactableState === "closed" ≤ 60; показ/скрытие блока в UI.

---

## 6. Pathfinding (прокладывание маршрута)

### 6.1. Принятые решения

| Решение | Выбор | Обоснование |
|---------|-------|-------------|
| Где выполняется | Только на сервере | Один источник истины; клиент получает готовый путь и использует его для предсказания. |
| Алгоритм | A* по сетке (grid-based) | Простая реализация, предсказуемое поведение, достаточно для текущего размера мира и числа препятствий. |
| Представление препятствий | AABB, раздутые на радиус игрока | Персонаж — круг r=30; клетка непроходима, если её центр попадает в «раздутую» геометрию препятствия. |
| Синхронизация с клиентом | Сервер отправляет путь сообщением `path` | Клиент предсказывает по тому же маршруту — устраняет дёргание при reconciliation. |

### 6.2. Реализация

**Модуль**: `server/src/pathfinding/Pathfinder.ts`

**Параметры**:
- **cellSize** = 24 px — размер клетки сетки. Компромисс между точностью пути и числом узлов.
- **playerRadius** = 30 px — радиус игрока; препятствия раздуваются на эту величину.
- **worldBounds** = ±450 px по осям — границы мира для построения сетки.

**Алгоритм**:
1. **Сетка**: мир разбивается на клетки 24×24 px. Клетка = узел графа.
2. **Непроходимые клетки**: для каждой клетки проверяется центр. Если центр попадает в AABB препятствия, раздутую на playerRadius (30 px), клетка помечается как blocked.
3. **A***: поиск пути от клетки старта до клетки цели. Соседи — 8 направлений (включая диагонали); стоимость диагонального шага = √2.
4. **Эвристика**: octile distance — `max(dx, dy) + (√2 − 1) * min(dx, dy)`.
5. **Результат**: последовательность центров клеток. Последняя точка заменяется на точную цель (targetX, targetY) для точного прибытия.

**Особые случаи**:
- Старт или цель вне границ мира → возвращается `[{ x: endX, y: endY }]` (прямое движение).
- Старт в непроходимой клетке → то же.
- Цель в непроходимой клетке → `[]` (путь не найден).
- Путь не найден (непреодолимое препятствие) → `[{ x: endX, y: endY }]` (fallback).
- Старт и цель в одной клетке → `[{ x: endX, y: endY }]`.

**Интеграция**:
- **GameWorld.handlePlayerInput**: при получении `input` с targetX, targetY собирает препятствия из `getStaticEntities()` (все AABB: стены, интерактивы), вызывает `pathfinder.findPath(start, end, obstacles)`, записывает результат в `InputController.waypoints`, возвращает путь для отправки клиенту.
- **GameRoom**: после `handlePlayerInput` вызывает `client.send("path", path)`.
- **InputSystem**: текущая цель = waypoints[0]; при достижении (dist &lt; 1 px) — snap, shift, переход к следующей точке; при пустом waypoints — velocity = 0.

### 6.3. Клиент

- **room.onMessage("path", (path) => ...)**: сохраняет путь в `localWaypoints` и `lastReceivedPath`, сбрасывает `localTarget`.
- **Предсказание**: использует `localWaypoints[0]` как цель; при достижении — shift; при пустом — `lastReceivedPath = null`.
- **Reconciliation**: при replay подставляет `localWaypoints = lastReceivedPath`, чтобы клиент продолжал предсказывать по тому же маршруту.

---

## 7. Подключение и комната

- Клиент: `Client(serverUrl)`, `joinOrCreate('game')`. URL задаётся через VITE_SERVER_URL или по hostname (localhost → ws://localhost:2567, иначе ws://server:2567).
- Сервер: при onJoin создаётся сущность игрока в GameWorld и запись в state.players; при onLeave сущность и запись удаляются. Reconnection и восстановление сессии не реализованы.

---

*Документ актуален на момент реализации: движение с pathfinding, коллизии, статика, интерактивы, синхронизация, prediction, interpolation, reconciliation.*
