# roonit
"roonit" is a simple tool for enabling and disabling services in runit (Void Linux).

## "Installing"
 1. Run the `make` command to compile
 2. Then run `make install` to install the compiled executable to /usr/local/bin

## "Uninstalling"
 1. Run `make remove`

## Usage
`roonit enable <service>` will enable a service located in /etc/sv/  
`roonit disable <service>` will remove a service in /var/service/  
`roonit start <service>` will start a service  
`roonit stop <service>` will stop a service  
`roonit status <service>` will show the status of a service (adding '-a' will show all services)  

## TODO:
  - Figure out how to use `symlinkat()`
  - Make this look neater
  - Fix this garbage code
  - Properly learn the C programming language

## Credits/Special Thanks
 - Special thanks to everybody on the voidlinux subreddit
 - You should check out https://github.com/bahamas10/vsv it's a shell script that does this and beyond
