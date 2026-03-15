# Алгоритмы для гексагональных сеток

## Соседи (Cube/Axial)

```typescript
const HEX_DIRECTIONS = [
  new Hex(1, 0, -1),   // 0: E
  new Hex(1, -1, 0),   // 1: NE
  new Hex(0, -1, 1),   // 2: NW
  new Hex(-1, 0, 1),   // 3: W
  new Hex(-1, 1, 0),   // 4: SW
  new Hex(0, 1, -1),   // 5: SE
];

function hexNeighbor(hex: Hex, direction: number): Hex {
  return hex.add(HEX_DIRECTIONS[direction]);
}
```

## Диагональные соседи

Шесть гексов «через угол» (расстояние 2):

```typescript
const HEX_DIAGONALS = [
  new Hex(2, -1, -1), new Hex(1, -2, 1), new Hex(-1, -1, 2),
  new Hex(-2, 1, 1), new Hex(-1, 2, -1), new Hex(1, 1, -2),
];

function hexDiagonalNeighbor(hex: Hex, direction: number): Hex {
  return hex.add(HEX_DIAGONALS[direction]);
}
```

## Соседи в Offset

В offset координатах вектор смещения к соседу зависит от чётности col/row — нужны две таблицы. Проще: перевести в cube → neighbor → перевести обратно. См. [02-conversions](./02-conversions.md).

---

## Line drawing (трассировка линии)

Все гексы на прямой между двумя точками. Линейная интерполяция + округление. Epsilon (1e-6) стабилизирует округление на границах гексов.

```typescript
function hexLinedraw(a: Hex, b: Hex): Hex[] {
  const N = a.distance(b);
  const a_nudge = new Hex(a.q + 1e-6, a.r + 1e-6, a.s - 2e-6);
  const b_nudge = new Hex(b.q + 1e-6, b.r + 1e-6, b.s - 2e-6);
  const results: Hex[] = [];
  const step = 1 / Math.max(N, 1);
  for (let i = 0; i <= N; i++) {
    results.push(a_nudge.lerp(b_nudge, step * i).round());
  }
  return results;
}
```

---

## Расстояние (Cube)

```typescript
function cubeDistance(a: Hex, b: Hex): number {
  const vec = a.subtract(b);
  return (Math.abs(vec.q) + Math.abs(vec.r) + Math.abs(vec.s)) / 2;
}
// Эквивалентно: max(|dq|, |dr|, |ds|)
```

## Расстояние (Axial)

```typescript
function axialDistance(a: Hex, b: Hex): number {
  return (Math.abs(a.q - b.q) + Math.abs(a.q + a.r - b.q - b.r) + Math.abs(a.r - b.r)) / 2;
}
```

## Евклидово расстояние (axial)

При необходимости «прямолинейного» расстояния в метрике плоскости (например для AI):  
`sqrt(dq² + dr² + dq·dr)` (Xiangguo Li, 2013).

## Расстояние (Doubled)

```typescript
// doublewidth (pointy top)
function doublewidthDistance(a: DoubledCoord, b: DoubledCoord): number {
  const dcol = Math.abs(a.col - b.col);
  const drow = Math.abs(a.row - b.row);
  return drow + Math.max(0, (dcol - drow) / 2);
}

// doubleheight (flat top)
function doubleheightDistance(a: DoubledCoord, b: DoubledCoord): number {
  const dcol = Math.abs(a.col - b.col);
  const drow = Math.abs(a.row - b.row);
  return dcol + Math.max(0, (drow - dcol) / 2);
}
```

---

## Диапазон (Range)

Все гексы в радиусе N от центра:

```typescript
function hexRange(center: Hex, N: number): Hex[] {
  const results: Hex[] = [];
  for (let q = -N; q <= N; q++) {
    for (let r = Math.max(-N, -q - N); r <= Math.min(N, -q + N); r++) {
      results.push(center.add(new Hex(q, r, -q - r)));
    }
  }
  return results;
}
```

### Пересечение двух диапазонов

Область «гексы в радиусе N1 от H1 и в радиусе N2 от H2»:

