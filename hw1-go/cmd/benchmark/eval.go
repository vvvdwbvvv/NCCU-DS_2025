package main

import (
	"encoding/csv"
	"errors"
	"fmt"
	"os"
	"time"

	rand "math/rand/v2"

	scores "hw1-go"
)

const streamInc = uint64(0x9e3779b97f4a7c15)
const maxDuration = 10 * time.Minute

type benchmarkResult struct {
	inserts     int
	solution    string
	insertAvg   time.Duration
	searchAvg   time.Duration
	sumAvg      time.Duration
	estimated   bool
	estimateSrc string
}

func runOne(store scores.Store, inserts int, rng *rand.Rand) (insertDur, searchDur, sumDur time.Duration) {
	ids := make([]uint32, inserts)
	scoresSlice := make([]uint8, inserts)
	for i := 0; i < inserts; i++ {
		ids[i] = uint32(rng.IntN(scores.MaxID) + 1)
		scoresSlice[i] = uint8(rng.IntN(101))
	}

	start := time.Now()
	for i := 0; i < inserts; i++ {
		store.Insert(ids[i], scoresSlice[i])
	}
	insertDur = time.Since(start)

	searchIDs := make([]uint32, 100_000)
	for i := range searchIDs {
		searchIDs[i] = uint32(rng.IntN(scores.MaxID) + 1)
	}

	start = time.Now()
	for _, id := range searchIDs {
		_ = store.Search(id)
	}
	searchDur = time.Since(start)

	unique := make(map[uint32]struct{}, len(ids))
	for _, id := range ids {
		unique[id] = struct{}{}
	}

	start = time.Now()
	total := 0
	for id := range unique {
		scores := store.Search(id)
		for _, sc := range scores {
			if sc >= 0 {
				total += sc
			}
		}
	}
	_ = total
	sumDur = time.Since(start)

	return
}

func estimateNextTime(prevPoints []time.Duration, prevInserts []int, currentInserts int) time.Duration {
	if len(prevPoints) == 0 {
		return 0
	}

	if len(prevPoints) == 1 || prevPoints[len(prevPoints)-1] == 0 {
		return prevPoints[len(prevPoints)-1]
	}

	lastTime := prevPoints[len(prevPoints)-1]
	prevTime := prevPoints[len(prevPoints)-2]
	if prevTime == 0 {
		return lastTime
	}

	ratio := float64(lastTime) / float64(prevTime)
	if ratio < 1 {
		ratio = float64(currentInserts) / float64(prevInserts[len(prevInserts)-1])
	}

	estimated := time.Duration(float64(lastTime) * ratio)
	if estimated < lastTime {
		estimated = lastTime
	}

	return estimated
}

func runBenchmarks() ([]benchmarkResult, error) {
	dataPoints := []int{
		1_000, 2_000, 4_000, 8_000, 16_000,
		32_000, 64_000, 128_000, 256_000, 512_000,
		1_024_000, 2_048_000, 4_096_000, 8_192_000, 16_384_000,
	}

	repeats := 10
	solutions := []struct {
		name string
		new  func() scores.Store
	}{
		{"Solution1", func() scores.Store { return scores.NewSolution1() }},
		{"Solution2", func() scores.Store { return scores.NewSolution2() }},
		{"LinkedListPlus", func() scores.Store { return scores.NewLinkedListPlus() }},
	}

	results := make([]benchmarkResult, 0, len(dataPoints)*len(solutions))
	seed := uint64(time.Now().UnixNano())

	for _, sol := range solutions {
		var insertHistory, searchHistory, sumHistory []time.Duration
		var insertCounts []int

		for _, dp := range dataPoints {
			fmt.Printf("== %s, inserts %d ==\n", sol.name, dp)

			var insertTotal, searchTotal, sumTotal time.Duration
			var timedOut bool

			for r := 0; r < repeats; r++ {
				rng := rand.New(rand.NewPCG(seed+uint64(dp)+uint64(r), (seed^streamInc)+uint64(r)))

				runStart := time.Now()
				insertDur, searchDur, sumDur := runOne(sol.new(), dp, rng)
				runDur := time.Since(runStart)

				if runDur > maxDuration {
					fmt.Printf("    repeat %d/%d timed out (%v)\n", r+1, repeats, runDur)
					timedOut = true
					break
				}

				insertTotal += insertDur
				searchTotal += searchDur
				sumTotal += sumDur

				fmt.Printf("    repeat %d/%d -> insert=%v search=%v sum=%v\n", r+1, repeats, insertDur, searchDur, sumDur)
			}

			res := benchmarkResult{
				inserts:  dp,
				solution: sol.name,
			}

			if timedOut {
				res.estimated = true
				res.estimateSrc = "extrapolated from previous points"

				res.insertAvg = estimateNextTime(insertHistory, insertCounts, dp)
				res.searchAvg = estimateNextTime(searchHistory, insertCounts, dp)
				res.sumAvg = estimateNextTime(sumHistory, insertCounts, dp)
			} else {
				res.insertAvg = insertTotal / time.Duration(repeats)
				res.searchAvg = searchTotal / time.Duration(repeats)
				res.sumAvg = sumTotal / time.Duration(repeats)

				insertHistory = append(insertHistory, res.insertAvg)
				searchHistory = append(searchHistory, res.searchAvg)
				sumHistory = append(sumHistory, res.sumAvg)
				insertCounts = append(insertCounts, dp)
			}

			results = append(results, res)
		}
	}

	return results, nil
}

func writeCSV(results []benchmarkResult, path string) error {
	if len(results) == 0 {
		return errors.New("no benchmark results to write")
	}

	file, err := os.Create(path)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := csv.NewWriter(file)
	defer writer.Flush()

	header := []string{"solution", "inserts", "insert_ms", "search_ms", "sum_ms", "estimated", "estimate_src"}
	if err := writer.Write(header); err != nil {
		return err
	}

	for _, res := range results {
		record := []string{
			res.solution,
			fmt.Sprintf("%d", res.inserts),
			fmt.Sprintf("%.3f", res.insertAvg.Seconds()*1000),
			fmt.Sprintf("%.3f", res.searchAvg.Seconds()*1000),
			fmt.Sprintf("%.3f", res.sumAvg.Seconds()*1000),
			fmt.Sprintf("%t", res.estimated),
			res.estimateSrc,
		}
		if err := writer.Write(record); err != nil {
			return err
		}
	}

	writer.Flush()
	return writer.Error()
}
