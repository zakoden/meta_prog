#include <iostream>
#include <functional>
#include <any>

#include "graph_lib.h" 
#include "dispatcher.h"
#include "composite.h"
#include "chain.h"

namespace dispatcher_example {
    class Character {
        virtual void f() {};
    };

    class Human : public Character {
        virtual void f() override {};
    };

    class Orc : public Character {
        virtual void f() override {};
    };

    class Item {
        virtual void f() {};
    };

    class Sword : public Item {
        virtual void f() override {};
    };

    class Shield : public Item {
        virtual void f() override {};
    };

    void OrcShield(Character* fir, Item* sec) {
        std::cout << "orc takes shield" << std::endl;
    }

    void Run() {
        Character* human = new Human();
        Orc* orc = new Orc();
        Sword* sword = new Sword();
        Shield* shield = new Shield();
        Item* item = new Item();

        DynamicDispatcher<
            DynamicMatrixGraph<std::type_index, std::function<void(Character*, Item*)>>,
            Character, Item, void, std::function<void(Character*, Item*)>
        > dispatcher;
       
        dispatcher.Add<Human, Sword>([](Character* fir, Item* sec) { 
                std::cout << "human takes sword" << std::endl;
            });
        dispatcher.Add<Human, Shield>([](Character* fir, Item* sec) {
                std::cout << "human takes shield" << std::endl;
            });
        dispatcher.Add<Orc, Sword>([](Character* fir, Item* sec) {
            std::cout << "orc takes sword" << std::endl;
            });
        dispatcher.Add<Orc, Shield>(OrcShield);

        dispatcher.Do(human, sword);
        dispatcher.Do(human, shield);
        dispatcher.Do(orc, sword);
        dispatcher.Do(orc, shield);
        dispatcher.Do(orc, item);
    }

}

namespace composite_example {
    void Run() {
        Composite<
            DynamicMatrixGraph<std::function<void(int, int)>, std::any>,
            std::function<void(int, int)>
        > composite;
        /*
        f0 -> f1 -> f2
          \
            -> f3
        */
        composite.AddNode(0, [](int a, int b) { 
            std::cout << "f0(" << a << ", " << b << ")" << std::endl;
            });
        composite.AddNode(0, [](int a, int b) {
            std::cout << "f1(" << a << ", " << b << ")" << std::endl;
            });
        composite.AddNode(1, [](int a, int b) {
            std::cout << "f2(" << a << ", " << b << ")" << std::endl;
            });
        composite.AddNode(0, [](int a, int b) {
            std::cout << "f3(" << a << ", " << b << ")" << std::endl;
            });
        size_t root;
        root = 0;
        composite.Do<int, int>(root, 1, 2);
        std::cout << std::endl;
        root = 1;
        composite.Do<int, int>(root, 1, 2);
    }
}


namespace chain_example {
    void Run() {
        Chain<
            DynamicListGraph<std::function<bool(int, int)>, std::any>,
            std::function<bool(int, int)>
        > chain;
        /*
        f0 -> f1 -> f2
        */
        chain.AddNode([](int a, int b) {
            std::cout << "f0(" << a << ", " << b << ")" << std::endl;
            return (a != 0);
            });
        chain.AddNode([](int a, int b) {
            std::cout << "f1(" << a << ", " << b << ")" << std::endl;
            return (b != 0);
            });
        chain.AddNode([](int a, int b) {
            std::cout << "f2(" << a << ", " << b << ")" << std::endl;
            return ((a + b) != 0);
            });

        std::cout << (chain.Do<int, int>(2, 0) ? "True" : "False") << std::endl;
        std::cout << std::endl;
        std::cout << (chain.Do<int, int>(1, 2) ? "True" : "False") << std::endl;
    }
}

int main() {
    std::cout << "------------------------" << std::endl;
    dispatcher_example::Run();
    std::cout << "------------------------" << std::endl;

    std::cout << "------------------------" << std::endl;
    composite_example::Run();
    std::cout << "------------------------" << std::endl;

    std::cout << "------------------------" << std::endl;
    chain_example::Run();
    std::cout << "------------------------" << std::endl;
    return 0;
}

