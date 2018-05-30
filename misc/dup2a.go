// dup2a - Adapted from THE GO PROGRAMMING LANGUAGE by Brian Kernighan
// & Alan Donovan

package main

import (
    "bufio"
    "fmt"
    "unicode"
    "os"
)

func main() {
    counts := make(map[string]map[string]int)
    files  := os.Args[1:]
    
    if len(files) == 0 {
        countLines(os.Stdin, "stdin", counts)
    } else {
        for _, arg := range files {
            f, err := os.Open(arg)
            if err != nil {
                fmt.Fprintf(os.Stderr, "%s: %v\n", os.Args[0], err)
                continue
            }
            countLines(f, arg, counts)
            f.Close()
        }
    }
    
    for filename, file := range counts {
        for line, n := range file {
            if n > 1 {
                fmt.Printf("%s\t%d\t%s\n", filename, n, line)
            }
        }
    }
}

func countLines(f *os.File, n string, counts map[string]map[string]int) {
    counts[n] = make(map[string]int)
    input := bufio.NewScanner(f)
    for input.Scan() {
        blank := true
        for _, char := range input.Text() {
            if ! unicode.IsSpace(char) {
                blank = false
            }
        }
        if ! blank {
            counts[n][input.Text()]++
        }
    }
}
