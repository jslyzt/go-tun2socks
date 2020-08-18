package core

import (
	"sync"
)

var (
	pool *sync.Pool
)

// 常量定义
const (
	BufSize = 2 * 1024
)

// SetBufferPool 设置pool
func SetBufferPool(p *sync.Pool) {
	pool = p
}

// NewBytes 新建bytes
func NewBytes(size int) []byte {
	if size <= BufSize {
		return pool.Get().([]byte)
	}
	return make([]byte, size)
}

// FreeBytes 释放bytes
func FreeBytes(b []byte) {
	if len(b) >= BufSize {
		pool.Put(b)
	}
}

func init() {
	SetBufferPool(&sync.Pool{
		New: func() interface{} {
			return make([]byte, BufSize)
		},
	})
}
