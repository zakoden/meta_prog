#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <queue>

template <typename NodeType, typename EdgeType>
class DynamicMatrixGraph {
private:
	std::vector<std::vector<EdgeType>> matrix_;
	std::vector<std::vector<bool>> exist_;
	std::vector<NodeType> nodes_;

public:

	DynamicMatrixGraph(size_t n) {
		matrix_.resize(n);
		exist_.resize(n);
		for (size_t i = 0; i < n; ++i) {
			matrix_[i].resize(n);
			exist_[i].resize(n, false);
		}
	}

	size_t Size() const {
		return matrix_.size();
	}

	void Resize(size_t new_size) {
		matrix_.resize(new_size);
		exist_.resize(new_size);
		for (size_t i = 0; i < new_size; ++i) {
			matrix_[i].resize(new_size);
			exist_[i].resize(new_size, false);
		}
	}

	void RemoveEdge(size_t from, size_t to) {
		exist_[from][to] = false;
	}

	void SetEdge(size_t from, size_t to, EdgeType edge) {
		matrix_[from][to] = edge;
		exist_[from][to] = true;
	}

	void SetNode(size_t ind, NodeType node) {
		nodes_[ind] = node;
	}

	void AddEdge(size_t from, size_t to, EdgeType edge) {
		size_t new_size = std::max(from, to) + 1;
		if (new_size > Size()) {
			Resize(new_size);
		}
		matrix_[from][to] = edge;
		exist_[from][to] = true;
	}

	void AddNode(size_t ind, NodeType node) {
		size_t new_size = ind + 1;
		if (new_size > Size()) {
			Resize(new_size);
		}
		nodes_[ind] = node;
	}

	void PushBackNode(NodeType node) {
		nodes_.push_back(node);
		Resize(Size() + 1);
	}

	bool HasEdge(size_t from, size_t to) {
		if (from >= Size() || to >= Size()) {
			return false;
		}
		return exist_[from][to];
	}

	EdgeType GetEdge(size_t from, size_t to) const {
		return matrix_[from][to];
	}

	NodeType GetNode(size_t ind) const {
		return nodes_[ind];
	}

	std::vector<NodeType>& Nodes() {
		return nodes_;
	}

	const std::vector<NodeType>& Nodes() const {
		return nodes_;
	}
	
	std::vector<size_t> NeighborsInd(size_t from) const {
		std::vector<size_t> res;
		for (size_t to = 0; to < Size(); ++to) {
			if (exist_[from][to]) {
				res.push_back(to);
			}
		}
		return res;
	}
};


template <typename NodeType, typename EdgeType>
class DynamicListGraph {
private:
	std::vector<std::vector<std::pair<size_t, EdgeType>>> list_;
	std::vector<NodeType> nodes_;

public:

	DynamicListGraph(size_t n) {
		list_.resize(n);
	}

	size_t Size() const {
		return list_.size();
	}

	void Resize(size_t new_size) {
		list_.resize(new_size);
	}

	void SetNode(size_t ind, NodeType node) {
		nodes_[ind] = node;
	}

	void AddEdge(size_t from, size_t to, EdgeType edge) {
		size_t new_size = std::max(from, to) + 1;
		if (new_size > Size()) {
			Resize(new_size);
		}
		list_[from].push_back(make_pair(to, edge));
	}

	void AddNode(size_t ind, NodeType node) {
		size_t new_size = ind + 1;
		if (new_size > Size()) {
			Resize(new_size);
		}
		nodes_[ind] = node;
	}

	void PushBackNode(NodeType node) {
		nodes_.push_back(node);
		Resize(Size() + 1);
	}

	bool HasEdge(size_t from, size_t to) {
		if (from >= Size() || to >= Size()) {
			return false;
		}
		for (auto cur_pair : list_[from]) {
			if (cur_pair.first == to) {
				return true;
			}
		}
		return false;
	}

	EdgeType GetEdge(size_t from, size_t to) const {
		for (auto cur_pair : list_[from]) {
			if (cur_pair.first == to) {
				return cur_pair.second;
			}
		}
	}

	NodeType GetNode(size_t ind) const {
		return nodes_[ind];
	}

	std::vector<NodeType>& Nodes() {
		return nodes_;
	}

	const std::vector<NodeType>& Nodes() const {
		return nodes_;
	}

	std::vector<size_t> NeighborsInd(size_t from) const {
		std::vector<size_t> res = list_[from];
		return res;
	}
};

template<class GraphType>
class NodesList {
private:
	std::vector<size_t> nodes_;
public:
	using iterator = typename std::vector<size_t>::iterator;
	using const_iterator = typename std::vector<size_t>::const_iterator;

	void PushBack(size_t ind) {
		nodes_.push_back(ind);
	}

	iterator begin() {
		return nodes_.begin();
	}

	const_iterator begin() const {
		return nodes_.begin();
	}

	iterator end() {
		return nodes_.end();
	}

	const_iterator end() const {
		return nodes_.end();
	}
};


template<class GraphType> 
NodesList<GraphType> NodesBFS(size_t start, GraphType graph) {
	NodesList<GraphType> res;

	std::queue<size_t> q;
	q.push(start);
	while (!q.empty()) {
		size_t cur = q.front();
		q.pop();
		res.PushBack(cur);
		for (size_t next : graph.NeighborsInd(cur)) {
			q.push(next);
		}
	}
	return res;
}

template<class GraphType>
NodesList<GraphType> NodesDFS(size_t start, GraphType graph) {
	NodesList<GraphType> res;

	std::vector<size_t> q;
	q.push_back(start);
	while (!q.empty()) {
		size_t cur = q.back();
		q.pop_back();
		res.PushBack(cur);
		for (size_t next : graph.NeighborsInd(cur)) {
			q.push_back(next);
		}
	}
	return res;
}

template<class GraphType>
NodesList<GraphType> NodesInd(GraphType graph) {
	NodesList<GraphType> res;
	for (size_t i = 0; i < graph.Size(); ++i) {
		res.PushBack(i);
	}
	return res;
}
