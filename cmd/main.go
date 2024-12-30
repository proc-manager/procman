package main

import (
	"os"

	"github.com/rutu-sh/procman"
)

func main() {
	tag := os.Args[1]
	procman.BuildImage("test-img", tag, "./alpine-basic")
	procman.BuildProcessContext("test-proc", "", "test-img", tag)
}
