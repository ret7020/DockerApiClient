# Docker API Client 
This is an attempt to create client for docker api library for C++

## Dependencies
- libfmt
- libcurl

## Build
To build you have to install g++ compiler on your system, and then run `build.sh` script. Output binary is `client`. We will add support for `make` later

## API methods
- [x] List containers
- [ ] Create container
- [ ] Inspect container
- [ ] List processes inside container
- [ ] Container logs
- [ ] Container FS changes
- [ ] Export container
- [ ] Container status (resources usage)
- [ ] Resize Container TTY
- [x] Start container
- [x] Stop container
- [x] Restart container
- [ ] Kill container
- [ ] Update container
- [ ] Rename container
- [ ] Pause container
- [ ] Unpause container
- [ ] Attach to container
