# HashTable
Coursera: C++ Development Fundamentals: Brown Belt: Week 1

Теперь, когда вы знаете, как unordered-контейнеры устроены внутри, вам предлагается самостоятельно реализовать упрощённую версию хеш-таблицы. 
Реализуйте шаблон HashSet.
Требования:
    - В любой момент времени в вашей хеш-таблице должно быть ровно num_buckets корзин. Рехеширование выполнять не нужно.
    - Метод Add не должен менять содержимое таблицы, если элемент, равный данному, уже есть в таблице.
    - Метод Has должен возвращать true тогда и только тогда, когда элемент, равный данному, есть в таблице.
    - Метод Erase должен стирать элемент, равный данному, из таблицы, если он там есть, в противном случае он не должен менять содержимое таблицы.
    - Метод GetBucket должен возвращать ссылку на бакет, который содержал бы данный элемент, вне зависимости от того, есть он в таблице или нет.
