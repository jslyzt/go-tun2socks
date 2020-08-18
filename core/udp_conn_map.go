package core

import (
	"sync"
)

var udpConns sync.Map

type udpConnID struct {
	src string
}
