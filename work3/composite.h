#pragma once

#include "graph_lib.h"

template <
	class GraphType,
	class NodeType
>
class Composite {
private:
	GraphType graph_;
public:
	Composite() : graph_(0) {
	}

	void AddNode(size_t from, NodeType node) {
		size_t node_ind = graph_.Size();
		graph_.PushBackNode(node);
		if (node_ind > 0) {
			graph_.AddEdge(from, node_ind, 0);
		}
	}

	template <typename ...Args>
	void Do(size_t from, Args... args) {
		for (size_t ind : NodesBFS(from, graph_)) {
			graph_.GetNode(ind)(args...);
		}
	}
};