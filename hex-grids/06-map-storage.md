# Хранение карты (Map storage)

Три подхода: **hash-таблица** (гибко), **массив массивов** (компактно для правильных форм), **граф** (рёбра заданы явно).

---

## Hash-таблица

Подходит для любой формы карты, дыр и произвольных границ.

```typescript
const map = new Map<string, TileData>();
const key = (h: Hex) => `${h.q},${h.r}`;
map.set(key(new Hex(1, -2, 1)), { terrain: "grass" });
if (map.has(key(hex))) return map.get(key(hex));
```

Доступ к соседям — через `hexNeighbor(hex, dir)`; граница — проверка `map.has(key(neighbor))`.

---

## Индексация в 2D-массиве (axial)

Идея: один цикл по форме даёт пары (a, b); храним в `array[a - a1][b - b1]`.

### Прямоугольник (pointy top)

Границы: left, right, top, bottom в offset-смысле.

```typescript
// Индекс: array[r - top][q - left + (r >> 1)]
// Размер: (bottom - top + 1) × (right - left + 1)
function getAt(map: TileData[][], q: number, r: number, left: number, top: number): TileData {
  return map[r - top][q - left + (r >> 1)];
}
```

### Прямоугольник (flat top)

```typescript
// Индекс: array[q - left][r - top + (q >> 1)]
function getAt(map: TileData[][], q: number, r: number, left: number, top: number): TileData {
  return map[q - left][r - top + (q >> 1)];
}
```

### Ромб (параллелограмм)

`first_row = 0`, `first_column(r) = 0`. Храним `Hex(q, r)` в `array[r][q]`. Размер строки одинаков.

### Шестиугольник радиуса N

При нумерации строк r от 0 до 2N (центр в N): длина строки r равна `2*N+1 - |N-r|`, сдвиг по q: `q - max(0, N - r)`.

```typescript
// array[r][q - Math.max(0, N - r)], размер строки r: 2*N+1 - Math.abs(N - r)
```

### Треугольник (нижний)

Строка r имеет размер `N + 1 - r`. Индекс: `array[r][q]`.

### Треугольник (верхний)

Индекс: `array[r][q - (N + 1 - r)]`, размер строки `r + 1`.

---

## Рекомендации

- **Произвольная форма, дыры, редактор** → hash.
- **Фиксированная прямоугольная/гексагональная карта, минимум памяти** → 2D-массив с формулами выше.
- **Сложные границы, стены** → граф (узлы + список соседей) или hash + проверка проходимости по данным тайла.

Доступ к карте лучше обернуть в класс/функции `get(h)`, `set(h, data)`, чтобы остальной код не зависел от выбора хранения.
