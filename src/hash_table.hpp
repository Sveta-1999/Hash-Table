template <typename Key, typename Value>
stl::HashTable<Key,Value>::HashTable(std::pair<Key, Value> elem) : m_buckets(9) {
    insert(std::make_pair(elem.first, elem.second));
}

template <typename Key, typename Value>
stl::HashTable<Key,Value>::HashTable(std::pair<Key, Value> elem, size_t count) : m_buckets(9) {
    for(int i = 0; i < count; ++i)
    {
        insert(std::make_pair(elem.first, elem.second));
    }
}

template <typename Key, typename Value>
stl::HashTable<Key,Value>::HashTable(const std::initializer_list<std::pair<Key, Value>>& elems) : m_buckets(9) {
    for (const auto& elem : elems)
    {
        insert({elem.first, elem.second});
    }
}

template <typename Key, typename Value> 
stl::HashTable<Key, Value>::HashTable(size_t count) : m_buckets(count) {}

template <typename Key, typename Value>
stl::HashTable<Key, Value>::HashTable(const HashTable& other) {
    m_size = other.m_size;
    m_buckets.resize(other.m_buckets.size()); 
    for(int i = 0; i < other.m_buckets.size(); ++i)
    {
        auto& bucket = m_buckets[i];
        const auto& other_bucket = other.m_buckets[i];
        for(const auto& p : other_bucket)
        {
            bucket.push_front(p);
        }
    }
}

template <typename Key, typename Value>
stl::HashTable<Key, Value>::HashTable(HashTable&& other)
{
    m_size = other.m_size;
    m_buckets = std::move(other.m_buckets);   
    other.m_buckets.clear();  
}

template <typename Key, typename Value>
stl::HashTable<Key, Value>& stl::HashTable<Key, Value>::operator=(const HashTable& other) {
    m_size = other.m_size;
    if(this == &other)
    {
        return *this;
    }
    m_buckets.resize(other.m_buckets.size()); 
    for(int i = 0; i < other.m_buckets.size(); ++i)
    {
        auto& bucket = m_buckets[i];
        const auto& other_bucket = other.m_buckets[i];
        for(const auto& p : other_bucket)
        {
            bucket.push_front(p);
        }
    }
    return *this;
}

template <typename Key, typename Value>
stl::HashTable<Key, Value>& stl::HashTable<Key, Value>::operator=(HashTable&& other) {
    m_size = other.m_size;
    m_buckets = std::move(other.m_buckets);   
    other.m_buckets.clear(); 
    return *this;
}

template <typename Key, typename Value>
bool stl::HashTable<Key, Value>::operator!=(const HashTable& other) const
{
  return !(*this == other);
}

template <typename Key, typename Value>
bool stl::HashTable<Key, Value>::operator==(const HashTable<Key, Value>& other) const {
    if (m_size != other.m_size)
    {
        return false;
    }
    for (std::size_t i = 0; i < m_buckets.size(); ++i)
    {
        if (m_buckets[i] != other.m_buckets[i])
        {
            return false;
        }
    }
    return true;
}

template <typename Key, typename Value>
bool stl::HashTable<Key,Value>::operator<(const HashTable<Key, Value>& other) const {
    return std::lexicographical_compare(m_buckets.begin(), m_buckets.end(), 
                                        other.m_buckets.begin(),
                                        other.m_buckets.end(),
                                        [](const auto& l1, const auto& l2) {
                                        return std::lexicographical_compare(l1.begin(), l1.end(),
                                                                            l2.begin(), l2.end(),
                                                                            [](const auto& p1, const auto& p2) {
                                                                                return p1.first < p2.first;});});
}

template <typename Key, typename Value>
bool stl::HashTable<Key, Value>::operator>(const HashTable<Key, Value>& other) const {
    return (other < *this);
}

template <typename Key, typename Value>
bool stl::HashTable<Key, Value>::operator<=(const HashTable<Key, Value>& other) const {
    return !(other < *this);
}

template <typename Key, typename Value>
bool stl::HashTable<Key, Value>::operator>=(const HashTable<Key, Value>& other) const {
    return !(other > *this);
}

template <typename Key, typename Value>
stl::HashTable<Key, Value> stl::HashTable<Key, Value>::operator+(const HashTable<Key, Value>& other) const {
    stl::HashTable<Key, Value> temp(m_size + other.m_size);
    size_t index {};
    for(const auto& bucket : m_buckets)
    {
        for(const auto& elem : bucket)
        {
            index = hash_function(elem.first);
            temp.insert(elem);
        }
    }
    for(const auto& bucket : other.m_buckets)
    {
        for(const auto& elem : bucket)
        {
            index = hash_function(elem.first);
            temp.insert(elem);
        }
    }
    if(temp.load_factor() > 0.75)
    {
        temp.rehash();
    }
    return temp;
}

template <typename Key, typename Value>
const stl::HashTable<Key, Value> stl::HashTable<Key, Value>::operator+=(const HashTable<Key, Value>& other) {
    stl::HashTable<Key, Value> temp(*this);
    operator=(temp + other);
    if(load_factor() > 0.75)
    {
        rehash();
    }
    return *this;
}

