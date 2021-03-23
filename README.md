*** Известные проблемы: ***

**Пайпы**
* Запуск в шелле команды ./21sh | ./21sh - два шелла в пайпе, ломаетcя, но не так плохо как раньше
* Отслеживание ФД
* наверное стоит отслеживать максимлаьный размер пайпа


**Редиректы**
* cat \<\b<br/>
==33440== Warning: invalid file descriptor -1 in syscall close()<br/>
* << heredoc не работает совсем. Их может быть неограниченное количество.
* Агрегация файлового дескриптера не работает
* touch riri <br/>
rm riri; cat riri 2>&- <br/>
* mkdir test ; cd test ; ls -a ; ls | cat | wc -c > fifi ; cat fifi <br/>
Команда должна выдать ". .. 5", выдаёт ". .. 0"
* команды по примеру <& и &>

**&& и ||**
* || работает некорректно.
echo a || echo b <br/>
a<br/>
b<br/>
А должно выдавать только а. В целом логика ИЛИ требует доработки<br/>

**Переменные**
* Изменить при старте локаль с utf-8, раз он не обрабатывается.
* export и сет требуют тестов и сверки
* Создание переменных для уникальных команд, к примеру HOME=/tmp cd

**Отображение**
* Нет лимита. Обрабатывает, не ломается, но чревато.

**История:**
* Не сохраняется в файл,а это обязательное условие для модуля 42ш
* поиск по истории не работает(суффиксное дерево можно будет строить и для истории в том числе, а потом уже легко имплементировать поиск)

**Пути**
* Везде где путь опирается на ВЛА на стаке надо исправить его на динамический массив

**Job Control**
#TODO
* jobs - п,fg,bg и &. Обязательный переквизит для 42, некритично для 21.

**Signals**
*обработка SIGPIPE отложил на будущее. Критично!
*в целом обработка сигналов при выполнении пайпов и редиректов пока дыра

**hash**
#TODO
* Хэш таблица была бы очень кстати. Особенно путей, особенно часто встречающихся. Man hash.<br/>