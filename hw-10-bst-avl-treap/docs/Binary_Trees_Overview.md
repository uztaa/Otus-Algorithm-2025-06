# Двоичные деревья поиска

> **ДЗ-10**: Двоичное дерево поиска, АВЛ и Декартово дерево  
> **Курс**: Алгоритмы и структуры данных

---

## 1. Двоичное дерево поиска (BST)

### 1.1 Определение

**Двоичное дерево поиска (Binary Search Tree)** — структура данных, где для каждого узла:
- Все ключи в левом поддереве **меньше** ключа узла
- Все ключи в правом поддереве **больше** ключа узла
```
        8
       / \
      3   10
     / \    \
    1   6    14
       / \   /
      4   7 13
```

### 1.2 Операции

| Операция | Средний случай | Худший случай |
|----------|----------------|---------------|
| search   | O(log n)       | O(n)          |
| insert   | O(log n)       | O(n)          |
| remove   | O(log n)       | O(n)          |

### 1.3 Алгоритм вставки
```
insert(node, key):
    if node == null:
        return new Node(key)
    if key < node.key:
        node.left = insert(node.left, key)
    else if key > node.key:
        node.right = insert(node.right, key)
    return node
```

### 1.4 Алгоритм удаления

Три случая:
1. **Лист** — просто удаляем
2. **Один потомок** — заменяем узел потомком
3. **Два потомка** — находим минимум в правом поддереве (или максимум в левом), копируем значение, удаляем найденный узел

### 1.5 Проблема вырождения

При вставке отсортированных данных BST вырождается в связный список:
```
1 → 2 → 3 → 4 → 5   (высота = n)
```

Все операции становятся O(n).

---

## 2. АВЛ-дерево

### 2.1 Определение

**АВЛ-дерево** — самобалансирующееся BST, где для каждого узла:
- |height(left) - height(right)| ≤ 1

**Balance factor** = height(right) - height(left)

Допустимые значения: {-1, 0, +1}

### 2.2 Вращения

#### Малое левое вращение (LL)

Применяется при balance = +2 и правый потомок имеет balance ≥ 0
```
    A (+2)              B (0)
     \                 / \
      B (+1)    →     A   C
       \
        C
```
```
smallLeftRotation(a):
    b = a.right
    a.right = b.left
    b.left = a
    updateHeight(a)
    updateHeight(b)
    return b
```

#### Малое правое вращение (RR)

Применяется при balance = -2 и левый потомок имеет balance ≤ 0
```
        C (-2)          B (0)
       /               / \
      B (-1)    →     A   C
     /
    A
```

#### Большое левое вращение (LR → LL)

Применяется при balance = +2 и правый потомок имеет balance < 0
```
    A (+2)          A (+2)           B
     \               \              / \
      C (-1)   →      B      →     A   C
     /                 \
    B                   C
```
```
bigLeftRotation(a):
    a.right = smallRightRotation(a.right)
    return smallLeftRotation(a)
```

#### Большое правое вращение (RL → RR)

Применяется при balance = -2 и левый потомок имеет balance > 0

### 2.3 Перебалансировка
```
rebalance(node):
    updateHeight(node)
    balance = getBalance(node)
    
    if balance == +2:
        if getBalance(node.right) < 0:
            return bigLeftRotation(node)
        else:
            return smallLeftRotation(node)
    
    if balance == -2:
        if getBalance(node.left) > 0:
            return bigRightRotation(node)
        else:
            return smallRightRotation(node)
    
    return node
```

### 2.4 Сложность операций

| Операция | Время     | Примечание |
|----------|-----------|------------|
| search   | O(log n)  | Гарантированно |
| insert   | O(log n)  | + до 2 вращений |
| remove   | O(log n)  | + до O(log n) вращений |

---

## 3. Декартово дерево (Treap)

### 3.1 Определение

**Декартово дерево** — комбинация BST и кучи (heap):
- По ключу (x) — свойство BST
- По приоритету (y) — свойство кучи (max-heap)
```
        (3, 9)           ← key=3, priority=9
        /    \
    (1, 7)  (5, 8)
      \        \
    (2, 4)   (7, 5)
```

### 3.2 Ключевые операции

#### Split (разделение)

Разделяет дерево на два: L (все ключи < x) и R (все ключи ≥ x)
```
split(t, x) → (L, R):
    if t == null:
        return (null, null)
    
    if t.key < x:
        (t.right, R) = split(t.right, x)
        return (t, R)
    else:
        (L, t.left) = split(t.left, x)
        return (L, t)
```

#### Merge (слияние)

Объединяет два дерева (все ключи L < все ключи R)
```
merge(L, R) → T:
    if L == null: return R
    if R == null: return L
    
    if L.priority > R.priority:
        L.right = merge(L.right, R)
        return L
    else:
        R.left = merge(L, R.left)
        return R
```

### 3.3 Вставка и удаление через split/merge
```
insert(t, key, priority):
    (L, R) = split(t, key)
    newNode = Node(key, priority)
    return merge(merge(L, newNode), R)

remove(t, key):
    (L, temp) = split(t, key)
    (_, R) = split(temp, key + 1)  // или найти и удалить узел
    return merge(L, R)
```

### 3.4 Сложность операций

| Операция | Ожидаемое время |
|----------|-----------------|
| search   | O(log n)        |
| insert   | O(log n)        |
| remove   | O(log n)        |
| split    | O(log n)        |
| merge    | O(log n)        |

При **случайных приоритетах** дерево остаётся сбалансированным с высокой вероятностью.

---

## 4. Сравнение деревьев

| Критерий | BST | AVL | Treap |
|----------|-----|-----|-------|
| Поиск (avg) | O(log n) | O(log n) | O(log n) |
| Поиск (worst) | O(n) | O(log n) | O(n)* |
| Вставка | O(log n) / O(n) | O(log n) | O(log n) |
| Удаление | O(log n) / O(n) | O(log n) | O(log n) |
| Балансировка | Нет | Да (вращения) | Вероятностная |
| Сложность реализации | Простая | Средняя | Средняя |
| Split/Merge | Сложно | Сложно | Просто |

*Treap имеет O(n) в худшем случае теоретически, но при случайных приоритетах практически всегда O(log n).

---

## 5. Применение

| Структура | Когда использовать |
|-----------|-------------------|
| **BST** | Учебные цели, данные приходят в случайном порядке |
| **AVL** | Нужна гарантированная производительность, много поисков |
| **Treap** | Нужны split/merge, интервальные запросы, персистентность |

---

## 6. Ссылки

- [Binary Search Tree — Wikipedia](https://en.wikipedia.org/wiki/Binary_search_tree)
- [AVL Tree — Wikipedia](https://en.wikipedia.org/wiki/AVL_tree)
- [Treap — Wikipedia](https://en.wikipedia.org/wiki/Treap)
- Кормен и др. "Алгоритмы: построение и анализ", глава 12-13