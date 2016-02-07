#include <jester/jester.hpp>
#include <Util/Memory.hpp>
#include <iostream>

using namespace jester;
using namespace dm;

struct object {
    int number;
    char x;
    dm_memory_pool(object);
};

int main(int argc, char *argv[])
{

    describe("memory pool", []() {
        it("should allocate object", []() {
            object* o = new object();
            assert_true(o != nullptr);
            assert_equal(object::memory_pool::instance().size(), 1);
            std::cout << (uintptr_t)o << std::endl;
            for(auto& p : object::memory_pool::instance().get_free()) {
                std::cout << (uintptr_t)p << std::endl;
            }
            assert_equal(
                object::memory_pool::instance().get_free().size(),
                object::memory_pool::block_size - 1
            );
            assert_false(object::memory_pool::instance().is_free(o));
        });
    });

    return 0;
}
