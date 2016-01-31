#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace dm {

template <class ObjectType>
class Singleton {
   public:
    static ObjectType& Instance() {
        static ObjectType instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;

    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

   private:
    Singleton() {}
    ~Singleton() {}
};
}

#endif
