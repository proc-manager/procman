package process

import (
	"fmt"
	"os"
	"strings"

	"github.com/google/uuid"
	"github.com/rutu-sh/procman/internal/common"
	"github.com/rutu-sh/procman/internal/image"
)

func buildProcessContext(name string, image_name string, image_tag string) (*Process, *common.ProcStartErr) {
	_logger := common.GetLogger()

	_logger.Info().Msgf("building process context with params (%v, %v, %v, %v)", name, "", image_name, image_tag)

	img, err := image.GetImage("", image_name, image_tag)
	if img == nil || err != nil {
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

	if errRun := run([]string{"cp", fmt.Sprintf("%v/img.tar.gz", img.ImgPath), proc.ContextDir}); errRun != nil {
		return nil, &common.ProcStartErr{Code: 500, Message: fmt.Sprintf("error copying: %v", err)}
	}

	wd, errGetWd := os.Getwd()
	if errGetWd != nil {
		return nil, &common.ProcStartErr{Code: 500, Message: fmt.Sprintf("error getting wd: %v", err)}
	}
	if errchdir := os.Chdir(proc.ContextDir); errchdir != nil {
		return nil, &common.ProcStartErr{Code: 500, Message: fmt.Sprintf("error changing dir: %v", err)}
	}
	if errRun := run([]string{"tar", "-xf", "img.tar.gz"}); errRun != nil {
		return nil, &common.ProcStartErr{Code: 500, Message: fmt.Sprintf("error unarchiving: %v", err)}
	}
	if errRun := run([]string{"rm", "img.tar.gz"}); errRun != nil {
		return nil, &common.ProcStartErr{Code: 500, Message: fmt.Sprintf("error removing: %v", err)}
	}
	if errchdir := os.Chdir(wd); errchdir != nil {
		return nil, &common.ProcStartErr{Code: 500, Message: fmt.Sprintf("error changing dir: %v", err)}
	}

	_logger.Info().Msgf("built process context (%v, %v, %v)", name, image_name, image_tag)

	return proc, nil
}
