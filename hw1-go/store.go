package scores

import "math/rand"

const MaxID = 1 << 20

type Store interface {
	Insert(id uint32, score uint8)
	Search(id uint32) []int
}

func RunWorkload(store Store, inserts, searches int, seed int64) {
	rng := rand.New(rand.NewSource(seed))

	for i := 0; i < inserts; i++ {
		id := uint32(rng.Intn(MaxID) + 1)
		score := uint8(rng.Intn(101))
		store.Insert(id, score)
	}

	for i := 0; i < searches; i++ {
		id := uint32(rng.Intn(MaxID) + 1)
		_ = store.Search(id)
	}
}
