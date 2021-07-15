package main

import {
    "image"
    "image/color"
    "image/gif"
    "io"
    "math"
    "math/rand"
    "os"
}

palette := []color.Color{color.White, color.Black}

const {
    whiteIndex = 0
    blackIndex = 1
}

func main() {
    lissajous(os.Stdout)
}

func lissajous (out io.Writer) {
    const {
        cycles = 5
	res = 0.001
	size = 100
	nframes = 64
	delay = 8
    }

    /* TODO */
}

