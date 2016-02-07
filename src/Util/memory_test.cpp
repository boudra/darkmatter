#include <jester/jester.hpp>
#include <Util/Memory.hpp>

using namespace jester;

struct object {
    int number;
    dm_memory_pool(object);
};

int main(int argc, char *argv[])
{

    describe("memory pool", []() {
        it("should allocate chunk", []() {
            object* o = new object;
            assert_true(o != nullptr);
            assert_false(object::memory_pool::instance().is_free(o));
        });
    });

    return 0;
}
