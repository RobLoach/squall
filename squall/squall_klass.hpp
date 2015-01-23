#ifndef SQUALL_KLASS_HPP_
#define SQUALL_KLASS_HPP_

#include <functional>

#include "squall_vm.hpp"
#include "squall_defun.hpp"
#include "squall_defvar.hpp"

namespace squall {

template <class C, class Base = void>
class Klass {
public:
    Klass(VM& vm, const string& name) : vm_(vm) {
        imp_ = vm.klass_table().add_klass<C, Base>(vm_.handle(), name);
    }
    ~Klass() { imp_.lock()->close(); }

    Klass(const Klass&) = delete;
    Klass(const Klass&&) = delete;
    void operator=(const Klass&) = delete;
    void operator=(const Klass&&) = delete;
    
    template <class F>
    Klass<C, Base>& func(const char* name, F f) {
        func(string(name), f);
        return *this;
    }

    template <class F>
    Klass<C, Base>& func(const string& name, F f) {
        detail::defun_local(
            vm_.handle(),
            imp_.lock()->get_klass_object(),
            name,
            to_function(f));
        return *this;
    }

    template <class V>
    Klass<C, Base>& var(const string& name, V C::* r) {
        detail::defvar_local(
            vm_.handle(),
            imp_.lock()->get_getter_table(),
            name,
            r);
        return *this;
    }

private:
    VM& vm_;
    std::weak_ptr<detail::KlassImp<C>> imp_;

};

}

#endif // SQUALL_KLASS_HPP_
