package main

import (
	"os"

	"github.com/rutu-sh/procman"
)

func main() {
	tag := os.Args[1]
	procman.BuildImage("test-img", tag, "./alpine-basic")
	env := make(map[string]string)
	procman.StartProcess("test-proc", "test-img", tag, env)
}
