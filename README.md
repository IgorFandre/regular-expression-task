# Задача

### Даны регулярное выражение α в обратной польской записи и слово u ∈ {a, b, c}* . Найти длину самого длинного суффикса u, являющегося также суффиксом некоторого слова в L, заданного α.

## Пример работы:

```console
igor@Igorek formal-lang-task % ./bin/main
ab+c.aba.*.bac.+.+* babc   // Ввод
2 : bc   // Вывод
```

## Формат ввода:

Регулярное выражение из алфавита a, b, c и операций
1. (r1 + r2) <=> (L1 ∪ L2)
2. (r1 . r2) <=> (L1 · L2)
3. (r *) <=> (L *)

## Асимптотика алгоритма

Пусть на вход даны регулярное выражение reg длины |reg|, и слово word длины |word|.

Алгоритм решение задачи состоит из двух этапов:

### Первый этап.
Создание недетерминированного конечного автомата из регулярного выражения. Рассмотрим, сколько каждый символ регулярного выражения порождает состояний и переходов:

1. Буква. Создается автомат с двумя вершинами и одним переходом.
2. Сложение. Создается автомат с двумя дополнительными вершинами и четырьмя дополнительными состояниями.
3. Умножение. Создается автомат без выделения дополнительных состояний и с одним дополнительным переходом.
4. Звезда Клини. Создается автомат с одним дополнительным переходом из конца в начало.

Таким образом, |Q| + |E| в получившемся автомате равна O ( |reg| ). Это будем использовать в доказательстве ассимптотики поиска слова в автомате.

### Второй этап.
Поиск слова осуществляется с помощью dfs по графу. Для каждого i ∈ {1, ..., |word|} мы запускаем из всех вершин поиск в глубину и ищем переход по буквам. Также для каждой пройденной буквы обновляем массив used = vector(|Q|, false) и таким образом для поиска прохода для каждой буквы тратим не более O ( |Q| + |E| ) времени, на заходя в возможные циклы по эпсилон-переходам.

Таким образом, мы запускаем |word| раз |Q| = O ( |reg| ) поисков в глубину, значит ассимптотика алгоритма O ( |word| * |reg| * |reg| ).