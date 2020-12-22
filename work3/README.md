## graph_lib.h
Основной файл, содержащий классы для работы с графами.

Два класса `DynamicMatrixGraph<NodeType, EdgeType>`, `DynamicListGraph<NodeType, EdgeType>` для хранения графа в виде матрицы смежности и списка смежности соответственно.

У обоих классов реализованы следующие функции:

- `size_t Size()` 
- `void Resize(size_t new_size)`
- `void SetNode(size_t ind, NodeType node)` 
- `void AddNode(size_t ind, NodeType node)` - `SetNode` и при необходимости `Resize` 
-	`void PushBackNode(NodeType node)` - `AddNode` в конец
-	`NodeType GetNode(size_t ind)`
- `void AddEdge(size_t from, size_t to, EdgeType edge)` - при необходимости `Resize` 
-	`bool HasEdge(size_t from, size_t to)`
-	`EdgeType GetEdge(size_t from, size_t to)`
-	`std::vector<NodeType>& Nodes()`
-	`std::vector<size_t> NeighborsInd(size_t from)`

Для графа в виде матрицы смежности также написаны:
- `void SetEdge(size_t from, size_t to, EdgeType edge)`
- `void RemoveEdge(size_t from, size_t to)`

Класс `NodesList<GraphType>` для хранения `range` на индексах вершин и три функции:
- `NodesBFS<GraphType>(size_t start, GraphType graph)` - индексы вершин в порядке `bfs`
- `NodesDFS<GraphType>(size_t start, GraphType graph)` - индексы вершин в порядке `dfs`
- `NodesInd<GraphType>(GraphType graph)` - индексы вершин в порядке возрастания

## dispatcher.h
Реализация двойной диспетчеризации на графе.
```
template <
	class GraphType,
	class BaseL,
	class BaseR,
	typename ResultType = void,
	typename FuncType = ResultType(*)(BaseL*, BaseR*)
>
class DynamicDispatcher 
```
Пример использования для иерархии классов: 
```
              ---> Human                  ---> Sword
            /                           /
Character -                      Item - 
            \                           \
              ---> Orc                    ---> Shield
```
```
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
```
вывод:
```
human takes sword
human takes shield
orc takes sword
orc takes shield
Error: no suitable function
```

# composite.h
Реализация паттерна композиция на графе.
```
template <
	class GraphType,
	class NodeType
>
class Composite
```
`NodeType` должен быть функцией. Параметры могут быть произвольными. 
Пример использования со следующим графом функций:
```        
f0 -> f1 -> f2
   \
     -> f3
```
```
void Run() {
    Composite<
        DynamicMatrixGraph<std::function<void(int, int)>, std::any>,
        std::function<void(int, int)>
    > composite;

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
```
вывод:
```
f0(1, 2)
f1(1, 2)
f3(1, 2)
f2(1, 2)

f1(1, 2)
f2(1, 2)
```
в первом блоке функции вызваны в порядке `bfs`, начиная с `f0`, а во втором, начиная с `f1`

# chain.h
Реализация паттерна цепочка обязанностей. Последовательно вызываются предикаты в вершинах графа и вызовы завершаются при первом возвращении `false` предикатом или по окончанию цепочки. 
```
template <
	class GraphType,
	class NodeType
>
class Chain 
```
Пример использования на цепочке предикатов 
`a != 0,  b != 0,  a + b != 0 `
```
void Run() {
    Chain<
        DynamicListGraph<std::function<bool(int, int)>, std::any>,
        std::function<bool(int, int)>
    > chain;
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
```
вывод:
```
f0(2, 0)
f1(2, 0)
False

f0(1, 2)
f1(1, 2)
f2(1, 2)
True
```

# graph_lib.cpp
Файл, со всеми использованными примерами в `dispathcer.h` `composite.h` `chain.h`
