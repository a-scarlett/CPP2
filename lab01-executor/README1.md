# Лабораторная работа №1 (executor)

## Содержание

1. [Содержание](#содержание)
2. [Задание](#задание)
   1. [Советы](#советы)
3. [Требования к корректности решения](#требования-к-корректности-решения)
   1. [Базовые требования](#базовые-требования)
   2. [Дополнительные требования](#дополнительные-требования)
   3. [Цитатник](#цитатник)
   4. [Аккуратное чтение](#аккуратное-чтение)
   5. [Бродилка](#бродилка)
   6. [Стенки](#стенки)
4. [Инструкция по сдаче](#инструкция-по-сдаче)
5. [Система оценки](#система-оценки)
6. [Сроки сдачи](#сроки-сдачи)

## Задание
В этом задании вам надо написать программу, читающую команды с ввода и реагирующую на них. Каждая
команда представляет собой отдельную строку: программа читает их и разбирает.

Это задание состоит из четырёх частей, вы можете выполнить только префикс и [получить частичные
баллы](#система-оценки). В каждой части вы расширяете набор поддерживаемых команд. Решением является
один файл `executor.cpp`, каждая следующая часть расширяет предыдущие. У команд могут быть
псевдонимы.

Обзор частей (подробные требования ниже):

1. **Цитатник**.
   По командам `sherlock-holmes` и `cat-jeoffry` выведите на экран отрывок из Шерлока Холмса и
   Jubilate Agno, соответственно. В точности сохраните все пробельные символы, включая переводы
   строк. По команде `quit` успешно завершите интерпретатор. В этой части гарантируется, что входные
   данные корректны.
2. **Аккуратное чтение**.
   Входные данные теперь — произвольная последовательность байт с кодами от 1 до 127 включительно.
   Каждая команда должна располагаться на отдельной строчке, могут быть дописаны пробельные символы
   в начале или в конце.
   Некорректные команды должны приводить к сообщению `Invalid command: '<команда>'`.
3. **Бродилка**.
   Проэмулируйте бродилку по бесконечной квадратной сетке: исходно вы находитесь в клетке `(0, 0)` и
   смотрите наверх. Команды позволяют ходить вперёд, назад, поворачиваться на 90 градусов налево и
   направо, а также показывать исследованную карту при помощи [ASCII графики][wiki: ASCII-графика].
   Также должна поддерживаться запись произвольной последовательности команд, чтобы её потом можно
   было просто выполнить.
4. **Стенки**.
   Добавьте две новые команды: добавление и удаление стенки перед игроком.
   Если игрок пытается пройти сквозь стену, выводится сообщение `Bump!`.

Точный формат смотрите в тестах, а семантику команд — в требованиях ниже.

### Советы
* Выполняйте части сверху вниз.
  Удалите из `tests.txt` тесты, которые вы пока что не собираетесь проходить.
* Никогда не используйте `while (cin)` или `while (cin.eof())` для проверки конца файла.
  Сначала попробуйте прочитать, а потом проверяйте успешность последнего чтения, как показано
  [тут](https://notes.algoprog.ru/cpp/syntax.html#id12).
* Используйте `std::getline` для построчного чтения.
* Для крупных типов (вроде `std::string`) используйте передачу по константной ссылке, где разумно.
* Вместо десятка разрозненных констант используйте `enum class`
  ([scoped enumeration][cppreference: enum class]).
* Добавьте свои собственные тесты в `tests.txt` и запускайте их на сервере.
* Отдельные функции удобно тестировать в начале `int main()` при помощи `assert`.
* Заведите константные массивы, в которых будет храниться ожидаемый сдвиг для каждого из четырёх
  направлений, например:
  ```c++
  const int dx[] = { 1, 0, -1, 0 };
  const int dy[] = { 0, 1, 0, -1 };
  int x = 10, y = 10, direction = 2;
  x += dx[direction];
  y += dy[direction];
  ```
* Вам наверняка пригодятся `std::set`, `std::map` и `std::pair`.
* `clang-tidy` может ругаться на глобальные переменные.
  В общем случае их плохо использовать, но в этом задании разумно, так что можно заглушить
  соответствующие предупреждения.
* Если вы скачаете себе ZIP-архив с репозиторием на Windows, то в файлах с ожидаемыми ответами будут
  использоваться Linux переводы строк — один символ `\n`.
  А под Windows ожидается свой перевод строк (два символа: `\r\n`), это автоматически обрабатывается
  стандартной библиотекой: вы выводите `\n`, а она преобразует это в `\r\n`. Также за вас по
  умолчанию починит переводы и Git. Так как редакторы обычно подстраиваются под стиль переводов
  строк, ответом и ваш вывод будет выглядеть одинаково, но команда `diff` и локальные тесты будут
  считать, что они отличаются во всех строчках. Это чинится преобразованием переводов строк во всех
  файлах при помощи команды `unix2dos`, которую можно поставить в msys2 командой
  `pacman -S dos2unix`.

## Требования к корректности решения
### Базовые требования
Действуют [стандартные требования](../common/common-requirements.md).

Для `cppcheck` убран флаг `--suppress=unusedFunction`.

### Дополнительные требования
Отсутствуют.

### Цитатник
Чтение команд идёт со стандартного потока ввода (`std::cin`), вывод всех результатов — в стандартный
поток вывода (`std::cout`).
Каждая команда располагается на отдельной строке.
Символов, кроме команд и переводов строки, нет.

Поддерживаемые команды:

| Псевдонимы                    | Поведение                                      |
|-------------------------------|------------------------------------------------|
| `quit`, `exit`                | Немедленное успешное завершение интерпретатора |
| `sherlock`, `sherlock-holmes` | Вывод на экран цитаты из Шерлока Холмса        |
| `cat`, `cat-jeoffry`          | Вывод на экран цитаты из Jubilate Agno         |

Цитата из Шерлока Холмса:

```
I had had so many reasons to believe in my friend's subtle powers of
reasoning and extraordinary energy in action that I felt that he must
have some solid grounds for the assured and easy demeanour with which
he treated the singular mystery which he had been called upon to
fathom. Once only had I known him to fail, in the case of the King of
Bohemia and of the Irene Adler photograph; but when I looked back to
the weird business of the Sign of Four, and the extraordinary
circumstances connected with the Study in Scarlet, I felt that it would
be a strange tangle indeed which he could not unravel.
```

Цитата из Jubilate Agno:
```
For he is of the tribe of Tiger.
For the Cherub Cat is a term of the Angel Tiger.
For he has the subtlety and hissing of a serpent, which in goodness he suppresses.
For he will not do destruction, if he is well-fed, neither will he spit without provocation.
For he purrs in thankfulness, when God tells him he's a good Cat.
For he is an instrument for the children to learn benevolence upon.
For every house is incomplete without him and a blessing is lacking in the spirit.
For the Lord commanded Moses concerning the cats at the departure of the Children of Israel from Egypt.
For every family had one cat at least in the bag.
For the English Cats are the best in Europe.
```

Первую цитату следует сохранить в программе при помощи одного или нескольких
строковых литералов: `"Hello"`.
Вторую цитату следует сохранить в программе при помощи сырых (raw)
[строковых литералов][cppreference: string literals]: `R"(Hello)"`.

### Аккуратное чтение
Определим формально формат ввода:

1. Ввод разделяется на строчки по стандартным конвенциям POSIX: строчка (line) — это
   последовательность байт (возможно, пустая), завершённая ровно одним символом перевода строки
   (`\n`, код 10).
   Также может быть одна непустая строка (incomplete line) в конце ввода: непустая
   последовательность байт, после которой вместо `\n` идёт конец ввода.
   * Обработка ОС-специфичных переводов строки (`\r\n` под Windows, `\r` под старыми Mac)
     выполняется стандартной библиотекой языка C++, вам не нужно про это думать.
2. В каждой строке может находиться одно из двух:
    * Команда, возможно, с добавлением пробельных символов (whitespace characters) до или после неё.
      В таких строках интерпретатор выполняет соответствующую команду.
    * Любая другая последовательность байт.
      В таких строках интерпретатор выдаёт сообщение `Invalid command: '<последовательность-байт>'`.

В частности, строки могут быть пустыми или состоять только из пробельных символов.
Они вызывают такое же сообщение об ошибке, как и остальные, начинающееся с `Invalid command`.

Дополнительно вам требуется выделить обработку строки в отдельную функцию `parse_line`,
которую можно протестировать независимо от поведения конкретных команд при помощи `assert`.
Напишите так хотя бы три теста на эту функцию.

### Бродилка
Поддерживаемые команды:

| Псевдонимы            | Поведение                                                 |
|-----------------------|-----------------------------------------------------------|
| `forward`, `fwd`      | Пройти на одну клетку вперёд                              |
| `backward`, `bwd`     | Пройти на одну клетку назад                               |
| `turn-right`, `right` | Повернуться на 90 градусов направо, по часовой стрелке    |
| `turn-left`, `left`   | Повернуться на 90 градусов налево, против часовой стрелки |
| `show-map`            | Выводит на экран текущую карту                            |
| `start-macro`         | Начинает запись макроса (смотри ниже)                     |
| `end-macro`           | Заканчивает запись макроса (смотри ниже)                  |
| `run-macro`           | Запускает записанный ранее макрос                         |

#### Формат вывода карты
Найдём минимальный прямоугольник с осями, параллельными осям координат, содержащий все посещённые
игроком клетки. Пусть он имеет размер `W` клеток в ширину и `H` клеток в высоту.

Тогда карта выводится как `2*H+1` строка, содержащая `2*W+1` символ каждая.
Чётные символы в чётных строках (при нумерации с единицы) соответствуют клеткам:

* `.` — клетка ещё не была посещена игроком;
* `o` — клетка была посещена игроком;
* `^`/`>`/`v`/`<` — игрок сейчас находится в этой клетке и смотрит наверх/направо/вниз/налево.

Остальные символы в этом задании должны быть пробельными.
В следующем задании там появятся стенки между клетками.

#### Макрос
Интерпретатор помнит ровно один макрос — последний записанный.
Исходно макрос пустой.

При записи макроса команды продолжают читаться так же, как и раньше, но вместо выполнения они
записываются в макрос и выводят на экран сообщение `Recorded`.
В макрос можно записать только команды `forward`, `backward`, `turn-right`, `turn-left`, `show-map`
и их псевдонимы. Остальные команды вызывают сообщение `Invalid command:` и игнорируются, даже если
они бы были корректны вне макроса.

Запись макроса заканчивается командой `end-macro` или концом файла, после чего на экран выводится
сообщение `New macro of X command(s)`, где `X` — количество команд в макросе.

В обычном режиме можно запустить макрос командой `run-macro`.
Она выполняется так же, как если бы команды из макроса ввели вручную.

### Стенки
Добавьте ещё две команды, в том числе в макросы:

| Псевдонимы    | Поведение                                                            |
|---------------|----------------------------------------------------------------------|
| `put-wall`    | Поставить стенку перед игроком, если она уже есть — ничего не делать |
| `remove-wall` | Удалить стенку перед игроком, если её нет — ничего не делать         |

Стенки ставятся между клетками.
Каждая стенка загораживает проход между двумя соседними клетками.
Если игрок пытается пройти сквозь стену, то у него это не должно получиться и интерпретатор выводит
сообщение `Bump!`.

Стены также должны выводиться на карте.
Для вертикальных стен между клетками используйте `|`, для горизонтальных — `-` (дефис).
В углах клетки ставьте `+` тогда и только когда, когда у этого угла есть хотя бы одна смежная стена.

## Инструкция по сдаче
[Схема стандартная](../common/common-requirements.md#формат-сдачи): вы должны выполнить
задание в отдельной ветке и создать Pull Request.
В этом задании должно хватить веб-интерфейса GitHub, если возникают проблемы — попросите практика
помочь.

Если вы хотите сдать только некоторые части задания, удалите остальные тесты из `tests.txt`.

## Система оценки
* Задание оценивается в 10 баллов: 6 за корректность, 4 за стиль.
  * Если не проходит хотя бы одна автоматическая проверка или неверно назван PR, вы получаете ноль.
  * Вы не можете получить за стиль больше баллов, чем за корректность.
* Вы можете сдать любой префикс частей:
  * Только [цитатник](#цитатник) оценивается в 1/10 баллов: 0.5 за корректность, 0.5 за стиль.
  * Если добавить [аккуратное чтение](#аккуратное-чтение), можно получить до 3/10 баллов: 1.5 за
    корректность, 1.5 за стиль.
  * Если добавить [бродилку](#бродилка), можно получить до 7/10 баллов: 4 за корректность, 3 за
    стиль.
  * Если добавить [стенки](#стенки), можно получить до 10/10 баллов: 6 за корректность, 4 за стиль.
  * Так как части взаимосвязаны, баллы на стиль могут распределяться между частями нечётко.
    Например, добавление бродилки может случайно испортить стиль аккуратного чтения.

## Сроки сдачи
Задание выдано 24 января 2023 (вторник).
Ниже в каждом случае указано кипрское время (_Asia/Nicosia_).

* **Дедлайн сдачи:** 07 февраля 2023 (вторник), **22**:59.
* Ожидаемый срок проверки: 14 февраля 2023 (вторник).
* Если первая попытка сдачи достаточно разумна (на усмотрение принимающего),
  то вы можете сделать ещё попытку исправлений или даже несколько.
  * Предложение актуально даже если вы получили автоматический ноль за первую попытку.
  * Выставляется максимум из всех попыток.
* **Срок исправлений**: 21 февраля 2023 (вторник), **22**:59.

[wiki: ASCII-графика]: https://ru.wikipedia.org/wiki/ASCII-графика
[cppreference: enum class]: https://en.cppreference.com/w/cpp/language/enum
[cppreference: string literals]: https://en.cppreference.com/w/cpp/language/string_literal