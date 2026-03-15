# Преобразования координат

Типы: `Hex(q, r, s)`, `OffsetCoord(col, row)`, `DoubledCoord(col, row)`. Константы: `OffsetCoord.EVEN = 1`, `OffsetCoord.ODD = -1`.

**Примечание:** для чётности (parity) используйте `(col & 1)` или `(row & 1)` вместо `% 2`, чтобы корректно работали отрицательные координаты (в JS/TS `%` даёт остаток с знаком делимого).

---

## Offset ↔ Cube

### Q-offset (flat top): Odd-Q / Even-Q

```typescript
// Cube → Offset
function qoffsetFromCube(offset: number, h: Hex): OffsetCoord {
  const col = h.q;
  const row = h.r + Math.floor((h.q + offset * (h.q & 1)) / 2);
  return { col, row };
}

// Offset → Cube
function qoffsetToCube(offset: number, h: OffsetCoord): Hex {
  const q = h.col;
  const r = h.row - Math.floor((h.col + offset * (h.col & 1)) / 2);
  return new Hex(q, r, -q - r);
}
```

### R-offset (pointy top): Odd-R / Even-R

```typescript
// Cube → Offset
function roffsetFromCube(offset: number, h: Hex): OffsetCoord {
  const col = h.q + Math.floor((h.r + offset * (h.r & 1)) / 2);
  const row = h.r;
  return { col, row };
}

// Offset → Cube
function roffsetToCube(offset: number, h: OffsetCoord): Hex {
  const q = h.col - Math.floor((h.row + offset * (h.row & 1)) / 2);
  const r = h.row;
  return new Hex(q, r, -q - r);
}
```

**offset:** `EVEN = +1` или `ODD = -1`

---

## Doubled ↔ Cube

### Double-width (pointy top)

```typescript
// Cube → Doubled
function qdoubledFromCube(h: Hex): DoubledCoord {
  return { col: h.q, row: 2 * h.r + h.q };
}

// Doubled → Cube
function qdoubledToCube(h: DoubledCoord): Hex {
  const q = h.col;
  const r = (h.row - h.col) / 2;
  return new Hex(q, r, -q - r);
}
```

### Double-height (flat top)

```typescript
// Cube → Doubled
function rdoubledFromCube(h: Hex): DoubledCoord {
  return { col: 2 * h.q + h.r, row: h.r };
}

// Doubled → Cube
function rdoubledToCube(h: DoubledCoord): Hex {
  const q = (h.col - h.row) / 2;
  const r = h.row;
  return new Hex(q, r, -q - r);
}
```

---

## Axial ↔ Cube

Axial — это (q, r), Cube — (q, r, s) с s = -q - r.

```typescript
// Axial → Cube
function axialToCube(q: number, r: number): Hex {
  return new Hex(q, r, -q - r);
}

// Cube → Axial: у Hex берём .q и .r; s = -q - r
```

---

## Offset: соседи

У offset соседи зависят от чётности строки/столбца — нужны две таблицы (odd/even). См. [03-algorithms](./03-algorithms.md). Удобнее конвертировать в cube, вызвать neighbor, конвертировать обратно.