```typescript
const qmin = Math.max(H1.q - N1, H2.q - N2);
const qmax = Math.min(H1.q + N1, H2.q + N2);
const rmin = Math.max(H1.r - N1, H2.r - N2);
const rmax = Math.min(H1.r + N1, H2.r + N2);
const smin = Math.max(H1.s - N1, H2.s - N2);
const smax = Math.min(H1.s + N1, H2.s + N2);
// затем цикл: q от qmin до qmax, r от max(rmin, -q-smax) до min(rmax, -q-smin)
```

---

## Достижимость с препятствиями (BFS)

```typescript
function hexReachable(start: Hex, movement: number, isBlocked: (h: Hex) => boolean): Hex[] {
  const toKey = (h: Hex) => `${h.q},${h.r}`;
  const visited = new Set<string>([toKey(start)]);
  const fringes: Hex[][] = [[start]];

  for (let k = 1; k <= movement; k++) {
    fringes[k] = [];
    for (const hex of fringes[k - 1]) {
      for (let dir = 0; dir < 6; dir++) {
        const neighbor = hexNeighbor(hex, dir);
        const key = toKey(neighbor);
        if (!visited.has(key) && !isBlocked(neighbor)) {
          visited.add(key);
          fringes[k].push(neighbor);
        }
      }
    }
  }
  return Array.from(visited).map(s => {
    const [q, r] = s.split(',').map(Number);
    return new Hex(q, r, -q - r);
  });
}
```

---

## Поворот (Cube)

Поворот на 60° вправо (по часовой): `[q, r, s] → [-r, -s, -q]`

Поворот на 60° влево: `[q, r, s] → [-s, -q, -r]`

```typescript
hex.rotateRight();  // [-r, -s, -q]
hex.rotateLeft();   // [-s, -q, -r]
```

---

## Отражение (Cube)

```typescript
function reflectQ(h: Hex): Hex { return new Hex(h.q, h.s, h.r); }
function reflectR(h: Hex): Hex { return new Hex(h.s, h.r, h.q); }
function reflectS(h: Hex): Hex { return new Hex(h.r, h.q, h.s); }
```

---

## Кольца и спирали

**Важно:** `cubeRing(center, 0)` не обрабатывается этим циклом (radius === 0); при необходимости вернуть только center отдельно.

```typescript
function cubeRing(center: Hex, radius: number): Hex[] {
  if (radius === 0) return [center];
  const results: Hex[] = [];
  let hex = center.add(HEX_DIRECTIONS[4].scale(radius));
  for (let i = 0; i < 6; i++) {
    for (let j = 0; j < radius; j++) {
      results.push(hex);
      hex = hexNeighbor(hex, i);
    }
  }
  return results;
}

function cubeSpiral(center: Hex, radius: number): Hex[] {
  const results: Hex[] = [center];
  for (let k = 1; k <= radius; k++) {
    results.push(...cubeRing(center, k));
  }
  return results;
}
```

Количество гексов в N кольцах: `1 + 3 * N * (N + 1)`

---

## Field of view (поле зрения)

**Простой алгоритм:** для каждого гекса в радиусе провести линию от центра к нему. Если линия не пересекает стены — гекс виден.

```typescript
function hexVisible(center: Hex, target: Hex, isBlocked: (h: Hex) => boolean): boolean {
  const line = hexLinedraw(center, target);
  for (let i = 1; i < line.length; i++) {
    if (isBlocked(line[i])) return false;
  }
  return true;
}
```

**Альтернатива:** алгоритм «от центра наружу» — обход по спирали, ведение множества затенённых углов. См. [Clark Verbrugge's guide](http://www-cs-students.stanford.edu/~amitp/Articles/HexLOS.html).

---

## Pathfinding (A*)

Pathfinding на гексах совпадает с графовым A*:

1. **Соседи:** `hexNeighbor(hex, dir)` для dir 0..5; отфильтровать непроходимые.
2. **Эвристика:** расстояние в гексах × стоимость шага, например `hex.distance(goal) * costPerHex`.

```typescript
function heuristic(a: Hex, b: Hex, costPerHex: number): number {
  return a.distance(b) * costPerHex;
}
```

См. [Red Blob Games Pathfinding](https://www.redblobgames.com/pathfinding/a-star/introduction.html).
