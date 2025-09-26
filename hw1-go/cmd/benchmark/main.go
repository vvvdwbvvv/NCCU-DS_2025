package main

import (
	"fmt"
	"log"
)

func main() {
	results, err := runBenchmarks()
	if err != nil {
		log.Fatalf("benchmark failed: %v", err)
	}

	outputPath := "benchmark_results.csv"
	if err := writeCSV(results, outputPath); err != nil {
		log.Fatalf("failed to write results: %v", err)
	}

	fmt.Printf("benchmark complete, results written to %s\n", outputPath)
}
