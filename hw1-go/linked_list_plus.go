package scores

import "sort"

type node struct {
	id    uint32
	score uint8
	next  int
}

type LinkedListPlus struct {
	nodes []node
	head  int
}

func NewLinkedListPlus() *LinkedListPlus {
	return &LinkedListPlus{head: -1}
}

func (l *LinkedListPlus) rebuildLinks() {
	if len(l.nodes) == 0 {
		l.head = -1
		return
	}

	l.head = 0
	for i := range l.nodes {
		if i+1 < len(l.nodes) {
			l.nodes[i].next = i + 1
		} else {
			l.nodes[i].next = -1
		}
	}
}

func (l *LinkedListPlus) Insert(id uint32, score uint8) {
	if id == 0 || id > MaxID {
		return
	}

	pos := sort.Search(len(l.nodes), func(i int) bool { return l.nodes[i].id >= id })
	for pos < len(l.nodes) && l.nodes[pos].id == id {
		pos++
	}

	l.nodes = append(l.nodes, node{})
	copy(l.nodes[pos+1:], l.nodes[pos:])
	l.nodes[pos] = node{id: id, score: score, next: -1}
	l.rebuildLinks()
}

func (l *LinkedListPlus) Search(id uint32) []int {
	start := sort.Search(len(l.nodes), func(i int) bool { return l.nodes[i].id >= id })
	if start == len(l.nodes) || l.nodes[start].id != id {
		return []int{-1}
	}

	end := start
	for end < len(l.nodes) && l.nodes[end].id == id {
		end++
	}

	out := make([]int, end-start)
	for i := start; i < end; i++ {
		out[i-start] = int(l.nodes[i].score)
	}

	return out
}
