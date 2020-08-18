package log

var (
	logger Logger
)

// RegisterLogger 注册
func RegisterLogger(l Logger) {
	logger = l
}

// SetLevel 设置等级
func SetLevel(level Level) {
	if logger != nil {
		logger.SetLevel(level)
	}
}

// Debugf debug format
func Debugf(msg string, args ...interface{}) {
	if logger != nil {
		logger.Debugf(msg, args...)
	}
}

// Infof info format
func Infof(msg string, args ...interface{}) {
	if logger != nil {
		logger.Infof(msg, args...)
	}
}

// Warnf warn format
func Warnf(msg string, args ...interface{}) {
	if logger != nil {
		logger.Warnf(msg, args...)
	}
}

// Errorf error format
func Errorf(msg string, args ...interface{}) {
	if logger != nil {
		logger.Errorf(msg, args...)
	}
}

// Fatalf fatal format
func Fatalf(msg string, args ...interface{}) {
	if logger != nil {
		logger.Fatalf(msg, args...)
	}
}
