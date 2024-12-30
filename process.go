package procman

import "github.com/rutu-sh/procman/internal/process"

func BuildProcessContext(name string, image_id string, image_name string, image_tag string) {
	process.BuildProcessContext(name, image_id, image_name, image_tag)
}
