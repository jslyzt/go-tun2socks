package simple

import (
	golog "log"

	"tun2socks/common/log"
)

type simpleLogger struct {
	level log.Level
}

// NewSimpleLogger 新建
func NewSimpleLogger() log.Logger {
	return &simpleLogger{log.INFO}
}

// SetLevel 设置等级
func (l *simpleLogger) SetLevel(level log.Level) {
	l.level = level
}

// Debugf debug format
func (l *simpleLogger) Debugf(msg string, args ...interface{}) {
	if l.level <= log.DEBUG {
		l.output(msg, args...)
	}
}

// Infof info format
func (l *simpleLogger) Infof(msg string, args ...interface{}) {
	if l.level <= log.INFO {
		l.output(msg, args...)
	}
}

// Warnf warn format
func (l *simpleLogger) Warnf(msg string, args ...interface{}) {
	if l.level <= log.WARN {
		l.output(msg, args...)
	}
}

// Errorf error format
func (l *simpleLogger) Errorf(msg string, args ...interface{}) {
	if l.level <= log.ERROR {
		l.output(msg, args...)
	}
}

// Fatalf fatal format
func (l *simpleLogger) Fatalf(msg string, args ...interface{}) {
	golog.Fatalf(msg, args...)
}

func (l *simpleLogger) output(msg string, args ...interface{}) {
	golog.Printf(msg, args...)
}

func init() {
	log.RegisterLogger(NewSimpleLogger())
}
