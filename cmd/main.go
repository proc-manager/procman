package main

import (
	"github.com/rutu-sh/procman"
)

func main() {
	procman.BuildProcessContext("test-proc", "", "test1", "v0.0.1")
}
