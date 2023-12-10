#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <functional>
#include <string>

using namespace boost::interprocess;

// Note that map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
// so the allocator must allocate that pair.
typedef int KeyType;
typedef float MappedType;
typedef std::pair<const int, float> ValueType;
// Alias an STL compatible allocator of for the map.
// This allocator will allow to place containers
// in managed shared memory segments
typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;

// Alias a map of ints that uses the previous STL-like allocator.
// Note that the third parameter argument is the ordering function
// of the map, just like with std::map, used to compare the keys.
typedef map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> MyMap;

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        struct shm_remove
        {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        } remover;

        managed_shared_memory segment(create_only, "MySharedMemory", 65536);

        ShmemAllocator alloc_inst(segment.get_segment_manager());

        // Construct a shared memory map.
        // Note that the first parameter is the comparison function,
        // and the second one the allocator.
        // This the same signature as std::map's constructor taking an allocator
        MyMap *mymap = segment.construct<MyMap>("MyMap") // object name
                       (std::less<int>()                 // first ctor parameter
                        ,
                        alloc_inst); // second ctor parameter

        for (int i = 0; i < 100; ++i)
        {
            mymap->insert(std::pair<const int, float>(i, (float)i));
        }

        std::string s(argv[0]);
        s += " child ";
        if (0 != std::system(s.c_str()))
            return 1;

        if (segment.find<MyMap>("MyMap").first)
            return 1;
    }
    else
    {
        managed_shared_memory segment(open_only, "MySharedMemory");

        MyMap *mymap = segment.find<MyMap>("MyMap").first;

        segment.destroy<MyMap>("MyMap");
    }

    return 0;
}
