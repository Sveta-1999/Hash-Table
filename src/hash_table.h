#ifndef HASH_TABLE
#define HASH_TABLE

namespace stl {
    template <typename Key, typename Value>
    class HashTable {
    public:
        // Constructors
        HashTable() : m_buckets(10) {};
        HashTable(std::pair<Key, Value> elem);
        HashTable(size_t count);
        HashTable(std::pair<Key, Value> elem, size_t count);
        HashTable(const std::initializer_list<std::pair<Key, Value>>& elems); 
        HashTable(const HashTable& other); 
        HashTable(HashTable&& other); 
        ~HashTable(); 

    public:
        // Operators
        HashTable& operator=(const HashTable& other);
        HashTable& operator=(HashTable&& other); 
        Value& operator[](const Key& key); 
        const Value& operator[](const Key& key) const; 
        bool operator==(const HashTable<Key, Value>& rhs) const; 
        bool operator!=(const HashTable<Key, Value>& rhs) const;
        HashTable operator+(const HashTable<Key, Value>& rhs) const;
        const HashTable operator+=(const HashTable<Key, Value>& rhs);
        bool operator<(const HashTable<Key, Value>& rhs) const; 
        bool operator>(const HashTable<Key, Value>& rhs) const; 
        bool operator<=(const HashTable<Key, Value>& rhs) const; 
        bool operator>=(const HashTable<Key, Value>& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const HashTable<Key, Value>& hashTb) 
        {
            for(const auto& bucket : hashTb.m_buckets)
            {
                for(const auto& elem : bucket)
                {
                    os << "{" << elem.first << " , " << elem.second << "} ";
                }
            }
            return os;
        }

    public:
        // Member functions
        // checks whether the container is empty 
        bool empty();
        // returns the number of elements 
        size_t size() const;
        // clears the contents 
        void clear();
        // inserts elements or nodes 
        void insert(std::pair<Key, Value> elem);
        // constructs element in-place 
        void emplace(Key& key, Value& value);
        // erases elements 
        void erase(Key key);
        // returns function used to hash the keys 
        size_t hash_function(Key key)const;
        // swaps the contents 
        void swap(HashTable& other);
        //  returns the number of elements matching specific key 
        size_t count(Key key);
        // finds element with specific key 
        const Value& find(Key key);
        //  checks if the container contains element with specific key 
        bool contains(Key key);
        // splices nodes from another container 
        void merge(const HashTable& other);
        std::pair<Key, Value> extract(const Key& key);

    private:
        size_t new_size();
        // reserves at least the specified number of buckets and regenerates the hash table 
        void rehash();
        //  returns average number of elements per bucket 
        double load_factor() const;

    private:
        std::vector<std::forward_list<std::pair<Key, Value>>> m_buckets;
        size_t m_size {};

    }; // hash_table class scope
} // stl namespace scope

#include "hash_table.hpp"
#endif // HASH_TABLE