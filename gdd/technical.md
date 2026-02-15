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
- **InputController** — playerId, targetX, targetY (цель движения в мировых координатах; null — стоять на месте).
- **NetworkReplication** — playerId, shouldReplicate (для отбора сущностей в snapshot).
- **Direction** — angle (радианы).
- **Collider** — shape (`"circle"` | `"aabb"`), isStatic, radius (для circle), halfWidth, halfHeight (для aabb).
- **Interactable** — interactionRadius, kind (`"chest"` | `"resource"`), state (`"closed"` | `"open"` | `"empty"`).

### 2.2. Системы (порядок выполнения каждый тик)

1. **InputSystem** — для сущностей с InputController: если заданы targetX, targetY, вычисляет вектор к цели и задаёт Velocity направлением к цели и модулем 200 px/s; при расстоянии до цели &lt; 2 px обнуляет скорость и сбрасывает цель; иначе обнуляет Velocity. Обновляет Direction из velocity при движении.
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

- Сообщения `input` обрабатываются в `onMessage("input")`: по playerId (sessionId) находится сущность с InputController и записываются targetX, targetY. Движение к цели и обновление скорости происходят в следующих тиках (InputSystem → MovementSystem → CollisionSystem).

---

## 4. Клиент: prediction, reconciliation, interpolation

### 4.1. Client-side prediction (свой игрок)

- Ввод: только мышь. Левый клик по своему персонажу — выбор (characterSelected). Правый клик по карте (если персонаж выбран) — отправка цели (targetX, targetY), установка localTarget и добавление команды в pendingInputs.
- Локальное состояние: x, y, vx, vy, direction; localTarget (цель движения или null). В ticker (60 FPS): если есть localTarget, вычисляется вектор к цели; при расстоянии &lt; 2 px цель сбрасывается и скорость обнуляется, иначе vx, vy задаются направлением к цели и 200 px/s, direction обновляется; затем `x += vx * dt`, `y += vy * dt`, разрешение коллизий circle vs AABB со статикой. Отрисовка своего персонажа — из локального состояния.

### 4.2. Pending inputs

- Каждая отправленная команда «move to» сохраняется: seq, targetX, targetY, timestamp. Используется при reconciliation. Команды старше **200** мс удаляются.

### 4.3. Reconciliation

- При обновлении своего игрока в state сравнивается локальная позиция с серверной (порог **5** пикселей). При расхождении локальные x, y, vx, vy подменяются серверными, затем вызывается replayPendingInputs: целевая точка устанавливается из **последней** команды в pendingInputs (localTarget = последняя цель). Подтверждённые команды отбрасываются по таймауту (200 мс).

### 4.4. Interpolation (другие игроки)

- Для каждого другого игрока хранится очередь снимков: x, y, vx, vy, direction, timestamp. Максимум **10** снимков. Отрисовка: целевое время = `now - 100` мс; линейная интерполяция между двумя снимками по x, y, direction для сглаживания при лагах.

---

## 5. Визуализация (клиент)

- PixiJS Application, один общий canvas (подменяется в DOM на renderer.view при необходимости). Размер = окно браузера, resolution = devicePixelRatio, autoDensity.
- Игроки: Graphics, drawCircle(0, 0, 30), цвет 0x00ff00 (свой) / 0x0088ff (другие), стрелка направления (lineTo(25, 0)), rotation = direction.
- Статика: drawRect по halfWidth/halfHeight, позиция entity.x/y + center экрана. Цвета по типу (препятствие / интерактив closed/open). При изменении interactableState перерисовывается объект.
- Подсказка «E — взаимодействовать»: проверка расстояния от localPlayerState до каждого staticEntity с interactableKind и interactableState === "closed" ≤ 60; показ/скрытие блока в UI.

---

## 6. Подключение и комната

- Клиент: `Client(serverUrl)`, `joinOrCreate('game')`. URL задаётся через VITE_SERVER_URL или по hostname (localhost → ws://localhost:2567, иначе ws://server:2567).
- Сервер: при onJoin создаётся сущность игрока в GameWorld и запись в state.players; при onLeave сущность и запись удаляются. Reconnection и восстановление сессии не реализованы.

---

*Документ актуален на момент реализации: движение, коллизии, статика, интерактивы, синхронизация, prediction, interpolation, reconciliation.*
