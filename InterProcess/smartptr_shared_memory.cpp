#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>

using namespace boost::interprocess;

struct list_node
{
    offset_ptr<list_node> next;
    int value;
};

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

        // Create linked list with 10 nodes in shared memory
        offset_ptr<list_node> prev = 0, current, first;

        for (int i = 0; i < 10; ++i, prev = current)
        {
            current = static_cast<list_node *>(segment.allocate(sizeof(list_node)));
            current->value = i;
            current->next = 0;

            if (!prev)
                first = current;
            else
                prev->next = current;
        }

        // Communicate list to other processes
        // ...
        // When done, destroy list
        for (current = first; current; /**/)
        {
            prev = current;
            current = current->next;
            segment.deallocate(prev.get());
        }
    }

    return 0;
}