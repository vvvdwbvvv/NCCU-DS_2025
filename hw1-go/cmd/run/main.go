package main

import (
	"fmt"
	"time"

	scores "hw1-go"
)

func main() {
	seed := time.Now().Unix()
	scores.RunWorkload(scores.NewSolution1(), 1000, 1000, seed)
	scores.RunWorkload(scores.NewSolution2(), 1000, 1000, seed)
	scores.RunWorkload(scores.NewLinkedListPlus(), 1000, 1000, seed)
	fmt.Println("workload finished")
}
