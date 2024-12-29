package images

import (
	"fmt"
	"os"
	"strings"

	"github.com/rutu-sh/procman/internal/common"

	"github.com/google/uuid"
	"gopkg.in/yaml.v3"
)

/*
Takes the image name and the absolute path to the setup script as argument and builds a tar.gz with dependencies injected.

Args:
  - name string: the name of the image
  - img_context string: the absolute path to the image context dir

Returns:
  - Image object with attributes containing details for the container image
  - ImageBuildErr
*/
func BuildImage(name string, tag string, img_context string) (*Image, *common.ImageBuildErr) {

	_logger := common.GetLogger()
	_logger.Info().Msgf("creating image '%v' using setup script %v", name, img_context)

	img_id := strings.Split(uuid.New().String(), "-")[0]
	img := &Image{
		Id:             img_id,
		Name:           name,
		ContextTempDir: getImageContextDir(img_id),
		Tag:            tag,
		ImgPath:        getImageDir(img_id),
	}

	if err := os.MkdirAll(img.ContextTempDir, 0755); err != nil {
		return nil, &common.ImageBuildErr{
			Code:    500,
			Message: fmt.Sprintf("context creation failed with error: %v", err),
		}
	}

	if err := buildImage(img, img_context); err != nil {
		return nil, &common.ImageBuildErr{
			Code:    500,
			Message: fmt.Sprintf("context creation failed with error: %v", err),
		}
	}

	return img, nil
}

/*
Reads the images available locally and returns a list containing the metadata.
*/
func ListImages() (*[]*Image, *common.ImageListErr) {

	_logger := common.GetLogger()

	_logger.Debug().Msgf("listing images")

	img_dir := getParentImgDir()

	images := []*Image{}

	_, err := os.Stat(img_dir)
	if !os.IsExist(err) {
		dirs, err := os.ReadDir(img_dir)
		if err != nil {
			_logger.Error().Msgf("error reading dir: %v", err)
			return &[]*Image{}, &common.ImageListErr{Code: 500, Message: fmt.Sprintf("error reading dir: %v", err)}
		}
		for _, dir := range dirs {
			dirName := dir.Name()
			imgMetadataFile := fmt.Sprintf("%v/%v/img.yaml", img_dir, dirName)
			if _, err := os.Stat(imgMetadataFile); err != nil {
				_logger.Error().Msgf("error statfile: %v", err)
				continue
			}

			data, err := os.ReadFile(imgMetadataFile)
			if err != nil {
				_logger.Error().Msgf("error readfile: %v", err)
				continue
			}
			parsedImgMetadata := Image{}
			err = yaml.Unmarshal(data, &parsedImgMetadata)

			if err != nil {
				_logger.Error().Msgf("error unmarshall: %v", err)
				continue
			}

			images = append(images, &parsedImgMetadata)
		}
	}
	return &images, nil
}

/*
Get the image by id or name and tag.

Args:
  - image_id : uniqe id of the image
*/
func GetImage(image_id string, name string, tag string) (*Image, *common.ImageGetErr) {
	_logger := common.GetLogger()

	if image_id != "" {
		imgMetadataFile := getImageDir(image_id)

		if _, err := os.Stat(imgMetadataFile); err != nil {
			_logger.Error().Msgf("error statfile: %v", err)
			return nil, &common.ImageGetErr{Code: 500, Message: fmt.Sprintf("error statfile: %v", err)}
		}

		data, err := os.ReadFile(imgMetadataFile)
		if err != nil {
			_logger.Error().Msgf("error readfile: %v", err)
			return nil, &common.ImageGetErr{Code: 500, Message: fmt.Sprintf("error readfile: %v", err)}
		}

		parsedImageMetadata := &Image{}
		err = yaml.Unmarshal(data, parsedImageMetadata)
		if err != nil {
			_logger.Error().Msgf("error unmarshal: %v", err)
			return nil, &common.ImageGetErr{Code: 500, Message: fmt.Sprintf("error unmarshal: %v", err)}
		}

		return parsedImageMetadata, nil
	} else if name != "" && tag != "" {
		images, err := ListImages()
		if err != nil {
			_logger.Error().Msgf("error list: %v", err)
			return nil, &common.ImageGetErr{Code: err.Code, Message: err.Message}
		}

		for _, img := range *images {
			if img.Name == name && img.Tag == tag {
				return img, nil
			}
		}
	}

	return nil, nil

}

/*
Delete the image by id
Args:
  - image_id string: unique id of the image

Return:
  - error
*/
func DelImage(image_id string) *common.ImageDelErr {
	_logger := common.GetLogger()
	_logger.Info().Msgf("deleting image with id: %v", image_id)

	imgdir := getImageDir(image_id)
	command := []string{"rm", "-rf", imgdir}
	runErr := runCmd([]string{}, command[0], command[1:]...)
	if runErr != nil {
		_logger.Error().Msgf("error del: %v", runErr)
		return &common.ImageDelErr{Message: fmt.Sprintf("error getting image: %v", runErr)}
	}
	return nil
}
