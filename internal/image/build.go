package image

import (
	"fmt"
	"os"
	"strings"
	"syscall"
	"time"

	"github.com/rutu-sh/procman/internal/common"

	"gopkg.in/yaml.v3"
)

func parseImageSpec(img_context string) (*ImageSpec, *common.ImageBuildErr) {
	_logger := common.GetLogger()

	spec_file := fmt.Sprintf("%v/ImageSpec.yaml", img_context)
	_logger.Info().Msgf("parsing image spec yaml at: %v", spec_file)

	if _, err := os.Stat(spec_file); err != nil {
		_logger.Error().Msgf("file %v not found", spec_file)
		return nil, &common.ImageBuildErr{
			Code:    500,
			Message: fmt.Sprintf("file %v not found", spec_file),
		}
	}

	data, err := os.ReadFile(spec_file)
	if err != nil {
		return nil, &common.ImageBuildErr{
			Code:    500,
			Message: fmt.Sprintf("error reading file %v: %v", spec_file, err),
		}
	}

	parsed_spec := ImageSpec{}
	err = yaml.Unmarshal(data, &parsed_spec)
	if err != nil {
		_logger.Error().Msgf("error unmarshal: %v", err)
		return nil, &common.ImageBuildErr{
			Code:    500,
			Message: fmt.Sprintf("error reading the yaml spec %v: %v", spec_file, err),
		}
	}

	_logger.Info().Msg("image spec yaml parsed successfully")

	return &parsed_spec, nil
}

func performCopy(img *Image, step *ImageBuildStep, img_context string) *common.ImageBuildErr {
	_logger := common.GetLogger()

	_logger.Info().Msgf("copying %v to %v", step.Source, step.Destination)

	abs_source := fmt.Sprintf("%v/%v", img_context, step.Source)
	abs_dest := fmt.Sprintf("%v%v", img.ContextTempDir, step.Destination)

	_, err := os.Stat(abs_source)
	if os.IsNotExist(err) {
		return &common.ImageBuildErr{Code: 500, Message: "path does not exist"}
	}

	_logger.Info().Msgf("copied %v to %v", step.Source, step.Destination)
	return runCmd([]string{}, "cp", "-r", abs_source, abs_dest)
}

func performRun(img *Image, step *ImageBuildStep) *common.ImageBuildErr {
	_logger := common.GetLogger()

	_logger.Info().Msgf("running command: %v", step.Command)

	pid, _, _ := syscall.Syscall(syscall.SYS_FORK, 0, 0, 0)
	if pid == 0 {
		// inside child
		childEnv := []string{"PATH=/bin:/sbin:/usr/bin:/usr/sbin"}
		if err := syscall.Chdir(img.ContextTempDir); err != nil {
			return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error changing dir: %v", err)}
		}
		if err := syscall.Chroot(img.ContextTempDir); err != nil {
			return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error changing root: %v", err)}
		}
		err2 := runCmd(childEnv, step.Command[0], step.Command[1:]...)
		if err2 == nil {
			os.Exit(0)
		}
		os.Exit(1)

	} else {
		// inside parent
		var ws syscall.WaitStatus
		_, err := syscall.Wait4(int(pid), &ws, 0, nil)
		if err != nil {
			return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error waiting for child process: %v", err)}
		}
		if ws.Exited() {
			_logger.Info().Msgf("child process exited with status: %v", ws.ExitStatus())
			if ws.ExitStatus() != 0 {
				return &common.ImageBuildErr{Code: 500, Message: "something happend to the child proc"}
			} else {
				_logger.Info().Msg("command executed successfully")
				return nil
			}
		} else if ws.Signaled() {
			_logger.Info().Msgf("child process killed by signal: %v", ws.Signal())
			return &common.ImageBuildErr{Code: 500, Message: "something happend to the child proc"}
		} else {
			return &common.ImageBuildErr{Code: 500, Message: "something happend to the child proc"}
		}
	}
	return &common.ImageBuildErr{Code: 500, Message: "error"}
}

