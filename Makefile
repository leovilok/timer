suid: timer
	su -c "chown root timer && chmod u+s timer"

.PHONY: suid
