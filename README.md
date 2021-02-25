*** Известные проблемы: ***

**Пайпы**

* Запуск в шелле команды ./21sh | ./21sh - два шелла в пайпе, ломает всё
* echo a | echo b<br/>
выдает ничего(даже переноса строки нет), а должно b


**Редиректы**
* при запуске через valgrind видны проблемы с fd <br/>
echo <a

==33440== Warning: invalid file descriptor -1 in syscall close()<br/>
А так же утечки при редиректе
* << heredoc не работает совсем. Их может быть неограниченное количество.
* Агрегация файлового дескриптера не работает

**&& и ||**
* || работает некорректно.
echo a || echo b <br/>
a<br/>
b<br/>
А должно выдавать только а. В целом логика ИЛИ требует доработки<br/>


**Обработка спец символов и парсинг строки:**
* echo \n выдает пустую строку вместо n
    echo \\n выдает \ вместо \n
* Размер имени и максимальная длинна строки не проверяются.
* команды типо $PWD выполняются некорректно
* команда echo \$PWD олжна выдать $PWD, а выдает содержимое переменной
* mkdir test ; cd test ; ls -a ; ls | cat | wc -c > fifi ; cat fifi <br/>
Команда должна выдать ". .. 5", выдаёт 0
* touch riri <br/>
rm riri; cat riri 2>&- <br/>
выполняется неправильно
* пробелы в коммандах сливают разные аргументы. К примеру, git add -A.<br/>
Одна строка - git<br/>
Вторая add -A<br/>
В итоге неправильно выполняется команда.
* функция *get_data_cmd(t_token *t, t_cmd *c, t_env **env) не во всех путях исполнения
содержит i++, в итоге иногда впадает в бесконечный цикл. К примеру, если ввести просто знак $ как команду или '\'. Без кавычек<br/> Где-то она за один проход по нескольку раз инкрементирует, поэтому влезать не стал.

**Переменные #TODO**
* Создание внутренних переменных шелла
* Вывод их для отображения через set 
* Вывод их в параметры окружения через export
* Снятие переменных через unset
* Создание переменных для уникальных команд, к примеру HOME=/tmp cd(хз, вроде и так работает)

**История:**
* Не сохраняется в файл,а это обязательное условие для модуля 42ш
* поиск по истории не работает(суффиксное дерево можно будет строить и для истории в том числе, а потом уже легко имплементировать поиск)

**Мултистрочный ввод**
* пустые строки некорректно отрабатывают
* По оси у за пределы отрисовки перемещаться пока нельзя.

**Автозаполнение:**
* Некорректно отображает множественные варианты:
    Либо уплывает список
    Либо перезаписывается строка ввода поверх списка
По итогу часто отсутствует отображение вариантов
* В редких случаях при запуске шелла автозаполнение отказывается работать от слова совсем.
Связанно с отображением

**Обработка сигналов:**
#TODO

**Job Control**
#TODO
* jobs,fg,bg и &

**type**
#TODO

**hash**
#TODO
* Хэш таблица была бы очень кстати. Особенно путей, особенно часто встречающихся. Man hash.<br/>

**Модули 42**
* Inhibitos - DONE
* History - partially done
* auto - done, for the most part