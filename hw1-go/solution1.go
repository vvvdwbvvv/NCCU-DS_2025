package scores

import "sort"

type record struct {
	id    uint32
	score uint8
}

type Solution1 struct {
	data     []record
	capacity int
}

func NewSolution1() *Solution1 {
	return &Solution1{
		data:     make([]record, 0, 1),
		capacity: 1,
	}
}

func (s *Solution1) ensureCapacity(extra int) {
	needed := len(s.data) + extra
	if needed <= s.capacity {
		return
	}

	newCap := s.capacity
	if newCap == 0 {
		newCap = 1
	}

	for needed > newCap {
		if newCap > (int(^uint(0)>>1))/10 {
			newCap = needed
			break
		}
		newCap *= 10
	}

	newData := make([]record, len(s.data), newCap)
	copy(newData, s.data)
	s.data = newData
	s.capacity = newCap
}

func (s *Solution1) Insert(id uint32, score uint8) {
	rec := record{id: id, score: score}
	s.ensureCapacity(1)

	pos := sort.Search(len(s.data), func(i int) bool { return s.data[i].id >= id })
	for pos < len(s.data) && s.data[pos].id == id {
		pos++
	}

	s.data = append(s.data, record{})
	copy(s.data[pos+1:], s.data[pos:])
	s.data[pos] = rec
}

func (s *Solution1) Search(id uint32) []int {
	start := sort.Search(len(s.data), func(i int) bool { return s.data[i].id >= id })
	if start == len(s.data) || s.data[start].id != id {
		return []int{-1}
	}

	end := start
	for end < len(s.data) && s.data[end].id == id {
		end++
	}

	out := make([]int, end-start)
	for i := start; i < end; i++ {
		out[i-start] = int(s.data[i].score)
	}

	return out
}
