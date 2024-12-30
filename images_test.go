package procman

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestBuild(t *testing.T) {
	res, err := BuildImage("test-img", "0.0.5", "./examples/alpine-basic")
	assert.Empty(t, err)
	assert.NotEmpty(t, res)

	res2, err2 := GetImage("", "test-img", "0.0.5")
	assert.Empty(t, err2)
	assert.NotEmpty(t, res2)

	err3 := DelImage(res.Id, "", "")
	assert.Empty(t, err3)
	assert.NotEmpty(t, err3)
}
