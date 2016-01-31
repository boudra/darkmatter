#ifndef ANY_HPP
#define ANY_HPP

#include "TypeId/TypeId.hpp"

namespace dm {

class Any {
   public:
    Any() : m_info(dm_type_info(std::nullptr_t)), m_holder(nullptr) {}

    Any(const Any &other)
        : m_info(other.Type()),
          m_holder(other.m_holder ? other.m_holder->Clone() : nullptr) {}

    Any(Any &&other) : m_info(other.m_info), m_holder(other.m_holder) {
        other.m_holder = nullptr;
    }

    template <typename ValueType>
    Any(const ValueType &data)
        : m_info(dm_type_info(ValueType)),
          m_holder(
              new Holder<typename std::decay<const ValueType>::type>(data)) {}

    template <typename ValueType>
    Any(ValueType &&data,
        typename std::enable_if<!std::is_same<Any &, ValueType>::value>::type
            * = 0,
        typename std::enable_if<!std::is_const<ValueType>::value>::type * = 0)
        : m_info(dm_type_info(ValueType)),
          m_holder(new Holder<typename std::decay<ValueType>::type>(
              std::forward<ValueType>(data))) {}

    template <typename T>
    void Assign(T data) {
        m_info = dm_type_info(T);
        m_holder = new Holder<T>(data);
    }

    template <typename T>
    const T Cast() const {
        return static_cast<Holder<T> *>(m_holder)->data;
    }

    template <typename T>
    T Cast() {
        return static_cast<Holder<T> *>(m_holder)->data;
    }

    const TypeInfo &Type() const { return m_info; }

    struct HolderBase {
        virtual ~HolderBase() {}
        virtual HolderBase *Clone() const = 0;
    };

    template <typename ValueType>
    struct Holder : public HolderBase {
        Holder() : data() {}
        Holder(const ValueType &_data) : data(_data) {}
        Holder(ValueType &&_data) : data(std::forward<ValueType>(_data)) {}

        HolderBase *Clone() const { return new Holder(data); }

        ValueType data;
    };

    template <typename ValueType>
    const Any &operator=(const ValueType &data) {
        Any(data).Swap(*this);
        return *this;
    }

    template <typename ValueType>
    const Any &operator=(ValueType &&data) {
        Any(std::forward<ValueType>(data)).Swap(*this);
        return *this;
    }

    Any &Swap(Any &any) {
        std::swap(m_holder, any.m_holder);
        std::swap(m_info, any.m_info);
        return *this;
    }

    const Any &operator=(const Any &a) {
        Any(a).Swap(*this);
        return *this;
    }

    const Any &operator=(Any &&a) {
        a.Swap(*this);
        Any().Swap(a);
        return *this;
    }

    const bool Empty() const { return m_holder == 0; }

   private:
    TypeInfo m_info;
    HolderBase *m_holder;
};
}

#endif
