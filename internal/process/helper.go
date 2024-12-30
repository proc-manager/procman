package process

import (
	"fmt"
	"os"
	"os/exec"

	"github.com/rutu-sh/procman/internal/common"
)

func createDirIfNotExists(dirpath string) {
	if _, err := os.Stat(dirpath); err != nil {
		if os.IsNotExist(err) {
			os.MkdirAll(dirpath, 0755)
		}
	}
}

func getAllProcDir() string {
	dir := "/var/lib/procman/proc"
	os.MkdirAll(dir, 0755)
	return dir
}

/*
Do not confuse process_id with process_pid
*/
func getProcessDir(process_id string) string {
	dir := fmt.Sprintf("%v/%v", getAllProcDir(), process_id)
	os.MkdirAll(dir, 0755)
	return dir
}

func runCmd(env []string, command string, args ...string) *common.ProcStartErr {
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
		return &common.ProcStartErr{
			Code:    500,
			Message: "error running the command",
		}
	}

	_logger.Info().Msgf("command %v executed", command)

	return nil
}
