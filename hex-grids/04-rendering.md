# Отрисовка: Hex ↔ Pixel

Типы: **Point** `{ x, y }`, **Layout** `{ orientation, size: Point, origin: Point }`, **Orientation** (матрицы f0..f3, b0..b3, start_angle), **FractionalHex** `{ q, r, s }` (числа с плавающей точкой).

## Layout (ориентация и масштаб)

```typescript
const Layout = {
  pointy: new Orientation(
    Math.sqrt(3), Math.sqrt(3) / 2, 0, 3 / 2,  // forward matrix
    Math.sqrt(3) / 3, -1 / 3, 0, 2 / 3,        // inverse matrix
    0.5  // start_angle (30° для pointy)
  ),
  flat: new Orientation(
    3 / 2, 0, Math.sqrt(3) / 2, Math.sqrt(3),
    2 / 3, 0, -1 / 3, Math.sqrt(3) / 3,
    0    // start_angle (0° для flat)
  ),
};
```

- **pointy top** — острый угол сверху (как в Civilization)
- **flat top** — плоская грань сверху

---

## Hex → Pixel

```typescript
function hexToPixel(layout: Layout, h: Hex): Point {
  const M = layout.orientation;
  const x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x + layout.origin.x;
  const y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y + layout.origin.y;
  return { x, y };
}
```

- **size** — масштаб (половина высоты для pointy, половина ширины для flat)
- **origin** — смещение центра гекса (0,0)

---

## Pixel → Hex (с округлением)

```typescript
function pixelToHexFractional(layout: Layout, p: Point): FractionalHex {
  const M = layout.orientation;
  const pt = {
    x: (p.x - layout.origin.x) / layout.size.x,
    y: (p.y - layout.origin.y) / layout.size.y,
  };
  const q = M.b0 * pt.x + M.b1 * pt.y;
  const r = M.b2 * pt.x + M.b3 * pt.y;
  return { q, r, s: -q - r };
}

function pixelToHex(layout: Layout, p: Point): Hex {
  return cubeRound(pixelToHexFractional(layout, p));
}
```

---

## Округление к ближайшему гексу (cube_round)

```typescript
function cubeRound(frac: { q: number; r: number; s: number }): Hex {
  let qi = Math.round(frac.q);
  let ri = Math.round(frac.r);
  let si = Math.round(frac.s);

  const q_diff = Math.abs(qi - frac.q);
  const r_diff = Math.abs(ri - frac.r);
  const s_diff = Math.abs(si - frac.s);

  if (q_diff > r_diff && q_diff > s_diff) {
    qi = -ri - si;
  } else if (r_diff > s_diff) {
    ri = -qi - si;
  } else {
    si = -qi - ri;
  }
  return new Hex(qi, ri, si);
}
```

Сбрасываем координату с наибольшей погрешностью, чтобы сохранить `q + r + s = 0`.

---

## Углы гекса (для отрисовки)

```typescript
function hexCornerOffset(layout: Layout, corner: number): Point {
  const angle = 2 * Math.PI * (layout.orientation.start_angle - corner) / 6;
  return {
    x: layout.size.x * Math.cos(angle),
    y: layout.size.y * Math.sin(angle),
  };
}

function polygonCorners(layout: Layout, h: Hex): Point[] {
  const center = hexToPixel(layout, h);
  const corners: Point[] = [];
  for (let i = 0; i < 6; i++) {
    const offset = hexCornerOffset(layout, i);
    corners.push({ x: center.x + offset.x, y: center.y + offset.y });
  }
  return corners;
}
```

---

## Модификации

### Ненулевое начало координат

В конце hex→pixel: `x += origin.x`, `y += origin.y`  
В начале pixel→hex: `x -= origin.x`, `y -= origin.y`

### Разный масштаб по осям (спрайты)

- Pointy top, спрайт W×H: `size = Point(W/√3, H/2)`
- Flat top, спрайт W×H: `size = Point(W/2, H/√3)`

### Инверсия при неоднородном масштабе

При pixel→hex делить на те же scale.x и scale.y, что использовались при hex→pixel.

---

## Doubled → Pixel (упрощённо)

```typescript
// doublewidth (pointy)
function doublewidthToPixel(hex: DoubledCoord, size: number): Point {
  return {
    x: (Math.sqrt(3) / 2) * hex.col * size,
    y: (3 / 2) * hex.row * size,
  };
}

// doubleheight (flat)
function doubleheightToPixel(hex: DoubledCoord, size: number): Point {
  return {
    x: (3 / 2) * hex.col * size,
    y: (Math.sqrt(3) / 2) * hex.row * size,
  };
}
```
