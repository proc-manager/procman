package procman

import "github.com/rutu-sh/procman/internal/process"

func StartProcess(name string, image_name string, image_tag string, env map[string]string) {
	processCreate := process.ProcessCreate{
		Name: name,
		Env:  env,
		Image: process.ProcessCreateImage{
			Name: image_name,
			Tag:  image_tag,
		},
	}
	process.StartProcess(processCreate)
}
