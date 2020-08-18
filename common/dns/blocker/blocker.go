// +build !windows

package blocker

import (
	"errors"
)

// BlockOutsideDNS block outside dns
func BlockOutsideDNS(tunName string) error {
	return errors.New("not implemented")
}
