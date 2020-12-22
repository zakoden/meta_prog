#pragma once

#include "graph_lib.h"

template <
	class GraphType,
	class NodeType
>
class Chain {
private:
	GraphType graph_;
public:
	Chain() : graph_(0) {
	}

	void AddNode(NodeType node) {
		size_t node_ind = graph_.Size();
		graph_.PushBackNode(node);
		if (node_ind > 0) {
			graph_.AddEdge(node_ind - 1, node_ind, 0);
		}
	}

	template <typename ...Args>
	bool Do(Args... args) {
		for (NodeType node : graph_.Nodes()) {
			bool ans = node(args...);
			if (!ans) {
				return false;
			}
		}
		return true;
	}
};