# Справочник по гексагональным картам

**Краткий справочник** по формулам и коду для гексагональных сеток. Составлен по материалам [Red Blob Games — Hexagonal Grids](https://www.redblobgames.com/grids/hexagons/).

Оригинальный гайд на сайте значительно больше (интерактивные схемы, вывод формул, варианты под разные языки). Здесь — сжатая выжимка: системы координат, преобразования, основные алгоритмы и готовый TypeScript. Для углубления в тему и наглядности всегда стоит открывать [оригинал](https://www.redblobgames.com/grids/hexagons/).

---

## Содержание по файлам

| Файл | Темы |
|------|------|
| [00-geometry.md](./00-geometry.md) | Geometry: spacing, angles, связь с Layout |
| [01-coordinate-systems.md](./01-coordinate-systems.md) | Coordinate systems: offset, cube, axial, doubled |
| [02-conversions.md](./02-conversions.md) | Conversions между системами, типы, parity |
| [03-algorithms.md](./03-algorithms.md) | Neighbors, diagonals, line drawing, distances, range, rotation, reflection, rings, field of view, pathfinding |
| [04-rendering.md](./04-rendering.md) | Hex to pixel, pixel to hex, rounding, углы гекса |
| [05-implementation.md](./05-implementation.md) | TypeScript: Hex, Point, Offset, Layout, формы карт, wraparound |
| [06-map-storage.md](./06-map-storage.md) | Map storage: hash, 2D-массив, индексация по формам |

## Соответствие разделам сайта

| Сайт (Red Blob Games) | Справочник |
|------------------------|------------|
| Geometry | 00 |
| Coordinate systems | 01 |
| Conversions | 02 |
| Neighbors, Distances, Line drawing, Range, Rotation, Reflection, Rings and Spirals, Field of view, Pathfinding | 03 |
| Hex to pixel, Pixel to hex, Rounding | 04 |
| Map storage, Wraparound maps | 05, 06 |

---

## Рекомендации по выбору системы

| Критерий | Offset | Doubled | Axial | Cube |
|----------|--------|---------|-------|------|
| Pointy top | evenr, oddr | doublewidth | axial | cube |
| Flat top | evenq, oddq | doubleheight | — | — |
| Векторные операции (+, −, ×) | нет | да | да | да |
| Хранение в массиве | прямоугольник | нет* | ромб* | нет* |
| Хранение в hash | любая форма | любая форма | — | — |
| Гексагональная симметрия | нет | нет | нет | да |
| Простота алгоритмов | мало | средне | большинство | большинство |

**Рекомендация:** для прямоугольных карт — **Doubled** или **Offset**; для карт любой формы — **Axial/Cube**. Axial (q,r) и Cube (q,r,s) по сути одна система: храните как Axial, вычисляйте s при необходимости.

---

## Термины

- **Pointy top / flat top** — ориентация гекса (остриём вверх или гранью вверх).
- **size** — расстояние от центра гекса до вершины (масштаб отрисовки).
- **q, r, s** — кубические/осевые координаты (q + r + s = 0).
- **Offset EVEN / ODD** — вариант смещения строк или столбцов (+1 или -1 в формулах).

---

## Порядок чтения

1. **Геометрия и координаты:** 00 → 01 → 02  
2. **Алгоритмы:** 03  
3. **Отрисовка:** 04  
4. **Код и хранение:** 05 → 06  

---

## Где смотреть подробнее

| Нужно | Ссылка |
|-------|--------|
| Визуализация, схемы, вывод формул | [Hexagonal Grids](https://www.redblobgames.com/grids/hexagons/) — по разделам (Geometry, Neighbors, Distances и т.д.) |
| Готовый код на других языках | [Implementation Guide](https://www.redblobgames.com/grids/hexagons/implementation.html) |
| A* и графы | [Pathfinding](https://www.redblobgames.com/pathfinding/a-star/introduction.html) |
| Поле зрения (LOS) | [Clark Verbrugge — Hex LOS](http://www-cs-students.stanford.edu/~amitp/Articles/HexLOS.html) |
