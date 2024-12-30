package process

import (
	"fmt"
	"strings"

	"github.com/google/uuid"
	"github.com/rutu-sh/procman/internal/common"
	"github.com/rutu-sh/procman/internal/image"
)

func BuildProcessContext(name string, image_id string, image_name string, image_tag string) (*Process, *common.ProcStartErr) {
	_logger := common.GetLogger()

	_logger.Info().Msgf("starting process with params (%v, %v, %v, %v)", name, image_id, image_name, image_tag)

	img, err := image.GetImage(image_id, image_name, image_tag)
	if err != nil {
		_logger.Error().Msgf("error reading image: %v", err)
		return nil, &common.ProcStartErr{Code: 500, Message: fmt.Sprintf("error reading image: %v", err)}
	}

	uid := strings.Split(uuid.New().String(), "-")[0]
	procDir := getProcessDir(uid)

	proc := &Process{
		Id:         uid,
		Image:      *img,
		ContextDir: procDir,
	}

	commands := [][]string{
		{"cp", fmt.Sprintf("%v/img.tar.gz", img.ImgPath), proc.ContextDir},
		{"tar", "-xf", fmt.Sprintf("%v/img.tar.gz", proc.ContextDir)},
		{"rm", fmt.Sprintf("%v/img.tar.gz", proc.ContextDir)},
	}
	for _, cmd := range commands {
		if err := runCmd([]string{}, cmd[0], cmd[1:]...); err != nil {
			_logger.Error().Msgf("error running command: %v", err)
			return nil, &common.ProcStartErr{Code: 500, Message: fmt.Sprintf("error running command image: %v", err)}
		}
	}
	return proc, nil
}
