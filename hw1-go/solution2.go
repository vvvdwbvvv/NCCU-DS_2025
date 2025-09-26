package scores

type linkNode struct {
	score uint8
	next  int
}

type Solution2 struct {
	heads []int
	nodes []linkNode
}

func NewSolution2() *Solution2 {
	heads := make([]int, MaxID+1)
	for i := range heads {
		heads[i] = -1
	}
	return &Solution2{
		heads: heads,
		nodes: make([]linkNode, 0),
	}
}

func (s *Solution2) Insert(id uint32, score uint8) {
	if id == 0 || id > MaxID {
		return
	}

	idx := int(id)
	nodeIndex := len(s.nodes)
	s.nodes = append(s.nodes, linkNode{
		score: score,
		next:  s.heads[idx],
	})
	s.heads[idx] = nodeIndex
}

func (s *Solution2) Search(id uint32) []int {
	if id == 0 || id > MaxID {
		return []int{-1}
	}

	idx := int(id)
	current := s.heads[idx]
	if current == -1 {
		return []int{-1}
	}

	scores := make([]int, 0)
	for current != -1 {
		node := s.nodes[current]
		scores = append(scores, int(node.score))
		current = node.next
	}

	if len(scores) == 0 {
		return []int{-1}
	}

	for i, j := 0, len(scores)-1; i < j; i, j = i+1, j-1 {
		scores[i], scores[j] = scores[j], scores[i]
	}

	return scores
}
