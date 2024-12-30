package process

import (
	"github.com/rutu-sh/procman/internal/image"
)

type Process struct {
	Id         string         `yaml:"id"`
	Name       string         `yaml:"name"`
	Pid        uint           `yaml:"pid"`
	ContextDir string         `yaml:"context_dir"`
	Image      image.Image    `yaml:"image"`
	Job        image.ImageJob `yaml:"job"`
}