template <typename Key, typename Value>
size_t stl::HashTable<Key,Value>::hash_function(Key key) const {
    return key % m_buckets.size();
}

template <typename Key, typename Value>
void stl::HashTable<Key,Value>::insert(std::pair<Key, Value> elem) {
    if (load_factor() > 0.75)
    {
        rehash();
    }
    size_t bucketIndex = hash_function(elem.first);
    m_buckets[bucketIndex].push_front(std::make_pair(elem.first, elem.second));
    ++m_size;
}

template <typename Key, typename Value>
void stl::HashTable<Key, Value>::erase(Key key) {
    size_t bucketIndex = hash_function(key);
    auto& bucket = m_buckets[bucketIndex];
    bool flag = false;
    int count = 0;
    for(auto& elem : m_buckets[bucketIndex])
    {
        if(key == elem.first)
        {
            flag = true;
            ++count;
        }
    }
    if(flag)
    {
        bucket.erase_after(bucket.before_begin(), bucket.end());
        m_size -= count;
    }
    else 
    {
        std::cout << "Wrong key!!!" << std::endl;
    }
}

template <typename Key, typename Value>
void stl::HashTable<Key, Value>::emplace(Key& key, Value& value) {
    size_t bucketIndex = hash_function(key);
    m_buckets[bucketIndex].push_front(std::make_pair(key, value));
    ++m_size;
}

template <typename Key, typename Value>
void stl::HashTable<Key, Value>::merge(const HashTable& other)
{
    operator+=(other);
}

template <typename Key, typename Value>
stl::HashTable<Key, Value>::~HashTable() {
    clear();
}

template <typename Key, typename Value>
Value& stl::HashTable<Key,Value>::operator[](const Key& key) {
    size_t bucketIndex = hash_function(key);
    auto iter = m_buckets[bucketIndex].begin();
    for(; iter !=  m_buckets[bucketIndex].end(); ++iter)
    {
        if(iter->first == key)
        {
            return iter->second;
        }
    }
    std::cout << "Wrong address!!!" << std::endl;
    exit(0);
}

template <typename Key, typename Value>
const Value& stl::HashTable<Key,Value>::operator[](const Key& key) const {
    size_t bucketIndex = hash_function(key);
    for (const auto& elem : m_buckets[bucketIndex]) {
      if (elem.first == key) {
        return elem.second;
      }
    }
    std::cout << "Wrong address!!!" << std::endl;
    exit(0);
}

template <typename Key, typename Value>
void stl::HashTable<Key, Value>::swap(HashTable<Key, Value>& other) {
    std::vector<std::forward_list<std::pair<Key, Value>>> temp = m_buckets;
    m_buckets = other.m_buckets;
    other.m_buckets = temp;
}

template <typename Key, typename Value>
size_t stl::HashTable<Key, Value>::count(Key key) {
    size_t count = 0;
    size_t keyIndex = hash_function(key);
    auto& bucket = m_buckets[keyIndex];
    for(const auto& p : bucket)
    {
        ++count;
    }
    return count;
}

template <typename Key, typename Value>
const Value& stl::HashTable<Key, Value>::find(Key key) {
    size_t keyIndex = hash_function(key);
    auto& bucket = m_buckets[keyIndex];
    for(const auto& p : bucket)
    {
        return p.second;
    }
    std::cout << "Wrong key!!!" << std::endl;
    exit(0);
}

template <typename Key, typename Value>
bool stl::HashTable<Key, Value>::contains(Key key) {
    for (const auto& list : m_buckets) {
        for (const auto& pair : list) {
            if (pair.first == key) {
                return true;
            }
        }
    }
    return false;
}

template <typename Key, typename Value>
std::pair<Key, Value> stl::HashTable<Key, Value>::extract(const Key& key) {
    size_t index = hash_function(key);
    std::pair<Key, Value> temp;
    for(const auto& elem : m_buckets[index])
    {
        if(elem.first == key)
        {
            temp = elem;
        }
    }
    return temp;
}

template <typename Key, typename Value>
size_t stl::HashTable<Key, Value>::new_size() {
    return 2 * m_buckets.size();
}

template <typename Key, typename Value>
void stl::HashTable<Key, Value>::rehash() {
    size_t newSize = new_size();
    std::vector<std::forward_list<std::pair<Key, Value>>> new_buckets(newSize);
     for (size_t i = 0; i < m_buckets.size(); ++i)
    {
        for (auto& elem : m_buckets[i])
        {
            size_t new_index = hash_function(elem.first);
            new_buckets[new_index].push_front(elem);
        }
    }
    m_buckets = std::move(new_buckets);
}

template <typename Key, typename Value>
double stl::HashTable<Key, Value>::load_factor() const {
    return static_cast<double>(m_size) / m_buckets.size();
}

template <typename Key, typename Value>
bool stl::HashTable<Key, Value>::empty() {
    if(m_size == 0)
    {
        return true;
    }
    return false;
}

template <typename Key, typename Value>
size_t stl::HashTable<Key, Value>::size() const {
    return m_size;
}

template <typename Key, typename Value>
void stl::HashTable<Key, Value>::clear() {
    for(int i = 0; i < m_buckets.size(); ++i)
    {
        m_buckets[i].clear();
    }
    m_size = 0;
}
