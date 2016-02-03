#ifndef UTIL_MEMORY_HPP
#define UTIL_MEMORY_HPP

#include <algorithm>
#include <cstdint>
#include <chrono>
#include <memory>
#include <cstring>
#include <thread>

#include "Core/Logger.hpp"
#include "TypeId/TypeId.hpp"

namespace dm {
template <typename object_type>
class Allocator {
   public:
    inline static object_type* allocate(const size_t& elements) {
        object_type* new_block = reinterpret_cast<object_type*>(operator new(
            sizeof(object_type) * elements, std::nothrow));

        if (!new_block) throw std::bad_alloc();
        return new_block;
    }

    inline static void deallocate(object_type*& ptr) {
        operator delete(ptr);
        ptr = nullptr;
    }

    inline static object_type* reallocate(object_type*& ptr, const size_t size,
                                          const size_t new_size) {
        object_type* new_block = allocate(new_size);
        if (ptr) {
            std::memcpy(new_block, ptr,
                        sizeof(object_type) * std::min(size, new_size));
            deallocate(ptr);
        }
        return reinterpret_cast<object_type*>(new_block);
    }
};

template <typename object_type>
class Stack {
   public:
    using allocator = Allocator<object_type>;

    Stack() : size_(0), data_(nullptr), top_(0) {}

    ~Stack() { allocator::deallocate(data_); }

    void reserve(const size_t& elements) {
        data_ = allocator::reallocate(data_, top_, elements);
        size_ = elements;
    }

    const size_t capacity() const { return size_; }
    const size_t size() const { return top_; }

    template <typename... arg_types>
    object_type& push(arg_types&&... args) {
        ++top_;

        /* needs more memory */
        if (top_ >= size_) {
            size_ = (size_ * 2) + 1;
            data_ = allocator::reallocate(data_, top_ - 1, size_);
        }

        return *(new (data_ + (top_ - 1))
                     object_type(std::forward<arg_types>(args)...));
    }

    object_type& pop() { return data_[--top_]; }
    bool empty() { return (top_ == 0); }
    void clear() { top_ = 0; }
    object_type* begin() { return data_; }
    object_type* end() { return data_ + top_; }

    /* sort it in descending order in memory (ascending in pop order) */
    void sort() { std::sort(data_, data_ + top_, std::greater<object_type>()); }

   private:
    unsigned int size_;
    object_type* data_;
    unsigned int top_;
};

template <typename object_type, size_t block_size = 10>
class MemoryPool {
   public:
    struct Block {
        object_type data_[block_size];
        Block* next_;
        Block* prev_;
    };

    using allocator = Allocator<object_type>;
    using block_allocator = Allocator<Block>;

    template <typename... arg_types>
    object_type& push(arg_types&&... args) {
        object_type* new_object = this->allocate(1);
        if (new_object)
            new (new_object) object_type(std::forward<arg_types>(args)...);
        return *new_object;
    }

    object_type& push() {
        object_type* new_object = this->allocate(1);
        if (new_object) new (new_object) object_type();
        return *new_object;
    }

    object_type* allocate(const size_t size) {
        object_type* new_object = nullptr;
        /* ran out of space, allocate a new block */
        if (free_.empty()) allocate_block();
        new_object = free_.pop();
        size_++;
        return (new_object);
    }

    void deallocate(object_type* ptr) {
        /* mark it as free */
        free_.push(ptr);
        free_.sort();
        size_--;
    }

    static MemoryPool<object_type, block_size>& instance() {
        static MemoryPool<object_type, block_size> instance_;
        return instance_;
    }

    static std::mutex& mutex() {
        return MemoryPool<object_type, block_size>::instance().mut_;
    }

