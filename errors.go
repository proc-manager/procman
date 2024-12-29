package procman

type ImageError struct {
	Message string `json:"message,omitempty"`
}

func (e *ImageError) Error() string {
	return e.Message
}

type ImageListError struct {
	Message string `json:"message,omitempty"`
}

func (e *ImageListError) Error() string {
	return e.Message
}
