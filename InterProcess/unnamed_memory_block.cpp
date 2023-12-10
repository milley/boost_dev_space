#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

struct Person
{
    Person() {}

    Person(string name, int age, string nickname)
    {
        name_ = name;
        age_ = age;
        nickname_ = nickname;
    }

    string name_;
    int age_;
    string nickname_;
};

int main(int argc, char *argv[])
{

    using namespace boost::interprocess;
    if (argc == 1)
    { // Parent process
        cout << "main parent start" << endl;
        // Remove shared memory on construction and destruction
        struct shm_remove
        {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        } remover;

        // Create a managed shared memory segment
        managed_shared_memory segment(create_only, "MySharedMemory", 65536);

        // Alocate a portion of the segment (raw memory)
        managed_shared_memory::size_type free_memory = segment.get_free_memory();
        void *shptr = segment.allocate(1024 /*bytes to allocate*/);

        Person person_arr[2];
        Person p1("mike", 20, "mike mini");
        Person p2("sam", 18, "hook");
        person_arr[0] = p1;
        person_arr[1] = p2;

        memcpy(shptr, person_arr, sizeof(Person) * 2);

        // Check invariant
        if (free_memory <= segment.get_free_memory())
            return 1;

        // An handle from the base address can identify any byte of the shared
        // memory segment even if it is mapped in different base addresses
        managed_shared_memory::handle_t handle = segment.get_handle_from_address(shptr);
        std::stringstream s;
        s << argv[0] << " " << handle;
        s << std::ends;
        // Launch child process
        if (0 != std::system(s.str().c_str()))
        {
            return 1;
        }
        // Check memory has been freed
        if (free_memory != segment.get_free_memory())
            return 1;
        cout << "main parent done" << endl;
    }
    else
    {
        cout << "main child start" << endl;
        // Open managed segment
        managed_shared_memory segment(open_only, "MySharedMemory");

        // An handle from the base address can identify any byte of the shared
        // memory segment even if it is mapped in different base addresses
        managed_shared_memory::handle_t handle = 0;

        // Obtain handle value
        std::stringstream s;
        s << argv[1];
        s >> handle;

        // Get buffer local address from handle
        void *msg = segment.get_address_from_handle(handle);

        Person person_arr[2];
        memcpy(person_arr, msg, sizeof(Person) * 2);

        for (int i = 0; i < 2; i++)
        {
            cout << "{" << person_arr[i].name_ << "," << person_arr[i].age_
                 << "," << person_arr[i].nickname_ << "}" << endl;
        }

        // Deallocate previously allocated memory
        segment.deallocate(msg);
        cout << "main child done" << endl;
    }

    return 0;
}
