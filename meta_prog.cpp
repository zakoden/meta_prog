#include <iostream>

class Nulltype {};

/*
|------------------------------------|
|            TypeList                |
|------------------------------------|
*/

template <typename ...Args>
struct TypeList
{
    using Head = Nulltype;
    using Tail = Nulltype;
};

template <typename H, typename ...T>
struct TypeList<H, T...>
{
    using Head = H;
    using Tail = TypeList<T...>;
};

typedef TypeList<> EmptyTypeList;

template <typename H>
struct TypeList<H>
{
    using Head = H;
    using Tail = EmptyTypeList;
};


/*
|------------------------------------|
|       TypeList functions           |
|------------------------------------|
*/

template <size_t ind, typename ...T>
struct TypeAt {
    using res = typename Nulltype;
};

template <size_t ind, typename ...T>
struct TypeAt<ind, TypeList<T...>>
{
    using res = typename TypeAt<ind - 1, typename TypeList<T...>::Tail>::res;
};

template <size_t ind>
struct TypeAt<ind, EmptyTypeList>
{
    using res = typename Nulltype;
};

template <size_t ind>
struct TypeAt<ind, Nulltype>
{
    using res = typename Nulltype;
};

template <typename ...T>
struct TypeAt<0, TypeList<T...>>
{
    using res = typename TypeList<T...>::Head;
};


/*
|------------------------------------|
|            StructComp              |
|------------------------------------|
*/

template<typename A, typename B>
struct StructComp
{
    static constexpr bool equal = false;
};

template<typename A>
struct StructComp<A, A>
{
    static constexpr bool equal = true;
};


/*
|------------------------------------|
|             DerHolder              |
|------------------------------------|
*/

template <typename T, class Base>
struct DerHolder
    : public Base
{
    T _obj;
    size_t sz = sizeof(T);
    void Draw() {
        _obj.Draw();
        Base::Draw();
    }
};

template <typename T>
struct DerHolder<Nulltype, T>
{
    Nulltype _obj;
    size_t sz = sizeof(Nulltype);
    void Draw() {
    }
};

template <>
struct DerHolder<Nulltype, Nulltype>
{
    Nulltype _obj;
    size_t sz = sizeof(Nulltype);
    void Draw() {
    }
};


/*
|------------------------------------|
|         GenCustomHierarchy         |
|------------------------------------|
*/

/*
GenCustomHierarchy

7 8 9   10 11 12   13 14 15
\ | /     \ | /      \ | /
  4         5          6
  ^         ^          ^
  |        ind         |
  left_ind            right_ind
*/

template <class TList, template <class T, class Base> class Unit, size_t ind = 0, size_t left_ind = 0, size_t right_ind = 0, bool empty_root = false>
struct GenCustomHierarchy;

template <size_t num, template <class T, class Base> class Unit, size_t ind, size_t left_ind, size_t right_ind, class ...TList>
using ChildBranchN =
    Unit<typename TypeAt<right_ind + num - 1 + (ind - left_ind) * 3, TypeList<TList...>>::res,                                    // up 
    Unit<typename TypeAt<right_ind + num - 1 + (ind - left_ind) * 3 + (right_ind - left_ind + 1) * 3, TypeList<TList...>>::res,   // up
    GenCustomHierarchy<
        TypeList<TList...>, 
        Unit, 
        right_ind + num + (ind - left_ind) * 3 + (right_ind - left_ind + 1) * 3,         // new ind
        (right_ind - left_ind + 1) * 3 + right_ind + 1,                                  // new left_ind
        (right_ind - left_ind + 1) * 3 + right_ind + (right_ind - left_ind + 1) * 3,     // new right_ind
        StructComp<   // stop when Nulltype
            typename TypeAt<right_ind + num - 1 + (ind - left_ind) * 3 + (right_ind - left_ind + 1) * 3, TypeList<TList...>>::res, // new ind type
            Nulltype
        >::equal
    >>>;

template <class ...TList, template <class T, class Base> class Unit, size_t ind, size_t left_ind, size_t right_ind>
struct GenCustomHierarchy<TypeList<TList...>, Unit, ind, left_ind, right_ind, false> : 
    public ChildBranchN<1, Unit, ind, left_ind, right_ind, TList...>,
    public ChildBranchN<2, Unit, ind, left_ind, right_ind, TList...>,
    public ChildBranchN<3, Unit, ind, left_ind, right_ind, TList...>
{
    using first_child = ChildBranchN<1, Unit, ind, left_ind, right_ind, TList...>;
    using second_child = ChildBranchN<2, Unit, ind, left_ind, right_ind, TList...>;
    using third_child = ChildBranchN<3, Unit, ind, left_ind, right_ind, TList...>;

    void Draw() {
        std::cout << "ind = " << ind << " childs begin (" << std::endl;
        first_child::Draw();
        second_child::Draw();
        third_child::Draw();
        std::cout << ") ind = " << ind << " childs end" << std::endl;
    }
};

template <class ...TList, template <class T, class Base> class Unit, size_t ind, size_t left_ind, size_t right_ind>
struct GenCustomHierarchy<TypeList<TList...>, Unit, ind, left_ind, right_ind, true>
{
    void Draw() {
    }
};



/*
|------------------------------------|
|                Test                |
|------------------------------------|
*/

template <size_t ind>
struct A {
    void Draw() {
        std::cout << "ind = " << ind << std::endl;
    }
};

int main() {
    GenCustomHierarchy<TypeList< A<1>, A<2>, A<3>, A<4>, A<5>, A<6>, A<7>, A<8> >, DerHolder> test_tree;
    test_tree.Draw();

    return 0;
}