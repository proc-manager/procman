package images

import (
	"fmt"
	"os"
	"os/exec"

	"github.com/rutu-sh/procman/internal/common"
)

func getParentImgDir() string {
	return "/var/lib/procman/img"
}

func getImageContextDir(img_id string) string {
	return fmt.Sprintf("%v/%v/rootfs", getParentImgDir(), img_id)
}

func getImageDir(img_id string) string {
	return fmt.Sprintf("%v/%v", getParentImgDir(), img_id)
}

func runCmd(env []string, command string, args ...string) *common.ImageBuildErr {
	_logger := common.GetLogger()

	_logger.Info().Msgf("executing command: %v with args: %v", command, args)

	cmd := exec.Command(command, args...)
	if len(env) > 0 {
		cmd.Env = env
	}
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin

	if err := cmd.Run(); err != nil {
		_logger.Error().Msgf("error running command: %v", err)
		return &common.ImageBuildErr{
			Code:    500,
			Message: "error running the command",
		}
	}

	_logger.Info().Msgf("command %v executed", command)

	return nil
}
