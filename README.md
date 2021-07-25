# Cognitia II
В этом репозитории находится вторая версия программы для оптимизации процесса обучения студентов, а также планирования курсов.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/Logo.png)

# Идея
Нужна программа, в которой сначала можно будет учебный спланировать курс. Например собрать из каких-нибудь удобных блоков. Потом по этой схеме курса
можно вести каждого студента отдельно. Важно иметь возможность отслеживать успехи студента на самом низком уровне, т.е. насколько каждый владеет тем или иным навыком.
На основе этим данных можно рекомендовать бонусные задания, стажировки, выявлять лучших студентов и т.д.

# План

## Структуры
Введем несколько структур для хранения, сохранения и загрузки данных.

### SkillPack
Это набор навыков, которыми студент может обзавестись в процессе обучения. Причем каждым навыком можно владеть на определенном уровне. 
Какое знание навыка какому уровню соответствует, определяет описание уровня. Формально это выглядит так:

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/SkillPack.png)

Пусть мы рассматриваем некий навык **S**. Тогда у него есть множество уровней. Причем существует число **N** - максимальный уровень.
Тогда будем считать, что:
1. Для каждого навыка у каждого студента будет число **K**, которое будет отображать текущий прогресс студента в данном навыке. Причем **0 <= K <= N**
2. Если имеем некий уровень **L**. Тогда **K >= L** в том и только в том случае, если выполняются условия из описания уровня

### CourseUnit
Это единица курса. Представляет собой задачу, которую нужно выполнить. Существуют два набора навыков: входные навыки - то, что необходимо знать, чтобы приступить к работе; выходные навыки - максимальные навыки, которые может получить студент после решения задачи.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/CourseUnit.jpg)

На выходные навыки накладываются некие ограничения:   
Пусть **S** - выходной навык, **L** - его уровень, тогда:
1. Если существует входной навык **S'** с уровнем **L'**, такой что **S' = S**. Тогда необходимо, чтобы **L' + 1 = L**
2. Если не существует входного навыка **S'**, такого что **S' = S**. Тогда необходимо, чтобы **L = 1**

Единицы курса образовывают ориентированный ациклический граф курса, играя роль вершин.   
Причем для корректного построения графа необходимо следующее:      
Для любой единицы курса **U** с **входными** скилами и их уровнями **{Si, Li}** существует набор **входных** ребер, которые связаны с другими вершинами **{Vj}**.
У каждой вершины **Vj** есть набор **выходных** скилов и их уровней **{Sjk, Ljk}**.   
Тогда для любого **входного** скила **{Si, Li}** существует **j** и **k**, такое что **Si = Sjk** и **Li = Ljk**.   

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/CourseGraph.jpg)

Единицу курса можно помечать меткой.   
Виды меток:
* Начало графа - отсутствуют входные ребра (в графе такая метка ровно одна)
* Конец графа - отсутствуют выходные ребра (в графе такая метка ровно одна)
* Бонус - данная единица курса не является основной, а является дополнительной   

Каждая единица курса может содержать граф других единиц курса. Такая рекурсия может продолжаться до бесконечности.
Причем входные скилы совпадают с входными скилами внутренней вершины с меткой **Начало графа**, а выходные скилы совпадают с выходными скилами вершины с меткой **Конец графа**.
Более того, весь курс, с которым будет работать пользователь будет **корневой** единицей курса, которая содержит в себе основной граф курса.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/CourseUnitRecursion.jpg)

### StudentProgress
Эта структура индивидуальна для каждого студента. Она жестко привязана к заданному SkillPack и CourseUnitGraph.   
Она содержит:
* для каждого скила действительное число K - навык студента в данном скиле. Если максимальный уровень данного скила - N, то 0 <= K <= N, причем число [K] - текущий уровень студента в данном скиле
* Данные о бонусных единицах курса, которые ментор может добавлять по своему желанию. По сути это просто дополнительные произвольные единицы курса, но они могут быть уникальны для каждого студента


## Процесс работы системы
Опираясь на заданные структуры данных, поймем, как будет происходить процесс взаимодействия с программой. Пользователями выступают методист, менторы и студенты.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/WorkProcess.jpg)

Здесь зеленая линия - чтение из облака, красная линия - запись в облако. Вершины обозначают действия пользователей. Кроме синей жирной - она обозначает облачное хранилище.
Под вершиной написано, кто выполняет данное действие.


## GUI
Теперь, когда мы знаем, что и как должно работать, нужно определить соответствующие gui-программы.

### CognitiaLauncher
Это будет основное окно, из которого будет запускаться все остальное.
В середине экрана будет кнопка, в которой можно выбрать, что именно запустить. А справа от нее кнопка запуска.
Сверху - лого. Снизу справа - автор.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/gui/Launcher.jpg)

### SkillPackEditor
Программа для редактирования файлов со скилами. Все просто - три колонки: первая - список скилов, вторая - список уровней скила, третья - описание текущего уровня скила.
Сверху есть меню, где можно сохранить, открыть и т.д.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/gui/SkillPackEditor.jpg)

### CourseEditor
Редактор курса.
Справа меню для редактирования единицы курса. Слева OpenGL отображение графа.
Сверху меню для сохранения, загрузки и т.д.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/gui/CourseEditor.jpg)

### CognitiaRepository
Здесь можно активировать облачное хранилище для студентов и менторов.
Работает на сокетах.
Редактирование папки с данными студентов.
Редактирование Графа курса.
Кнопки запуска и остановки сервера.
Справа консоль для информации о работе сервера.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/gui/CognitiaRepository.jpg)

### StudentClient
Интерфейс для студента.
Панель со списком скилов и индивидуальном прогрессе в каждом.
Панель для подключения к репозиторию.
Справа окно с графом курса. В нем отображается прогресс обучения.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/gui/StudentClient.jpg)

### MentorClient
Интерфейс для ментора.
Слева панель для подключения к репозитрию, панель для выбора студента, панель для взаимодействия с единицей курса (тут есть кнопка, что единица курса пройдена)
В этом месте ментор сможет без удобно и быстро регулировать навыки студентов. Отмечать успехи и т.д. 
Справа панель с графом курса.

![](https://github.com/timattt/Project-Cognitia-II/blob/main/About/gui/MentorClient.jpg)