    object_type* allocate_block() {
        Block* block = block_allocator::allocate(1);

        if (!last_block_ && !first_block_) {
            first_block_ = block;
            last_block_ = block;
        }

        last_block_->next_ = block;
        first_block_->prev_ = block;
        block->prev_ = last_block_;
        block->next_ = first_block_;
        last_block_ = block;

        /* reserve the necesary size to represent a new block */
        free_.reserve(free_.size() + block_size);

        for (object_type* i = block->data_; i < block->data_ + block_size; i++)
            free_.push(i);

        free_.sort();

        return block->data_;
    }

    void cleanup() {
        Log::progress("debug", "Cleaning up the memory pool for %s",
                      dm_type_name(object_type));

        Block* block = first_block_;

        assert(first_block_ != nullptr, "MemoryPool already freed");

        for (auto& object : *this) {
            object.~object_type();
            this->deallocate(&object);
        }

        do {
            Block* next = block->next_;
            block_allocator::deallocate(block);
            block = next;
        } while (block != first_block_);

        assert(size_ == 0, "MemoryPool not freed properly");

        first_block_ = nullptr;

        Log::result(Log::Result::OK);
    }

    ~MemoryPool() { cleanup(); }

    bool is_free(object_type* object) {
        return std::binary_search(free_.begin(), free_.end(), object,
                                  std::greater<object_type*>());
    }

    Block* get_block(object_type* object) {
        Block* block = first_block_;

        do {
            if (object >= &block->data_[0] &&
                object < &block->data_[0] + block_size)
                return block;
            block = block->next_;
        } while (block != first_block_);

        assert(false, "Object not in pool");

        return nullptr;
    }

    class Iterator {
       private:
        object_type* current_;
        Block* current_block_;
        MemoryPool<object_type>& pool_;

       public:
        Iterator(MemoryPool<object_type>& pool, object_type* pos)
            : current_(pos), current_block_(pool.last_block_), pool_(pool) {
            if (current_ != pool.end_ptr()) {
                current_block_ = pool_.get_block(current_);
            }
        }

        void reset() {
            current_ = &pool_.first_block_->data_[0];
            current_block_ = pool_.first_block_;
            skip_free();
        }

        void next() {
            current_++;
            skip_free();
        }

        void skip_free() {
            while (pool_.is_free(current_)) current_++;

            if (current_ >= &current_block_->data_[block_size]) {
                if (current_block_->next_ != pool_.first_block_) {
                    current_block_ = current_block_->next_;
                    skip_free();
                } else
                    current_ = pool_.end_ptr();
            }
        }

        const bool operator==(const Iterator& rhs) const {
            return current_ == rhs.current_;
        }

        const bool operator!=(const Iterator& rhs) const {
            return current_ != rhs.current_;
        }

        Iterator& operator++() {
            this->next();
            return *this;
        }

        object_type& operator*() { return *current_; }

        object_type* operator->() { return current_; }

        friend MemoryPool;
    };

    object_type* end_ptr() { return &last_block_->data_[block_size]; }

    Iterator begin() {
        Iterator it(*this, &first_block_->data_[0]);
        it.skip_free();
        return it;
    }

    Iterator end() { return Iterator(*this, end_ptr()); }

    Stack<object_type*>& get_free() { return free_; }

    const size_t size() { return size_; }

   private:
    MemoryPool()
        : first_block_(nullptr), last_block_(nullptr), size_(0), mut_() {}

    Block* first_block_;
    Block* last_block_;
    Stack<object_type*> free_;
    size_t size_;
    std::mutex mut_;
};

#define dm_memory_pool_impl(object_type)                                       \
    void* operator new(size_t size) {                                          \
        object_type* ptr = MemoryPool<object_type>::instance().allocate(size); \
        return ptr;                                                            \
    }                                                                          \
    void operator delete(void* ptr) {                                          \
        MemoryPool<object_type>::instance().deallocate(                        \
            static_cast<object_type*>(ptr));                                   \
    }

#define dm_memory_pool(object_type)                            \
    dm_memory_pool_impl(object_type)                           \
    }                                                          \
    ;                                                          \
    dm_internal_register_type_name(object_type, #object_type); \
    namespace dummy {
}

#endif
