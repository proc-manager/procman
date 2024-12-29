package procman

type Image struct {
	Id      string `yaml:"id"`
	Name    string `yaml:"name"`
	ImgPath string `yaml:"imgpath,omitempty"`
	Tag     string `yaml:"tag,omitempty"`
	Created string `yaml:"created,omitempty"`
}
