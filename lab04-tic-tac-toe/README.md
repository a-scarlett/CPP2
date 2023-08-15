# Лабораторная работа №4 (Tic Tac Toe)

## Содержание

1. [Содержание](#содержание)
2. [Задание](#задание)
   1. [Архитектура приложения](#архитектура-приложения)
   2. [Советы](#советы)
3. [Требования к корректности решения](#требования-к-корректности-решения)
   1. [Базовые требования](#базовые-требования)
   2. [Дополнительные требования](#дополнительные-требования)
   3. [Правила игры](#правила-игры)
   4. [Консольное приложение](#консольное-приложение)
   5. [Компактное отображение](#компактное-отображение)
   6. [Красивое отображение](#красивое-отображение)
   7. [Частичные решения](#частичные-решения)
4. [Инструкция по сдаче](#инструкция-по-сдаче)
5. [Система оценки](#система-оценки)
6. [Сроки сдачи](#сроки-сдачи)

## Задание

Реализуйте консольное приложение-игру крестики-нолики 10 на 10 (5 в ряд) для двух игроков. Один
запуск приложения соответствует одной игре. В процессе работы можно переключать режим отображения
поля/ввода команд. Их может быть сколько угодно, но вам надо реализовать два: один «компактный»,
другой «красивый», каждому соответствует отдельная разделяемая библиотека.

Проектируйте приложение так, чтобы можно было добавлять новые режимы без перекомпиляции основного
исполняемого файла, только добавлением новых разделяемых библиотек.

### Архитектура приложения

В архитектуре должны быть явно выделены классы «модель игры» и «отображение» (model-view); первый
отвечает за всю логику игры, второй — за конкретный формат ввода-вывода.

Вся логика игры в крестики-нолики должна по максимуму находиться в "модели" в классе
`tictactoe::Game`: хранение поля, совершение ходов, определение текущего игрока, победителя,
корректности хода… Тогда можно теоретически написать много тестов, которые тестируют модель
напрямую, без запуска приложения, без привязки к конкретному формату отображения, и в хитрых
ситуациях. Такую модель должно быть можно без изменений использовать в схожих программах для игры в
крестики-нолики.

Отображение `tictactoe::View` — это то, что выносится в разделяемую библиотеку. Скорее всего, сам
`View` — абстрактный класс, а в разделяемых библиотеках определяются наследники. Этому коду может
потребоваться использовать модель, это нормально. Но вот модель от отображения зависеть не должна:
модель одна, отображений много разных, и они могут добавляться.

Какая-то логика может не попасть ни туда, ни туда: например, чтение команд из `cin` в цикле. Её
абсолютно нормально оставить вне model-view.

### Советы

* Заведите `enum class OptionalPlayer`, хранящий в себе одно из трёх значений: ничего, крестики,
  нолики.
* Не храните поле как набор строк из символов `X`, `O`, `.` — это:
  * Завязка на формат вывода, а формат вывода бывает разный.
  * Не даёт никаких гарантий на этапе компиляции на содержимое этих строк: можно случайно
    опечататься.
* Аккуратно подумайте, как вы выделите класс «отображение» и вынесете его в разделяемую библиотеку.
  Прочитайте все части задания, чтобы потом не пришлось переделывать.
* Пишите побольше тестов, можно с использованием `doctest.h` и добавить отдельные единицы
  трансляции.
  * Авторское решение выявляет победителя за ~25 строк кода без дублирования кода копированием.
  * Если возникают проблемы с автопроверками из-за единиц трансляции — попросите помощи.
* Корректно считать два числа из строки со случайными данными — это сложно, там много крайних
  случаев. Но делается при помощи стандартных `<iostream>` и `<sstream>`. Внимательно
  поэкспериментируйте с тем, как это работает.
  * Например, если при помощи `std::stringstream` из строчки `1234foo` прочитать `int`, то
    прочитается число `1234` — максимальный префикс-число.
  * При этом `4 4foo` не является корректным ходом, равно как и `1row1column`, а вот `0 -0` —
    является корректным ходом в компактном отображении.
* Если вы забыли добавить единицу трансляции в разделяемую библиотеку, то на Linux ошибка линковки
  может возникнуть не при компиляции, а при _загрузке_ этой библиотеки.
* При компиляции Apple Clang под macOS _undefined sanitizer_ может считать, что `abstract_plugin` в
  исполняемом файле и в библиотеке — это два разных класса, и ругаться, что указатель на
  `abstract_plugin` указывает не на наследника `abstract_plugin`, а на какого-то `plugin_b`:
  ```
  /Users/user/.../main.cpp:18:38: runtime error: member call on address 0x000104ba4060 which does not point to an object of type 'abstract_plugin'
  0x000104ba4060: note: object is of type 'plugin_b'
   00 00 00 00  40 00 ba 04 01 00 00 00  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  00 00 00 00
                ^~~~~~~~~~~~~~~~~~~~~~~
                vptr for 'plugin_b'
  SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior /Users/user/exercises/10-211122/problem/03-boost-dll/main.cpp:18:38 in
  ```
  Попробуйте добавить `BOOST_SYMBOL_VISIBLE` ко всех нужным классам (особенно `abstract_plugin`).
  Это может не помочь — вероятно, либо вам не всё рассказали про Boost.DLL и dynlib, либо баг в
  Boost.DLL, либо баг в UBSan. Тогда отключите UBSan (убрав `-fsanitize=undefined`) или конкретно
  одну проверку флагом `-fno-sanitize=vptr`.
* У CLion под Windows [есть баг][jb-yt-cpp-17579]: если строчка в выводе не завершилась переводом,
  то на вкладке Run пробельные символы в конце выводятся не сразу. Например, это будет видно при
  выводе приглашения вроде `X move: `. При работе через терминал (неважно, встроенный в CLion или
  отдельный) всё должно работать.
* Будьте осторожны с пробелами в конце строк в тестовых файлах:
  * Их не видно в выводе и в редакторах. Надо придумать, как увидеть: выделить строчку,
    перенаправить в файл и открыть в специальном режиме, поставить курсор в строку и нажать End…
  * Некоторые редакторы (вроде CLion) могут при сохранении автоматически удалять пробелы в конце
    всех или некоторых строк.
* Предупреждение `bugprone-exception-escape` от clang-tidy можно просто подавить. Через
  `NOLINTNEXTLINE` могут быть проблемы, а вот `NOLINT` на одной строчке с `int main` должен
  сработать.
* Предупреждение `cppcoreguidelines-special-member-functions` требует дописать не только деструктор,
  но и четыре других "магических" метода. Посмотрите в предыдущих заданиях, у кого мы писали
  `= delete;` (два конструктора и два `operator=`).

## Требования к корректности решения

### Базовые требования

Действуют [стандартные требования](../common/common-requirements.md), но:

* Максимальное суммарное количество строк (включая пустые) в файлах `.hpp` и `.cpp` — 600.
  Полное авторское решение занимает 365.
* Тестирования с `-stdlib=libc++` нет, потому что Boost под Ubuntu собран с `-stdlib=libstdc++`.
* Сборка происходит не напрямую, а через CMake.

Файлы `.cpp` должны лежать в папке `src/`, файлы `.hpp` должны лежать в папке `include/`.
Запрещается упоминать имена этих папок в исходном коде, требуется корректно настроить пути поиска
компилятора, чтобы работало `#include "game.hpp"`.

Если используете forward-заголовки, то в файле `foo.hpp` находятся определения классов и объявления
функций, в файле `foo_fwd.hpp` — forward declaration классов. Определения `enum` — на ваше
усмотрение, но в `_fwd` они могут оказаться полезнее.

Вы можете редактировать `CMakeLists.txt` и изменять набор единиц трансляции и файлов как угодно, но
в итоге должны получиться как минимум три артефакта:

* Приложение `tic-tac-toe`.
  Внутри не должно быть никаких конкретных отображений.
* Разделяемая библиотека `compact` с компактным отображением.
* Разделяемая библиотека `pretty` с красивым отображением.

Если хотите, вы также можете положить в репозиторий автотесты и `doctest.h`.

### Дополнительные требования

* В каждой библиотеке и исполняемом файле должно содержаться по минимуму объектных файлов и
  зависимостей.
* При возможности используйте и пишите forward-заголовки.
* Весь код должен по максимуму лежать внутри `namespace tictactoe`.
* Используйте unnamed namespaces, чтобы не экспортировать из единиц трансляции что-то ненужное.
* Используйте `BOOST_SYMBOL_VISIBLE` на любых сущностях (классах, функциях, переменных), которые
  должны быть доступны и приложению, и разделяемой библиотеке. Например, на абстрактном классе
  `View` (при его наличии).
* В любой момент времени должно быть загружено ровно одно представление — текущее.

### Правила игры

Игра ведется на игровом поле — квадрат 10 на 10 клеток.
Один игрок играет крестиками, второй — ноликами.

Первыми начинают **нолики**. Игроки ходят по очереди. В свой ход игрок может поставить на свободную
клетку свой знак.

Целью игры является выставить 5 своих знаков в ряд по вертикали, горизонтали или диагонали.
Выигрывает тот игрок, который сделает это первым. Если свободных клеток на игровом поле не осталось,
то назначается ничья.

### Консольное приложение

При запуске приложение сразу начинает единственную игру с управлением через консоль.
Исходно используется отображение из разделяемой библиотеки, имя которой (без ОС-специфичных
украшений) передано как единственный аргумент, например: `./tic-tac-toe view-compact`.

Все разделяемые библиотеки лежат в текущей папке со стандартными украшениями, которые выберет CMake.
В коде можете использовать `boost::dll::load_mode::append_decorations`.

Исходно поле пустое.
В начале каждого хода выводится игровое поле и приглашение: `X move:&nbsp;` — для крестиков и
`O move:&nbsp;` — для ноликов (нолики обозначаются латинской буквой «O»). Обратите внимание на
пробел после двоеточия.

В ответ на приглашение пользователь должен ввести команду CMD, завершённую переводом строки:

* Если CMD начинается с `view-`, то происходит немедленное переключение на представление из
  разделяемой библиотеки с именем CMD (пробелы не убираются, украшения добавляются). После этого
  выводится игровое поле, приглашение и происходит ещё один запрос команды. Не требуется как-то
  отслеживать имя текущего представления и отдельно разбирать переключение на текущий view:
  например, под Linux (в отличие от Windows) никто не запрещает изменить `.so`-файл, даже если он
  кем-то загружен.
* В противном случае команда трактуется как очередной ход, формат зависит от отображения. Некоторые
  отображения могут дополнительно распознавать команду выхода, по такой команде приложение должно
  немедленно корректно завершиться без дополнительных сообщений. Если введен некорректный ход
  (например, некорректный формат чисел, клетка занята и т.д.), то выводится сообщение `Bad move!`,
  затем команда запрашивается повторно (приглашение выводится еще раз, доску выводить не нужно).

Гарантируется, что в ответ на приглашение пользователь вводит строчку из произвольных символов с
ASCII-кодами от 32 до 127, завершённую ровно одним символом перевода строки.

Если поток ввода заканчивается без команды, приложение немедленно корректно завершается без
дополнительных сообщений. Гарантируется, что частично введённой команды (без перевода строки) не
будет, хотя это тоже интересный случай.

Если после очередного хода партия закончилась, то на экран выводится текущее игровое поля и, на
отдельной строчке, один из результатов игры: `X wins!`, `O wins!` или `Draw.`, после чего программа
корректно завершается.

Точную расстановку переводов строк и пробелов уточните в открытых тестах.

### Компактное отображение

Доска должна выводится в виде строк из символов, описывающих клетки (`X` — буква «X», крестик; `O` —
буква «O», нолик; `.` — точка, незанятая клетка), например:

```
XXXX.....O
XOXOXOXOXO
X........O
O........X
OXOXOXOXOX
XXXX.....O
XOXOXOXOXO
X........O
O........X
OXOXOXOXOX
```

Команда хода — это два целых числа (строка и столбец, числа от 0 до 9) через пробельный символ.
Также пользователь в ответ на приглашение может ввести два числа: -1 и -1, это считается командой
выхода.

Если пользователь добавляет лишние ведущие нули и/или пробельные символы до/после/между числами,
это всё ещё считается корректным ходом или командой завершения.

Пример партии. Обратите внимание на переводы строк и пробелы.

| Ввод           | Вывод                                                                                                                                                                                                                                                                                |
|:---------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                | `​` _(здесь и далее: пустая строчка)_<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/><code>O move:&nbsp;</code> _(здесь и далее без перевода строки)_ |
| `1 1`          | `​`<br/>`..........`<br/>`.O........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/><code>X move:&nbsp;</code>                                                                         |
| `0 0`          | `​`<br/>`X.........`<br/>`.O........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/><code>O move:&nbsp;</code>                                                                         |
| `0 0`          | `Bad move!`<br/><code>O move:&nbsp;</code>                                                                                                                                                                                                                                           |
| `1 0`          | `​`<br/>`X.........`<br/>`OO........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/><code>O move:&nbsp;</code>                                                                         |
| `0 1`          | _ходы опущены_                                                                                                                                                                                                                                                                       |
| _ходы опущены_ | `​`<br/>`XXXX......`<br/>`OOOO......`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/><code>O move:&nbsp;</code>                                                                         |
| `1 4`          | `​`<br/>`XXXX......`<br/>`OOOOO.....`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`..........`<br/>`O wins!` _(с переводом строки)_                                                                   |

Доступна [запись работы в терминале](https://asciinema.org/a/f91ZYWm5VYuUJ4uIGliELALVB) для этого примера.
Также эту запись можно просмотреть с помощью команды `asciinema play cli-demo.asciinema`.
На записи также есть демонстрация красивого отображения и переключения.

### Красивое отображение

Доска из `R` строк и `C` столбцов выводится в `3 * R + 2` строк, каждая ровно из `5 * C + 3`
символов (по умолчанию пробелы). Пример вывода для `R = 2`, `C = 3`, где крестик стоит в клетке `a0`, а нолик — в
клетке `b1`:

```
  +----+----+----+
1 |    | @@ |    |
  |    | @@ |    |
  +----+----+----+
0 | \/ |    |    |
  | /\ |    |    |
  +----+----+----+
    a    b    c
```

Команда хода — это строчка из ровно двух символов: сначала буква (обозначает столбец), потом цифра
(обозначает строку). Команды выхода в красивом режиме нет.

### Частичные решения

Вы можете реализовать любой префикс из четырёх подзаданий ниже и получить частичные баллы:

1. Разрешается не использовать разделяемые библиотеки и собрать одно приложение `tic-tac-toe`,
   которое всегда использует компактное отображение.
   * Игра всегда прерывается до её окончания.
   * Входные данные для компактного отображения всегда соответствуют либо корректному ходу, либо
     ходу в занятую клетку, либо команде выхода.
2. Как в предыдущем задании, но требуется сделать единственное отображение в разделяемой библиотеке,
   а не вкомпилировать в `tic-tac-toe`.
3. Как в предыдущем задании, но требуется сделать оба отображения и переключение между ними при
   помощи разделяемых библиотек.
   * Игра всегда прерывается до её окончания.
   * Гарантии на вход компактного отображения такие же.
   * Для красивого отображения ограничений на вход нет.
4. Как в предыдущем задании, но теперь игра может завершиться победой или ничьёй.
5. Дополнительные гарантии отсутствуют.

## Инструкция по сдаче

[Схема стандартная](../common/assignment-workflow.md): вы должны выполнить задание в отдельной ветке
своего существующего закрытого репозитория в организации и в нём же создать Pull Request. Никаких
форков! В этом задании должно хватить веб-интерфейса GitHub, если возникают проблемы — попросите
практика помочь.

Если вы хотите сдать только некоторые части задания, удалите соответствующие строки из файла
`tests.txt` и исправьте `CMakeLists.txt`. Автоматически тесты не могут отличить части 1 и 2, так что
тестов на часть 2 нет.

## Система оценки

[Схема стандартная](../common/common-requirements.md#система-оценки), баллы:

| Выполненные части | Макс. корректность | Макс. стиль | Макс. итог |
|-------------------|--------------------|-------------|------------|
| 1                 | 1                  | 1           | 2          |
| 1-2               | 2                  | 1           | 3          |
| 1-3               | 3                  | 2           | 5          |
| 1-4               | 5                  | 3           | 8          |
| 1-5               | 6                  | 4           | 10         |

## Сроки сдачи

Задание выдано 27 марта 2023 (понедельник),
Ниже в каждом случае указано кипрское время (_Asia/Nicosia_).

* **Дедлайн сдачи:** 2 апреля 2023 (воскресенье), **22**:59.
* Ожидаемый срок проверки: 9 апреля (воскресенье).
* Если первая попытка сдачи достаточно разумна (на усмотрение принимающего),
  то вы можете сделать ещё попытку исправлений или даже несколько.
  * Предложение актуально даже если вы получили автоматический ноль за первую попытку.
  * Выставляется максимум из всех попыток.
* **Срок исправлений**: 16 апреля 2022 (суббота), **22**:59.

[jb-yt-cpp-17579]: https://youtrack.jetbrains.com/issue/CPP-17579