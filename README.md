Известные проблемы:

Пайпы:
Запуск в шелле команды ./21sh | ./21sh - два шелла в пайпе, ломает всё
Команда ppid ломает иногда пайп

Редиректы:
Редиректы не работают(

Эко:
1) echo ''"$PWD"'' - печатает лишние кавычки


Обработка спец символов:
1) echo \n выдает пустую строку вместо n
    echo \\n выдает \ вместо \n
2) создаем в баше папку с пробелом в названии:
    mkdir ha\ he
    cd ha\ he должно дать ошибку(опционально, как по мне)
    cd ha\ he/ точно должно работать
    21sh в данный момент выдает:
        -shelp!: ha: No such file or directory
3) часто пользуюсь командой для отслеживания количества процессов
    ps aux | grep 21sh | awk '{print $2}' | wc -l
    Отрабатывает вот так:
    awk: cmd. line:1: {print
    awk: cmd. line:1:       ^ unexpected newline or end of string
    0


История:
1) Не сохраняется в файл,а это обязательное условие для модуля 42ш
2) поиск по истории не работает(суффиксное дерево можно будет строить и для истории в том числе, а потом уже легко имплементировать поиск)

Соединение строк в единую строку
1) \ для конкатенации строк не имплементировано(нужно ли?)
2) Не уверен как обрабатывается каретка - \n пока вроде игнорируется и заменяется вроде как ничем. Как корректно сделать стоит обсудить
Там 3 разных сценария - кавычки, heredoc и \

Отображение:
1) Переход по истории меняет Y адрес строки и часто затирает уже написанное
2) Если первая строка не умещается в терминал(уплывает вверх из-за скролла), ломается отображение и перемещение по строке, но не сразу
3) Ломается отображение в специфичном случае: начать писать на последней строке терминала, задействовать доп строку через heredoc
или кавычки и если эта подстрока не уместившись переплывет на новую строку терминала, то с каждым новым символом она будет отрисовываться 1 лишний раз


Автозаполнение:
Конструктор дерева готов, но конвеер передачи аргументов нет
Придумал способ рекурсивно вынимать слова, но не могу вынимать их эффективно, т.к. каждое слово - это проход по списку от головы как в случае запроса, так и вставки.
Надо игнорировать все символы не входящие в предусмотренный алфавит


Обработка сигналов:
#TODO
