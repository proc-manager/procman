package common

import "fmt"

// image errors

type ImageBuildErr struct {
	Code    int    `json:"code"`
	Message string `json:"message,omitempty"`
}

func (e *ImageBuildErr) Error() string {
	return fmt.Sprintf("Error %d: %s", e.Code, e.Message)
}

type ImageListErr struct {
	Code    int    `json:"code"`
	Message string `json:"message,omitempty"`
}

func (e *ImageListErr) Error() string {
	return fmt.Sprintf("Error %d: %s", e.Code, e.Message)
}

type ImageGetErr struct {
	Code    int    `json:"code"`
	Message string `json:"message,omitempty"`
}

func (e *ImageGetErr) Error() string {
	return fmt.Sprintf("Error %d: %s", e.Code, e.Message)
}

type ImageDelErr struct {
	Code    int    `json:"code"`
	Message string `json:"message,omitempty"`
}

func (e *ImageDelErr) Error() string {
	return fmt.Sprintf("Error %d: %s", e.Code, e.Message)
}

// proc errors

type ProcStartErr struct {
	Code    int    `json:"code"`
	Message string `json:"message,omitempty"`
}

func (e *ProcStartErr) Error() string {
	return fmt.Sprintf("Error %d: %s", e.Code, e.Message)
}
