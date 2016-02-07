#include <jester/jester.hpp>
#include <Util/Memory.hpp>
#include <iostream>

using namespace jester;
using namespace dm;

struct object {
    int number;
    dm_memory_pool(object);
};

int main(int argc, char *argv[])
{

    describe("memory pool", []() {
        it("should allocate chunk", []() {
            object* o = new object();
            assert_true(o != nullptr);
            assert_false(object::memory_pool::instance().is_free(o));
        });
    });

    return 0;
}
