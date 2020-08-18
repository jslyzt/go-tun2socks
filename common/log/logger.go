package log

// Level 日志等级
type Level uint8

// 等级定义
const (
	DEBUG Level = iota
	INFO
	WARN
	ERROR
	NONE
)

// Logger 日志
type Logger interface {
	SetLevel(level Level)
	Debugf(msg string, args ...interface{})
	Infof(msg string, args ...interface{})
	Warnf(msg string, args ...interface{})
	Errorf(msg string, args ...interface{})
	Fatalf(msg string, args ...interface{})
}
