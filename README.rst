                            
                    nionhalt
                            

A universal, minimalist shutdown and reboot wrapper written in C
for `OpenRC` and `runit` init systems.

'Description

nionhalt is designed to be a lightweight, "suckless"
utility that detects the active init system and executes the appropriate
power management commands. It includes a built-in logging system to keep track of power actions.

'Features

- **Universal Detection:** Automatically identifies if the system 
   is running OpenRC or runit.
- **Logging:** Records every attempt to shutdown or 
   reboot in ``/var/log/nionhalt.log``, including the username and timestamp.
- **Simulation Mode:** Automatically enters a safe "dry run" mode if
 executed without root privileges.
- **Minimalist:** Small binary footprint with zero heavy dependencies.


'Logging

All events are logged to ``/var/log/nionhalt.log``. You can check the history by running:

'License

MIT License - Feel free to use and modify.