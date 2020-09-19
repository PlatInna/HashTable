/* HashTable.cpp : https://www.coursera.org/ Основы разработки на C++: коричневый пояс, Неделя 1.
Задание по программированию : Хеш-таблица
Теперь, когда вы знаете, как unordered-контейнеры устроены внутри, вам предлагается самостоятельно реализовать упрощённую версию хеш-таблицы. 
Реализуйте шаблон HashSet.
Требования:
    - В любой момент времени в вашей хеш-таблице должно быть ровно num_buckets корзин. Рехеширование выполнять не нужно.
    - Метод Add не должен менять содержимое таблицы, если элемент, равный данному, уже есть в таблице.
    - Метод Has должен возвращать true тогда и только тогда, когда элемент, равный данному, есть в таблице.
    - Метод Erase должен стирать элемент, равный данному, из таблицы, если он там есть, в противном случае он не должен менять содержимое таблицы.
    - Метод GetBucket должен возвращать ссылку на бакет, который содержал бы данный элемент, вне зависимости от того, есть он в таблице или нет.
*/

//#include "test_runner.h"
#include "..\..\test_runner.h"

#include <forward_list>
#include <iterator>
#include <algorithm>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
    public:
        using BucketList = forward_list<Type>;

    public:
        explicit HashSet(size_t num_buckets, const Hasher& hasher = {}) 
            : _buckets_(num_buckets)
            , _hasher_{ hasher }
        {}

        void Add(const Type& value) {
            size_t i = _HashIndex_(value);
            if (find(_buckets_[i].begin(), _buckets_[i].end(), value) == _buckets_[i].end()) {
                _buckets_[i].push_front(value);
            }
        }

        bool Has(const Type& value) const {
            size_t i = _HashIndex_(value);
            return find(_buckets_[i].begin(), _buckets_[i].end(), value) != _buckets_[i].end();
        }

        void Erase(const Type& value) {
            _buckets_[_HashIndex_(value)].remove(value);
        }

        const BucketList& GetBucket(const Type& value) const {
            return _buckets_[_HashIndex_(value)];
        }

    private:
        vector<BucketList> _buckets_;
        const Hasher& _hasher_;
        size_t _HashIndex_(const Type& value) const {
            return _hasher_(value) % _buckets_.size();
        }
};

struct IntHasher {
    size_t operator()(int value) const {
        // Это реальная хеш-функция из libc++, libstdc++.
        // Чтобы она работала хорошо, std::unordered_map
        // использует простые числа для числа бакетов
        return value;
    }
};

struct TestValue {
    int value;

    bool operator==(TestValue other) const {
        return value / 2 == other.value / 2;
    }
};

struct TestValueHasher {
    size_t operator()(TestValue value) const {
        return value.value / 2;
    }
};

void TestSmoke() {
    HashSet<int, IntHasher> hash_set(2);
    hash_set.Add(3);
    hash_set.Add(4);

    ASSERT(hash_set.Has(3));
    ASSERT(hash_set.Has(4));
    ASSERT(!hash_set.Has(5));

    hash_set.Erase(3);

    ASSERT(!hash_set.Has(3));
    ASSERT(hash_set.Has(4));
    ASSERT(!hash_set.Has(5));

    hash_set.Add(3);
    hash_set.Add(5);

    ASSERT(hash_set.Has(3));
    ASSERT(hash_set.Has(4));
    ASSERT(hash_set.Has(5));
}

void TestEmpty() {
    HashSet<int, IntHasher> hash_set(10);
    for (int value = 0; value < 10000; ++value) {
        ASSERT(!hash_set.Has(value));
    }
}

void TestIdempotency() {
    HashSet<int, IntHasher> hash_set(10);
    hash_set.Add(5);
    ASSERT(hash_set.Has(5));
    hash_set.Add(5);
    ASSERT(hash_set.Has(5));
    hash_set.Erase(5);
    ASSERT(!hash_set.Has(5));
    hash_set.Erase(5);
    ASSERT(!hash_set.Has(5));
}

void TestEquivalence() {
    HashSet<TestValue, TestValueHasher> hash_set(10);
    hash_set.Add(TestValue{ 2 });
    hash_set.Add(TestValue{ 3 });

    ASSERT(hash_set.Has(TestValue{ 2 }));
    ASSERT(hash_set.Has(TestValue{ 3 }));

    const auto& bucket = hash_set.GetBucket(TestValue{ 2 });
    const auto& three_bucket = hash_set.GetBucket(TestValue{ 3 });
    ASSERT_EQUAL(&bucket, &three_bucket);

    ASSERT_EQUAL(1, distance(begin(bucket), end(bucket)));
    ASSERT_EQUAL(2, bucket.front().value);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSmoke);
    RUN_TEST(tr, TestEmpty);
    RUN_TEST(tr, TestIdempotency);
    RUN_TEST(tr, TestEquivalence);
    return 0;
}