func performSteps(img *Image, spec *ImageSpec, img_context string) *common.ImageBuildErr {
	for _, step := range spec.Steps {
		if step.Type == "copy" {
			if err := performCopy(img, &step, img_context); err != nil {
				return err
			}
			continue
		}
		if step.Type == "run" {
			if err := performRun(img, &step); err != nil {
				return err
			}
			continue
		}
	}

	if err := os.MkdirAll(fmt.Sprintf("%v/etc/procman", img.ContextTempDir), 0755); err != nil {
		return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error creating conf: %v", err)}
	}

	yamlData, err := yaml.Marshal(&spec.Job)
	if err != nil {
		return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error creating conf: %v", err)}
	}

	filepath := fmt.Sprintf("%v/etc/procman/job.yaml", img.ContextTempDir)
	err = os.WriteFile(filepath, yamlData, 0644)
	if err != nil {
		return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error creating conf: %v", err)}
	}

	return nil
}

func buildAlpineBase(img *Image, spec *ImageSpec) *common.ImageBuildErr {
	_logger := common.GetLogger()

	_logger.Info().Msg("building alpine base")
	base := strings.Split(spec.Base, ":")
	// imgbase_src := base[0]
	imgbase_ver := base[1]

	arch := "x86_64"

	commands := [][]string{
		{"wget", "-q", "-O", fmt.Sprintf("%v/rootfs.tar.gz", img.ContextTempDir), fmt.Sprintf("http://dl-cdn.alpinelinux.org/alpine/v%v/releases/%v/alpine-minirootfs-%v.0-%v.tar.gz", imgbase_ver, arch, imgbase_ver, arch)},
		{"sh", "-c", fmt.Sprintf("cd %v && tar -xf rootfs.tar.gz && rm rootfs.tar.gz", img.ContextTempDir)},
		{"chmod", "755", img.ContextTempDir},
		{"find", img.ContextTempDir, "-type", "d", "-exec", "chmod", "755", "{}", ";"},
	}

	for _, cmd := range commands {
		if err := runCmd([]string{}, cmd[0], cmd[1:]...); err != nil {
			return err
		}
	}

	_logger.Info().Msg("base alpine build succeeded")
	return nil
}

func packageImage(img *Image) *common.ImageBuildErr {
	_logger := common.GetLogger()
	if err := os.MkdirAll(img.ImgPath, 0755); err != nil {
		_logger.Error().Msgf("error creating the imgpath dir: %v", err)
		return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error creating imgpath: %v", err)}
	}
	cmd := []string{"tar", "-czf", fmt.Sprintf("%v/img.tar.gz", img.ImgPath), "-C", img.ContextTempDir}
	return runCmd([]string{}, cmd[0], cmd[1:]...)
}

func deleteImageContext(img *Image) *common.ImageBuildErr {
	_logger := common.GetLogger()
	_logger.Info().Msg("deleting image context dir: " + img.ContextTempDir)
	cmd := []string{"rm", "-rf", img.ContextTempDir}
	return runCmd([]string{}, cmd[0], cmd[1:]...)
}

func writeImageMetadata(img *Image) *common.ImageBuildErr {
	_logger := common.GetLogger()
	_logger.Info().Msgf("writing metadata for image: %v", img)

	yamlData, err := yaml.Marshal(&img)
	if err != nil {
		return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error writing image metadata: %v", err)}
	}

	filepath := fmt.Sprintf("%v/img.yaml", img.ImgPath)
	err = os.WriteFile(filepath, yamlData, 0644)
	if err != nil {
		return &common.ImageBuildErr{Code: 500, Message: fmt.Sprintf("error creating conf: %v", err)}
	}

	_logger.Info().Msgf("successfully wrote metadata for image: %v", img)
	return nil
}

func buildImage(img *Image, img_context string) *common.ImageBuildErr {
	_logger := common.GetLogger()
	_logger.Info().Msgf("starting image build: %v", img)

	spec, err := parseImageSpec(img_context)
	if err != nil {
		return err
	}

	err = buildAlpineBase(img, spec)
	if err != nil {
		return err
	}

	if err = performSteps(img, spec, img_context); err != nil {
		return err
	}

	if err = packageImage(img); err != nil {
		return err
	}

	if err = deleteImageContext(img); err != nil {
		return err
	}

	img.Created = time.Now().UTC().Format("2006-01-02 15:04:05")

	if err = writeImageMetadata(img); err != nil {
		return err
	}

	return nil

}
