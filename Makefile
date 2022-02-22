suid: timer
	chmod u+s timer
	su -c "chown root timer"

.PHONY: suid
