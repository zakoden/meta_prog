#pragma once

#include "graph_lib.h"
#include <typeindex>

template <
	class GraphType,
	class BaseL,
	class BaseR,
	typename ResultType = void,
	typename FuncType = ResultType(*)(BaseL*, BaseR*)
>
class DynamicDispatcher {
private:
	GraphType graph_;

	size_t FindNode(std::type_index value, bool create = false) {
		size_t pos = 0;
		for (auto node : graph_.Nodes()) {
			if (node == value) {
				return pos;
			}
			pos++;
		}
		if (create) {
			graph_.PushBackNode(value);
		}
		return pos;
	}

public:
	DynamicDispatcher() : graph_(0) {

	}
	
	template <class TypeL, class TypeR>
	void Add(FuncType func) {
		size_t node1 = FindNode(std::type_index(typeid(TypeL)), true);
		size_t node2 = FindNode(std::type_index(typeid(TypeR)), true);
		graph_.AddEdge(node1, node2, func);
	}

	ResultType Do(BaseL* left, BaseR* right) {
		size_t node1 = FindNode(std::type_index(typeid(*left)));
		size_t node2 = FindNode(std::type_index(typeid(*right)));
		if (graph_.HasEdge(node1, node2)) {
			return graph_.GetEdge(node1, node2)(left, right);
		} else {
			std::cerr << "Error: no suitable function" << std::endl;
			return ResultType();
		}
	}

};
