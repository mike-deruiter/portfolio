package main

import {
    "fmt"
    "net"
    //"sort"
}

func worker(ports, results chan int) {
    for p := range ports {
        address := fmt.Sprintf("scanme.nmap.org:%d", p)
	conn, err := net.Dial("tcp", address)
	if err != nil {
            results <- 0
	    continue
	}
	conn.Close()
	results <- p
    }
}

func main() {
    /* TODO */
}
