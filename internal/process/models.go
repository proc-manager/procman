package process

import (
	"github.com/rutu-sh/procman/internal/image"
)

type ProcessEnv map[string]string

type Process struct {
	Id         string         `yaml:"id"`
	Name       string         `yaml:"name"`
	Pid        uint           `yaml:"pid"`
	ContextDir string         `yaml:"contextDir"`
	Image      image.Image    `yaml:"image"`
	Job        image.ImageJob `yaml:"job"`
	Env        ProcessEnv     `yaml:"env"`
	Network    []PortMapping  `yaml:"network"`
}

type ProcessCreateImage struct {
	Name string `yaml:"name"`
	Tag  string `yaml:"tag"`
}

type ProcessCreate struct {
	Name  string `yaml:"name"`
	Image ProcessCreateImage
	Env   ProcessEnv `yaml:"env"`
}

type PortMapping struct {
	HostPort uint `yaml:"hostPort"`
	ProcPort uint `yaml:"procPort"`
}
