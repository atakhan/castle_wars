# Реализация на TypeScript

Готовый код на основе [Red Blob Games Implementation](https://www.redblobgames.com/grids/hexagons/implementation.html).

---

## Базовые типы

```typescript
class Point {
  constructor(public x: number, public y: number) {}
}

class Hex {
  constructor(public q: number, public r: number, public s: number) {
    if (Math.round(q + r + s) !== 0) throw new Error("q + r + s must be 0");
  }

  add(b: Hex) {
    return new Hex(this.q + b.q, this.r + b.r, this.s + b.s);
  }
  subtract(b: Hex) {
    return new Hex(this.q - b.q, this.r - b.r, this.s - b.s);
  }
  scale(k: number) {
    return new Hex(this.q * k, this.r * k, this.s * k);
  }
  rotateLeft() {
    return new Hex(-this.s, -this.q, -this.r);
  }
  rotateRight() {
    return new Hex(-this.r, -this.s, -this.q);
  }
  neighbor(direction: number) {
    return this.add(Hex.directions[direction]);
  }
  len() {
    return (Math.abs(this.q) + Math.abs(this.r) + Math.abs(this.s)) / 2;
  }
  distance(b: Hex) {
    return this.subtract(b).len();
  }
  round() {
    let qi = Math.round(this.q);
    let ri = Math.round(this.r);
    let si = Math.round(this.s);
    const q_diff = Math.abs(qi - this.q);
    const r_diff = Math.abs(ri - this.r);
    const s_diff = Math.abs(si - this.s);
    if (q_diff > r_diff && q_diff > s_diff) qi = -ri - si;
    else if (r_diff > s_diff) ri = -qi - si;
    else si = -qi - ri;
    return new Hex(qi, ri, si);
  }
  lerp(b: Hex, t: number) {
    return new Hex(
      this.q * (1 - t) + b.q * t,
      this.r * (1 - t) + b.r * t,
      this.s * (1 - t) + b.s * t
    );
  }
  linedraw(b: Hex): Hex[] {
    const N = this.distance(b);
    const a_nudge = new Hex(this.q + 1e-6, this.r + 1e-6, this.s - 2e-6);
    const b_nudge = new Hex(b.q + 1e-6, b.r + 1e-6, b.s - 2e-6);
    const results: Hex[] = [];
    const step = 1 / Math.max(N, 1);
    for (let i = 0; i <= N; i++) {
      results.push(a_nudge.lerp(b_nudge, step * i).round());
    }
    return results;
  }

  static directions = [
    new Hex(1, 0, -1), new Hex(1, -1, 0), new Hex(0, -1, 1),
    new Hex(-1, 0, 1), new Hex(-1, 1, 0), new Hex(0, 1, -1),
  ];
}
```

---

## Offset координаты

```typescript
class OffsetCoord {
  constructor(public col: number, public row: number) {}
  static EVEN = 1;
  static ODD = -1;

  static qoffsetFromCube(offset: number, h: Hex): OffsetCoord {
    const col = h.q;
    const row = h.r + Math.floor((h.q + offset * (h.q & 1)) / 2);
    return new OffsetCoord(col, row);
  }
  static qoffsetToCube(offset: number, h: OffsetCoord): Hex {
    const q = h.col;
    const r = h.row - Math.floor((h.col + offset * (h.col & 1)) / 2);
    return new Hex(q, r, -q - r);
  }
  static roffsetFromCube(offset: number, h: Hex): OffsetCoord {
    const col = h.q + Math.floor((h.r + offset * (h.r & 1)) / 2);
    const row = h.r;
    return new OffsetCoord(col, row);
  }
  static roffsetToCube(offset: number, h: OffsetCoord): Hex {
    const q = h.col - Math.floor((h.row + offset * (h.row & 1)) / 2);
    const r = h.row;
    return new Hex(q, r, -q - r);
  }
}
```

---

## Layout и отрисовка

```typescript
class Orientation {
  constructor(
    public f0: number, public f1: number, public f2: number, public f3: number,
    public b0: number, public b1: number, public b2: number, public b3: number,
    public start_angle: number
  ) {}
}

class Layout {
  constructor(
    public orientation: Orientation,
    public size: Point,
    public origin: Point
  ) {}

  hexToPixel(h: Hex): Point {
    const M = this.orientation;
    const x = (M.f0 * h.q + M.f1 * h.r) * this.size.x + this.origin.x;
    const y = (M.f2 * h.q + M.f3 * h.r) * this.size.y + this.origin.y;
    return new Point(x, y);
  }
  pixelToHex(p: Point): Hex {
    const M = this.orientation;
    const pt = new Point(
      (p.x - this.origin.x) / this.size.x,
      (p.y - this.origin.y) / this.size.y
    );
    const q = M.b0 * pt.x + M.b1 * pt.y;
    const r = M.b2 * pt.x + M.b3 * pt.y;
    return new Hex(q, r, -q - r).round();
  }
  polygonCorners(h: Hex): Point[] {
    const corners: Point[] = [];
    const center = this.hexToPixel(h);
    for (let i = 0; i < 6; i++) {
      const angle = 2 * Math.PI * (this.orientation.start_angle - i) / 6;
      corners.push(new Point(
        center.x + this.size.x * Math.cos(angle),
        center.y + this.size.y * Math.sin(angle)
      ));
    }
    return corners;
  }

  static pointy = new Orientation(
    Math.sqrt(3), Math.sqrt(3) / 2, 0, 3 / 2,
    Math.sqrt(3) / 3, -1 / 3, 0, 2 / 3, 0.5
  );
  static flat = new Orientation(
    3 / 2, 0, Math.sqrt(3) / 2, Math.sqrt(3),
    2 / 3, 0, -1 / 3, Math.sqrt(3) / 3, 0
  );
}
```

---

## Хранение карты

### Hash-таблица (универсально)

```typescript
const map = new Map<string, TileData>();
const key = (h: Hex) => `${h.q},${h.r}`;
map.set(key(new Hex(1, -2, 1)), { terrain: "grass" });
```

### Формы карт

**Параллелограмм:**
```typescript
for (let q = q1; q <= q2; q++)
  for (let r = r1; r <= r2; r++)
    map.add(new Hex(q, r, -q - r));
```

**Шестиугольник (радиус N):**
```typescript
for (let q = -N; q <= N; q++) {
  const r1 = Math.max(-N, -q - N);
  const r2 = Math.min(N, -q + N);
  for (let r = r1; r <= r2; r++)
    map.add(new Hex(q, r, -q - r));
}
```

**Прямоугольник (pointy top):**
```typescript
for (let r = top; r <= bottom; r++) {
  const r_offset = Math.floor(r / 2);
  for (let q = left - r_offset; q <= right - r_offset; q++)
    map.add(new Hex(q, r, -q - r));
}
```

**Прямоугольник (flat top):**
```typescript
for (let q = left; q <= right; q++) {
  const q_offset = Math.floor(q / 2);
  for (let r = top - q_offset; r <= bottom - q_offset; r++)
    map.add(new Hex(q, r, -q - r));
}
```

**Треугольник (одна из ориентаций):**
```typescript
for (let q = 0; q <= size; q++)
  for (let r = 0; r <= size - q; r++)
    map.add(new Hex(q, r, -q - r));
```

Подробнее об индексации в 2D-массивах — см. [06-map-storage](./06-map-storage.md).

---

## DoubledCoord

```typescript
class DoubledCoord {
  constructor(public col: number, public row: number) {}
  static qdoubledFromCube(h: Hex): DoubledCoord {
    return new DoubledCoord(h.q, 2 * h.r + h.q);
  }
  qdoubledToCube(): Hex {
    const q = this.col;
    const r = (this.row - this.col) / 2;
    return new Hex(q, r, -q - r);
  }
  static rdoubledFromCube(h: Hex): DoubledCoord {
    return new DoubledCoord(2 * h.q + h.r, h.r);
  }
  rdoubledToCube(): Hex {
    const q = (this.col - this.row) / 2;
    const r = this.row;
    return new Hex(q, r, -q - r);
  }
}
```

---

## Wraparound maps (зацикленные карты)

Для шестиугольной карты радиуса N — шесть зеркальных центров. При выходе за край подменяем гекс на соответствующий из основной карты.

**Первый зеркальный центр:** `Cube(2*N+1, -N, -N-1)`. Остальные пять — повороты на 60° (через rotateRight/rotateLeft).

```typescript
function allMirrorCenters(radius: number): Hex[] {
  const M0 = new Hex(2 * radius + 1, -radius, -radius - 1);
  const out = [M0];
  let h = M0;
  for (let i = 0; i < 5; i++) {
    h = h.rotateRight();
    out.push(h);
  }
  return out;
}

function createMirrorTable(radius: number): Map<string, Hex> {
  const table = new Map<string, Hex>();
  const centers = allMirrorCenters(radius);
  const mapHexes = hexRange(new Hex(0, 0, 0), radius); // hexRange — см. 03-algorithms
  const toKey = (h: Hex) => `${h.q},${h.r}`;
  for (const M of centers) {
    for (const L of mapHexes) {
      const mirrored = L.add(M);
      table.set(toKey(mirrored), L);
    }
  }
  return table;
}

function wrapHex(hex: Hex, mirrorTable: Map<string, Hex>): Hex {
  const key = `${hex.q},${hex.r}`;
  return mirrorTable.get(key) ?? hex;
}
```

---

## Линейная интерполяция и линия

`linedraw` возвращает все гексы на линии между двумя точками. Используется для line-of-sight, трассировки и т.п.

Epsilon (1e-6) в коде нужен для стабильного округления на границах гексов.
