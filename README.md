# Hash-Table
<b>Hash Table </b> is a data structure which stores data in an associative manner. In a hash table, data is stored in an array format, where each data value has its own unique index value. Access of data becomes very fast if we know the index of the desired data.

<hr>


![HashTable](https://user-images.githubusercontent.com/79526438/209135864-9c1bc49a-abe0-4aeb-9d36-0fdc393c38c6.png) 


In hashing there is a hash function that maps keys to some values. But these hashing function may lead to collision that is two or more keys are mapped to same value. <b> Chain hashing </b> avoids collision. The idea is to make each cell of hash table point to a linked list of records that have same hash function value.


<hr>

• <b>empty():</b> checks whether the container is empty </br>
• <b>size(): </b> returns the number of elements </br>
• <b>clear):</b> clears the contents </br>
• <b>insert():</b> inserts elements or nodes </br>
• <b>emplace():</b> constructs element in-place </br>
• <b>erase():</b> erases elements </br>
• <b>swap():</b> swaps the contents </br>
• <b>extract():</b> extracts nodes from the container </br>
• <b>merge():</b> splices nodes from another container </br>
• <b>count():</b> returns the number of elements matching specific key </br>
• <b>find():</b> finds element with specific key </br>
• <b>contains():</b> checks if the container contains element with specific key </br>
• <b>load factor():</b> returns average number of elements per bucket </br>
• <b>rehash():</b> reserves at least the specified number of buckets and regenerates the hash table </br>
• <b>hash _function():</b> returns function used to hash the keys </br>
